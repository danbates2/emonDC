// emonDC extras, Daniel Bates SEP, 2019.
// Free to use and modify.
#include "wifi.h"
#include "emonesp.h"
#include "emondc.h"
#include "input.h"
#include "config.h"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "AH_MCP320x.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// -------------------------------------------------------------------
// change the following variable for the system.
// -------------------------------------------------------------------

unsigned long _MAIN_INTERVAL = 5; // seconds

bool chanAref = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
bool chanBref = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.

float chanA_shuntAmp_gain = 100; // shunt amp gain.
float chanB_shuntAmp_gain = 100;

// values of resistor divider for voltage reading.
float R1_A = 1000000.0; // 1Mohm default.
float R2_A = 75000.0; // 75kohm default.
float R1_B = 1000000.0;
float R2_B = 75000.0;
float Rshunt_A = 0.000500; // value of Rsense in ohms.
float Rshunt_B = 0.000500;

// Calibration values for manual adjustment.
float _CAL_FACTOR1 = 1.00; // CURRENT_A.
float _CAL_FACTOR2 = 1.00; // VOLTAGE_A.
float _CAL_FACTOR3 = 1.00; // CURRENT_B.
float _CAL_FACTOR4 = 1.00; // VOLTAGE_B.
float _CAL_FACTOR_TEST = 1.00; // ADC_CHANNEL_7.

/*
  todo:
  microSD card logging scheme || low power mode.. void savetoSDcard() {   }
  Midnight calibration.
  Serial commands.
  wake from sleep routine
  Allowable discharge depth? relation to Peukert?

  notes:
  ESP.deepSleep(microseconds, mode) will put the chip into deep sleep. mode is one of:
  WAKE_RF_DEFAULT
  WAKE_RFCAL
  WAKE_NO_RFCAL
  WAKE_RF_DISABLED

  data model:
  main_emondc_interval
  chanAref
  chanBref
  R1_A
  R2_A
  R1_B
  R2_B
  Rshunt_A
  Rshunt_B
  chanA_shuntAmp_gain
  chanB_shuntAmp_gain
  _CAL_FACTOR1
  _CAL_FACTOR2
  _CAL_FACTOR3
  _CAL_FACTOR4
  _CAL_FACTOR_TEST
  _t_begin
  RFM69_enabled
  battery_voltage_nominal
  peukert_exponent
  rated_discharge_time
  rated_capacity
  sendData_go
  low_power_mode
  waking_time
  sleeping_time
  Valarm_A_HIGH
  Valarm_A_LOW
  Valarm_A_VERY_LOW
  Valarm_B_HIGH
  Valarm_B_LOW
  Valarm_B_VERY_LOW
  Calarm_A_HIGH
  Calarm_B_HIGH
  TempAlarmBatHIGH
  TempAlarmBatLOW
  TempAlarmRoomHIGH
  TempAlarmRoomLOW
  midnight_calibration
  chanA_shuntAmp_gain
  chanB_shuntAmp_gain
  Wh_chA
  Wh_chB
*/



unsigned long main_emondc_interval = _MAIN_INTERVAL * 1000;

unsigned long _t;

bool RFM69_enabled = false; // RFM69Pi compatibility.



// -------------------------------------------------------------------
// 'ADVANCED' STUFF BELOW HERE
// -------------------------------------------------------------------


const int chipSelectSD = 15; // SD card

AH_MCP320x ADC_SPI(2);  

int _t_begin;

float Wh_chA = 0.0;
float Wh_chB = 0.0;

float Wh_accumulate(float current_value, float voltage_value, float elapsed_seconds) {
  float watts = current_value * voltage_value;
  float watt_seconds = watts * elapsed_seconds;
  float wh = watt_seconds / 3600;
  return wh;
}

unsigned long rtc_now_time = 0; 

float CH_A_CURRENT_AVERAGED;
float CH_A_VOLTAGE_AVERAGED;
float CH_B_CURRENT_AVERAGED;
float CH_B_VOLTAGE_AVERAGED;
float VREF_UNI_AVERAGED;
float VREF_BI_AVERAGED;
float VREF33_AVERAGED;
float CH8_AVERAGED;

float Voltage_A;
float Current_A;
float Voltage_B;
float Current_B;

int battery_voltage_nominal = 12;
float State_of_ChargeB; // percentage SoC, related to temperature, rate of discharge and peukert coefficient.
float Temperature_Room;
float Temperature_Bat;
float peukert_exponent = 1.20; // can be derived from the 5h and 20h ratings of a battery.
float rated_discharge_time = 20.0; // hours (Rt, or H)
float rated_capacity = 100.0; // amp-hours (C)

float time_until_discharged(void) {
  //t = Rt(C/IRt)^k [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  float IRt = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB) * rated_discharge_time;
  float C_IRt = rated_capacity / IRt;
  float Rt_C_IRt = rated_discharge_time * C_IRt;
  float _time_until_discharged = pow(Rt_C_IRt, 2.0);
  return _time_until_discharged;
}

float effective_capacity(void) {
  // It = C (C / I*H)^k-1 [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  float _IH = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB) * rated_discharge_time;
  float _C_IH = rated_capacity / _IH;
  float _C_C_IH = rated_capacity * _C_IH;
  float _k_neg_one = 1 / peukert_exponent;
  float _result_It = pow(_C_C_IH, _k_neg_one);
  return _result_It;
}

float volts_to_adc_reading_ratio;

float volts_to_adc_reading_ratio_function(void) {
  float _volts_to_adc_reading_ratio = 3.3000 / VREF33_AVERAGED;
  return (_volts_to_adc_reading_ratio);
}


float Vcal_coefficient_A_pre = (R1_A + R2_A); // resistor divider calc.
float Vcal_coefficient_A = R2_A / Vcal_coefficient_A_pre;
float Vcal_coefficient_B_pre = (R1_B + R2_B); // resistor divider calc.
float Vcal_coefficient_B = R2_B / Vcal_coefficient_B_pre;

// adc value to Volts function:
float make_readable_Volts (float _adcValue, String _chan) {
  float _readableV1 = _adcValue * volts_to_adc_reading_ratio;
  if (_chan == chanA) {
    float _readableV2 = _readableV1 / Vcal_coefficient_A;
    return (_readableV2);
  }
  else if (_chan == chanB) {
    float _readableV2 = _readableV1 / Vcal_coefficient_B;
    return (_readableV2);
  }
}

// adc value to Amps function:
float make_readable_Amps (float _adcValue, String _chan, float _gain)
{
  float _readable_1 = _adcValue * volts_to_adc_reading_ratio;
  float _readable_2 = _readable_1 / _gain; // gives mV signal value.

  if (_chan == chanA) {
    float _readable_current = _readable_2 / Rshunt_A; // I=V/R
    return (_readable_current);
  }
  else if (_chan == chanB) {
    float _readable_current = _readable_2 / Rshunt_B; // I=V/R
    //Serial.println(_readableC3);
    return (_readable_current);
  }
}

int averaging_loop_counter = 0;

bool sendData_go = true;

bool low_power_mode = false;
unsigned int waking_time = 6000;
unsigned int sleeping_time = 10000;

float Valarm_A_HIGH;
float Valarm_A_LOW;
float Valarm_A_VERY_LOW;
float Valarm_B_HIGH;
float Valarm_B_LOW;
float Valarm_B_VERY_LOW;
float Calarm_A_HIGH;
float Calarm_B_HIGH;
float TempAlarmBatHIGH;
float TempAlarmBatLOW;

bool midnight_calibration = false;
bool setup_done = 0;

unsigned int ADC_DelayTest = 1; // microsecond delay in ADC routine, to improve accuracy, related to the Sample and Hold architecture of the ADC.

// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// OLED Display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET  -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int screentog = 0;



// MCP3208
#define MCP3208_CS_PIN    2 // CS
#define DATAOUT_MCP3208    13 // MOSI
#define DATAIN_MCP3208     12 // MISO
#define SPICLOCK          14 // CLK

unsigned long previousMillis = 0;

int CHANNEL1 = 0;
int CHANNEL2 = 0;
int CHANNEL3 = 0;
int CHANNEL4 = 0;
int CHANNEL5 = 0;
int CHANNEL6 = 0;
int CHANNEL7 = 0;
int CHANNEL8 = 0;
unsigned long CH_A_CURRENT_ACCUMULATOR = 0;
unsigned long CH_A_VOLTAGE_ACCUMULATOR = 0;
unsigned long CH_B_CURRENT_ACCUMULATOR = 0;
unsigned long CH_B_VOLTAGE_ACCUMULATOR = 0;
unsigned long VREF_UNI_ACCUMULATOR = 0;
unsigned long VREF_BI_ACCUMULATOR = 0;
unsigned long VREF33_ACCUMULATOR = 0;
unsigned long CH8_ACCUMULATOR = 0;

unsigned long numberofsamples = 0;

String ADC_KeyValue_String = "";

unsigned long currentMillis;

#define NTCENABLE
#ifdef NTCENABLE
//int time_offset = 3600; //GMT +0
int time_offset = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", time_offset, 300000);
unsigned long previousMillisNTC = 0;
//unsigned long main_emondc_interval;
unsigned long intervalNTC = main_emondc_interval;
#endif



String datalogFilename = "datalog.txt";


void save_to_SDcard(void) {
  
  //using sd::File;
  File dataFile = SD.open(datalogFilename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(ADC_KeyValue_String);
    dataFile.close();
    // print to the serial port too:
    Serial.println(ADC_KeyValue_String);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  //SD::SD.end();
}


void emondc_setup(void) {
  //-----------------------------------
  // microSD card init
  //-----------------------------------
  //SPI.setClockDivider(SPI_CLOCK_DIV128);
  //SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
  

  pinMode(chipSelectSD, OUTPUT);
  digitalWrite(chipSelectSD, HIGH); delay(1);
  if (SD.begin(chipSelectSD, 400000)) {
    Serial.println("SDcard initialised.");
    // File myFile;
    // myFile = SD.open("datalog.txt", FILE_WRITE);
    // myFile.close();
    // Serial.println("SD card file created");
  }
  else {
    Serial.println("SD card not detected.");
  }

  //-----------------------------------
  // MCP3208 ADC init
  //-----------------------------------
  /*
  pinMode(MCP3208_CS_PIN, OUTPUT);
  pinMode(DATAOUT_MCP3208, OUTPUT);
  pinMode(DATAIN_MCP3208, INPUT);
  pinMode(SPICLOCK, OUTPUT);

  digitalWrite(MCP3208_CS_PIN, HIGH);
  digitalWrite(DATAOUT_MCP3208, LOW);
  digitalWrite(SPICLOCK, LOW);
  */
  //SPI.setClockDivider(SPI_CLOCK_DIV128);

  //-----------------------------------
  // OLED Display init
  //-----------------------------------

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    // for (;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner


  display.println(F("emonDCduo"));
  display.println(F("v3.6 N19"));
  screentog = 1;
  display.display();
  delay(1500);

  //-----------------------------------
  // RTC init
  //-----------------------------------
  if (!rtc.begin()) {
    Serial.println("RTC/I2C not functioning.");
  }

  else if (!rtc.initialized()) {
    Serial.println("RTC not running,\r\n    - attempting initialisation with compile time.");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  else {
    Serial.println("RTC initialised.");
  }

#ifdef NTCENABLE
      timeClient.setTimeOffset(time_offset);

    timeClient.begin();
    #endif

  delay(10);
}

void NTC_update_PCF8523_update(void) {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    rtc.adjust(timeClient.getEpochTime());
  }
  DateTime now = rtc.now();
  rtc_now_time = now.unixtime();
}


void emondc_loop(void) {

  currentMillis = millis();

  CHANNEL1 = ADC_SPI.readCH(0);
  CHANNEL2 = ADC_SPI.readCH(1);
  CHANNEL3 = ADC_SPI.readCH(2);
  CHANNEL4 = ADC_SPI.readCH(3);
  CHANNEL5 = ADC_SPI.readCH(5);
  CHANNEL6 = ADC_SPI.readCH(4);
  CHANNEL7 = ADC_SPI.readCH(6);
  CHANNEL8 = ADC_SPI.readCH(7);

  /*
  CHANNEL1 = read_adc(1);
  CHANNEL2 = read_adc(2);
  CHANNEL3 = read_adc(3);
  CHANNEL4 = read_adc(4);
  CHANNEL5 = read_adc(6);
  CHANNEL6 = read_adc(5);
  CHANNEL7 = read_adc(7);
  CHANNEL8 = read_adc(8); // Labeled on the PCB as CH7.
  */

  CH_A_CURRENT_ACCUMULATOR = CH_A_CURRENT_ACCUMULATOR + CHANNEL1;
  CH_A_VOLTAGE_ACCUMULATOR = CH_A_VOLTAGE_ACCUMULATOR + CHANNEL2;
  CH_B_CURRENT_ACCUMULATOR = CH_B_CURRENT_ACCUMULATOR + CHANNEL3;
  CH_B_VOLTAGE_ACCUMULATOR = CH_B_VOLTAGE_ACCUMULATOR + CHANNEL4;
  VREF_UNI_ACCUMULATOR = VREF_UNI_ACCUMULATOR + CHANNEL5;
  VREF_BI_ACCUMULATOR = VREF_BI_ACCUMULATOR + CHANNEL6;
  VREF33_ACCUMULATOR = VREF33_ACCUMULATOR + CHANNEL7;
  CH8_ACCUMULATOR = CH8_ACCUMULATOR + CHANNEL8;
  numberofsamples++;

  if (currentMillis - previousMillis >= main_emondc_interval) {
    //int overrunMillis = currentMillis % main_emondc_interval;
    unsigned long pre_mills_store = previousMillis;
    previousMillis = currentMillis;

    //previousMillis = previousMillis - overrunMillis;

    if (numberofsamples >= 1000) {
      
      average_and_calibrate(pre_mills_store, currentMillis);
      
      NTC_update_PCF8523_update();

      //-----------------------------------
      // what to do with the ready data:
      //-----------------------------------
          
      forward_to_emonESP(); // sending to emonCMS?

      // print_readable(); // debugging...

      // forward_to_RFM69Pi(); // not tested
     
      Serial.print("FreeRAM (bytes): ");  Serial.println(ESP.getFreeHeap()); // for testing

      save_to_SDcard(); // 

      drawvalues_to_OLED(); // for the 128x32 I2C OLED.

      clear_accumulators(); // this happens before gathering next samples, or else chaos ensues.
    }
  }
}


void average_and_calibrate(unsigned long pre_mills, unsigned long curr_mills) {

  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
  VREF_UNI_AVERAGED = VREF_UNI_ACCUMULATOR / numberofsamples;
  VREF_BI_AVERAGED = VREF_BI_ACCUMULATOR / numberofsamples;
  VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
  CH8_AVERAGED = CH8_ACCUMULATOR / numberofsamples;

  // CALIBRATE
  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_AVERAGED * _CAL_FACTOR1;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_AVERAGED * _CAL_FACTOR2;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_AVERAGED * _CAL_FACTOR3;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_AVERAGED * _CAL_FACTOR4;
  CH8_AVERAGED = CH8_AVERAGED * _CAL_FACTOR_TEST;


  // adjust current readings according to Vref values.
  if (chanAref == chanRef) {
    CH_A_CURRENT_AVERAGED = CH_A_CURRENT_AVERAGED - VREF_UNI_AVERAGED;
  }
  else {
    CH_A_CURRENT_AVERAGED = CH_A_CURRENT_AVERAGED - VREF_BI_AVERAGED;
  }

  if (chanBref == chanRef) {
    CH_B_CURRENT_AVERAGED = CH_B_CURRENT_AVERAGED - VREF_UNI_AVERAGED;
  }
  else {
    CH_B_CURRENT_AVERAGED = CH_B_CURRENT_AVERAGED - VREF_BI_AVERAGED;
  }

  volts_to_adc_reading_ratio = volts_to_adc_reading_ratio_function(); // convert VREF33 into 'ADCvalue to Voltage factor'

  Current_A = make_readable_Amps(CH_A_CURRENT_AVERAGED, chanA, chanA_shuntAmp_gain) * _CAL_FACTOR1;
  Voltage_A = make_readable_Volts(CH_A_VOLTAGE_AVERAGED, chanA) * _CAL_FACTOR2;
  Current_B = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB, chanB_shuntAmp_gain) * _CAL_FACTOR3;
  Voltage_B = make_readable_Volts(CH_B_VOLTAGE_AVERAGED, chanB) * _CAL_FACTOR4;
  CH8_AVERAGED = CH8_AVERAGED * _CAL_FACTOR_TEST;

  //Voltage_A = Voltage_A - 0.02;
  //Voltage_B = Voltage_B - 0.02;

  int this_interval = curr_mills - pre_mills;
  Wh_chA = Wh_chA + Wh_accumulate(Current_A, Voltage_A, this_interval / 1000);
  Wh_chB = Wh_chB + Wh_accumulate(Current_B, Voltage_B, this_interval / 1000);

  if (setup_done == false) {
    _t_begin = millis() / 1000;
    setup_done = true;
  }

  int _t_pre = millis() / 1000;
  _t = _t_pre - _t_begin;

  averaging_loop_counter++;

#ifndef RELEASE
  Serial.print("Number of samples: ");
  Serial.println(numberofsamples);
  Serial.print("Averaging loop counter: ");
  Serial.println(averaging_loop_counter);
  Serial.print("_time: ");
  Serial.println(_t);
#endif

} // Averaging loop end.

void print_readable(void) {
  Serial.println();
#ifndef RELEASE
  Serial.println("print_readable:");
#endif
  Serial.print("CH_A_CURRENT: ");
  Serial.println(Current_A);
  Serial.print("CH_A_VOLTAGE: ");
  Serial.println(Voltage_A);
  Serial.print("CH_B_CURRENT: ");
  Serial.println(Current_B);
  Serial.print("CH_B_VOLTAGE: ");
  Serial.println(Voltage_B);
  Serial.print("VREF_UNI: ");
  Serial.println(VREF_UNI_AVERAGED * volts_to_adc_reading_ratio);
  Serial.print("VREF_BI: ");
  Serial.println(VREF_BI_AVERAGED * volts_to_adc_reading_ratio);
  Serial.print("VREF33: ");
  Serial.println(VREF33_AVERAGED * volts_to_adc_reading_ratio);
  Serial.print("CH8_ADC_AVERAGED: ");
  Serial.println(CH8_AVERAGED);
  Serial.print("_time: ");
  Serial.println(_t);
}



void forward_to_emonESP(void)
{
  ADC_KeyValue_String = "Volts_A:" + String(Voltage_A, 2);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_A;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Volts_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Voltage_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Ref_Uni:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_UNI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Ref_Bi:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_BI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "CH8:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH8_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Wh_chA:";
  ADC_KeyValue_String = ADC_KeyValue_String + Wh_chA;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Wh_chB:";
  ADC_KeyValue_String = ADC_KeyValue_String + Wh_chB;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "samplecount:";
  ADC_KeyValue_String = ADC_KeyValue_String + numberofsamples;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "time:";
  //_t = rtc_now_time;
  ADC_KeyValue_String = ADC_KeyValue_String + rtc_now_time;
  input_string = ADC_KeyValue_String;
}


void clear_accumulators(void) {
  CH_A_CURRENT_ACCUMULATOR = 0;
  CH_A_VOLTAGE_ACCUMULATOR = 0;
  CH_B_CURRENT_ACCUMULATOR = 0;
  CH_B_VOLTAGE_ACCUMULATOR = 0;
  VREF_UNI_ACCUMULATOR = 0;
  VREF_BI_ACCUMULATOR = 0;
  VREF33_ACCUMULATOR = 0;
  CH8_ACCUMULATOR = 0;
  numberofsamples = 0;
}


// Read ADC function for MCP3208
int read_adc(int channel) {
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
  //B10100000
  //allow channel selection
  commandbits |= ((channel - 1) << 3);

  digitalWrite(MCP3208_CS_PIN, LOW); //Select adc
  // setup bits to be written
  for (int i = 7; i >= 3; i--) {
    digitalWrite(DATAOUT_MCP3208, commandbits & 1 << i);
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }

  digitalWrite(SPICLOCK, HIGH);   //ignores 2 null bits
  digitalWrite(SPICLOCK, LOW);
  digitalWrite(SPICLOCK, HIGH);
  digitalWrite(SPICLOCK, LOW);

  //read bits from adc
  for (int i = 11; i >= 0; i--) {
    adcvalue += digitalRead(DATAIN_MCP3208) << i;
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }
  digitalWrite(MCP3208_CS_PIN, HIGH); //turn off device
  return adcvalue;
} // Read ADC function for MCP3208


void testscrolltext(String scrollstring) {
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(scrollstring);
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  //display.startscrolldiagright(0x00, 0x07);
  // delay(2000);
  //display.startscrolldiagleft(0x00, 0x07);
  //delay(2000);
  //display.stopscroll();
  //delay(1000);
}



void RTC_PCF8523_PRINT(void) {
  DateTime now = rtc.now();

  Serial.print("RTC_PCF8523:");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  //Serial.println();

  rtc_now_time = now.unixtime();
  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(rtc_now_time);
  Serial.print("s = ");
  Serial.print(rtc_now_time / 86400L);
  Serial.println("d");

}

void drawvalues_to_OLED(void) { // draw to OLED
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner

  if (screentog == 0) {
    display.println(F("Volts(A)")); display.println(Voltage_A);
    screentog = 1;
  }
  else if (screentog == 1) {
    display.println(F("Amps(A)")); display.println(Current_A);
    screentog = 2;
  }
  else if (screentog == 2) {
    display.println(F("Volts(B)")); display.println(Voltage_B);
    screentog = 3;
  }
  else if (screentog == 3) {
    display.println(F("Amps(B)")); display.println(Current_B);
    screentog = 4;
  }
  else if (screentog == 4) {
    display.println(F("Wh(A)")); display.println(Wh_chA);
    screentog = 5;
  }
  else if (screentog == 5) {
    display.println(F("Wh(B)")); display.println(Wh_chB);
    screentog = 6;
  }
  else if (screentog == 6) {
    display.println(F("SSID:")); display.println(esid);
    screentog = 7;
  }
  /*  else if (screentog == 7) {
      display.print(now.year(), DEC);
      display.print(F("/"));
      if (now.month() <= 9) {
        display.print(F("0"));
      }
      display.print(now.month(), DEC);
      display.print(F("/"));
      if (now.day() <= 9) {
        display.print(F("0"));
      }
      display.println(now.day(), DEC);

      display.print(F("  "));
      display.print(now.hour(), DEC);
      display.print(F(":"));
      if (now.minute() <= 9) {
        display.print(F("0"));
      }
      display.print(now.minute(), DEC);
      display.print(F(":"));
      if (now.second() <= 9) {
        display.print(F("0"));
      }
      display.print(now.second(), DEC);
      display.println();

      //display.display(); /////

      screentog = 8;
    }
  */
  else if (screentog == 7) {
    display.println(F("IP Addr:")); display.println(ipaddress_OLED);
    //display.display(); /////
    screentog = 0;
  }

  display.display();
} // draw to OLED


void config_save_emondc(unsigned long  interval)
{
  main_emondc_interval = interval * 1000;

  /*
    EEPROM_write_string(EEPROM_WWW_USER_START, EEPROM_WWW_USER_SIZE, user);
    EEPROM_write_string(EEPROM_WWW_PASS_START, EEPROM_WWW_PASS_SIZE, pass);

    EEPROM.commit();
  */

}
