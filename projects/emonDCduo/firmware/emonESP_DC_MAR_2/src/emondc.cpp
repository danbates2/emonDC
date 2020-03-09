// emonDC extras, Dan Bates 2018-2020, free to use and modify.

// This firmware is set-up for for channel A to be connected to solar, and channel B to the battery,
// other configurations will work, however some of the printed data will be less relevant.
// Channels A and B are printed on the PCB.

#include "wifi.h"
#include "emonesp.h"
#include "input.h"
#include "config.h"
#include "emondc.h"

#include "AH_MCP320x.h"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// -------------------------------------------------------------------
// change the following variables for your system.
// -------------------------------------------------------------------
// NOTE: SEE THE JSON SETTINGS FILE FOR CHANGING THESE, THE VALUES INDICATED BELOW ARE A BACKUP.

unsigned int _MAIN_INTERVAL = 10; // seconds
unsigned int main_emondc_interval = _MAIN_INTERVAL * 1000;
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
float _CAL_FACTOR_icalA = 1.00; // CURRENT_A.
float _CAL_FACTOR_vcalA = 1.00; // VOLTAGE_A.
float _CAL_FACTOR_icalB = 1.00; // CURRENT_B.
float _CAL_FACTOR_vcalB = 1.00; // VOLTAGE_B.
// Amplifier offset corrections for zero measurements, ideally these would be maps derived from experiment.
float offset_correction_ampsA = 0.14;
float offset_correction_voltsA = -0.01;
float offset_correction_ampsB = 0.14;
float offset_correction_voltsB = -0.01;



// -------------------------------------------------------------------
// More advanced setting below.
// -------------------------------------------------------------------
// OLED Display
unsigned int oled_interval = 5000;
unsigned long oled_previousMillis;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET  -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int screentog;

const int chipSelectSD = 15; // SD card chip SPI chip select

AH_MCP320x ADC_SPI(2); // ADC SPI chip select. SPI speed set in library.

int _t_begin; //  human readable time of start
unsigned long _t;//  human readable time since start

// ADC reading
int CHANNEL1 = 0;
int CHANNEL2 = 0;
int CHANNEL3 = 0;
int CHANNEL4 = 0;
int CHANNEL5 = 0;
int CHANNEL6 = 0;
int CHANNEL7 = 0;
int CHANNEL8 = 0;
// Accumulators
unsigned long long CH_A_CURRENT_ACCUMULATOR = 0;
unsigned long long CH_A_VOLTAGE_ACCUMULATOR = 0;
unsigned long long CH_B_CURRENT_ACCUMULATOR = 0;
unsigned long long CH_B_VOLTAGE_ACCUMULATOR = 0;
unsigned long long VREF_UNI_ACCUMULATOR = 0;
unsigned long long VREF_BI_ACCUMULATOR = 0;
unsigned long long VREF33_ACCUMULATOR = 0;
unsigned long long CH8_ACCUMULATOR = 0;
unsigned long numberofsamples = 0;
// Averaged ADC channel values.
float CH_A_CURRENT_AVERAGED;
float CH_A_VOLTAGE_AVERAGED;
float CH_B_CURRENT_AVERAGED;
float CH_B_VOLTAGE_AVERAGED;
float VREF_UNI_AVERAGED;
float VREF_BI_AVERAGED;
float VREF33_AVERAGED;
float CH8_AVERAGED;
// Human-readable volts and amps
float Voltage_A;
float Current_A;
float Voltage_B;
float Current_B;
// Battery variables
double Ah_level_A = 0;
double Ah_level_B = 0;
double Ah_positive_A = 0;
double Ah_positive_B = 0;
double Ah_negative_A = 0;
double Ah_negative_B = 0;
// Wh variables
double Wh_chA_positive = 0.0;
double Wh_chB_positive = 0.0;
double Wh_chA_negative = 0.0;
double Wh_chB_negative = 0.0;
// Battery State of Charge variables [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
float battery_voltage_nominal = 12.0; // just in case useful later
float bat_max = 14.8; float bat_min = 10.6; // these actually change with temperature, and are therefore crude values.
float State_of_Charge; // percentage SoC, related to temperature, rate of discharge and Peukert coefficient.
float Temperature_Room; // future use
float Temperature_Bat; // future use
float peukert_exponent = 1.20; // can be derived from the 5h and 20h ratings of a battery/cell.
float rated_discharge_time = 20.0; // hours (Rt, or H)
float rated_capacity = 110.0; // amp-hours
// for converting the 3.3V reading into a useful calibration value, sets unit accuracy to the accuracy of the voltage regulator.
float volts_to_adc_reading_ratio;
// for storing the main string to be send to SD card and emonESP front-end and emonCMS.
String ADC_KeyValue_String = ""; 
// counting the number of posts.
int averaging_loop_counter = 0;
// future use.
bool low_power_mode = false;
unsigned int waking_time = 6000;
unsigned int sleeping_time = 10000;
// battery and voltage alarm levels for future development.
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
// calibrate solar at midnight?
bool midnight_calibration = false; 
// stopping the first post firing off too soon, before samples are gathered.
bool setup_done = 0;
// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long rtc_now_time = 0;
// time variables.
unsigned long currentMillis;
unsigned long previousMillis = 0;
//---------------
// NTP TIME SETTINGS
//---------------
//int time_offset = 3600; //GMT +0
int time_offset = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", time_offset, 300000);
unsigned long previousMillisNTP = 0;
unsigned long intervalNTP = 60000;
// SD CARD
String datalogFilename = "datalog.txt";
bool SD_present;


//-----------------------------------
// emonDC Setup
//-----------------------------------
void emondc_setup(void) {

  main_emondc_interval = _MAIN_INTERVAL * 1000; // note timeing this is called at, after JSON loading.

  // microSD card init
  pinMode(chipSelectSD, OUTPUT);
  digitalWrite(chipSelectSD, HIGH); delay(1);
  if (SD.begin(chipSelectSD, 400000)) { // SPI speed sey here. Clash possible with ADC.
    Serial.println("SDcard initialised.");
    SD_present = true;
  }
  else {
    Serial.println("SD card not detected.");
    SD_present = false;
  }

  // OLED Display init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  // Initial print, version number.
  display.println(F("emonDCduo"));
  display.println(F("HW:v3.6"));
  screentog = -1;
  display.display();
  //delay(1500);


  // RTC init
  if (!rtc.begin()) {
    Serial.println("RTC or I2C not functioning.");
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


  // Start NTP time 
  timeClient.setTimeOffset(time_offset);
  timeClient.begin();
}


//----------------- 
// emonDC main loop.
//-----------------
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
      
      NTP_update_PCF8523_update();

      //-----------------------------------
      // what to do with the ready data:
      //-----------------------------------
      forward_to_emonESP(); // sending to emonCMS
      if (SD_present) save_to_SDcard();
      clear_accumulators(); // this happens before gathering next samples, or else chaos ensues.
      // done.
      
      Serial.print("FreeRAM (bytes): ");  Serial.println(ESP.getFreeHeap()); // for debugging
    }
  }
  
  if (currentMillis - oled_previousMillis >= oled_interval) {
    oled_previousMillis = currentMillis;
    drawvalues_to_OLED(); // for the 128x32 I2C OLED.
  }
}


//-----------------
// Create averaged values from ADC channels, apply calibration, make readable amps and volts.
//-----------------
double Ah_A;
double Ah_B;
void average_and_calibrate(unsigned long pre_mills, unsigned long curr_mills) {
  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
  VREF_UNI_AVERAGED = VREF_UNI_ACCUMULATOR / numberofsamples;
  VREF_BI_AVERAGED = VREF_BI_ACCUMULATOR / numberofsamples;
  VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
  CH8_AVERAGED = CH8_ACCUMULATOR / numberofsamples;

 
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

  Current_A = make_readable_Amps(CH_A_CURRENT_AVERAGED, chanA, chanA_shuntAmp_gain) * _CAL_FACTOR_icalA;
  Voltage_A = make_readable_Volts(CH_A_VOLTAGE_AVERAGED, chanA) * _CAL_FACTOR_vcalA;
  Current_B = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB, chanB_shuntAmp_gain) * _CAL_FACTOR_icalB;
  Voltage_B = make_readable_Volts(CH_B_VOLTAGE_AVERAGED, chanB) * _CAL_FACTOR_vcalB;
  //CH8_AVERAGED = CH8_AVERAGED * _CAL_FACTOR_TEST;

  int this_interval = curr_mills - pre_mills;
  // amps in, amps out
  Ah_A = Ah_calculate(Current_A, this_interval / 1000);
  Ah_B = Ah_calculate(Current_B, this_interval / 1000);
  Ah_level_A += Ah_A;
  Ah_level_B += Ah_B;
  if (Ah_A > 0) Ah_positive_A += Ah_A;
  else Ah_negative_A += Ah_A;
  if (Ah_B > 0) Ah_positive_B += Ah_B;
  else Ah_negative_B += Ah_B;
  // -- - -- -
  Wh_chA_positive += Wh_accumulate(Current_A, Voltage_A, this_interval / 1000);
  Wh_chB_positive += Wh_accumulate(Current_B, Voltage_B, this_interval / 1000);
  
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


//-----------------
// Build Strign to forward to emonESP, same string is stored to SD card.
//-----------------
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
  ADC_KeyValue_String = ADC_KeyValue_String + "Ah_level_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + Ah_level_A;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Ah_level_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + Ah_level_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "samplecount:";
  ADC_KeyValue_String = ADC_KeyValue_String + numberofsamples;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "time:";
  ADC_KeyValue_String = ADC_KeyValue_String + rtc_now_time;
  input_string = ADC_KeyValue_String;
}


//-----------------
// Clear ADC accumulators for next round of samples.
//-----------------
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


//-----------------
// 128x32 I2C OLED
//-----------------
void drawvalues_to_OLED(void) { // draw to OLED
  if (screentog == -1) {
    screentog = 0;
    return;
  }
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
    display.println(F("Ah(A)")); display.println(Ah_level_A);
    screentog = 5;
  }
  else if (screentog == 5) {
    display.println(F("Ah(B)")); display.println(Ah_level_B);
    screentog = 6;
  }
  else if (screentog == 6) {
    display.println(F("SSID:")); display.println(esid);
    screentog = 7;
  }
  else if (screentog == 7) {
    display.println(F("IP Addr:")); display.println(ipaddress_OLED);
    //display.display(); /////
    screentog = -1;
  }

  display.display();
} // draw to OLED


//-----------------
// Update NTP time, set to RTC, read time from RTC.
//-----------------
void NTP_update_PCF8523_update(void) {
  if (WiFi.status() == WL_CONNECTED) {
    if (currentMillis - previousMillisNTP >= intervalNTP) {
      previousMillisNTP = currentMillis;
      timeClient.update();
      rtc.adjust(timeClient.getEpochTime());
    }
  }
  DateTime now = rtc.now();
  rtc_now_time = now.unixtime();
}


//-----------------
// Web server -> Save settings, intermediate function.
//-----------------
void config_save_emondc(unsigned int interval, float vcalA, float icalA, float vcalB, float icalB)
{
  _MAIN_INTERVAL = interval;
  main_emondc_interval = _MAIN_INTERVAL * 1000;
  _CAL_FACTOR_vcalA = vcalA;
  _CAL_FACTOR_icalA = icalA;
  _CAL_FACTOR_vcalB = vcalB;
  _CAL_FACTOR_icalB = icalB;
  
  Serial.println("emonDC saving settings...");
  Serial.println(_MAIN_INTERVAL);
  Serial.println(_CAL_FACTOR_vcalA);
  Serial.println(_CAL_FACTOR_icalA);
  Serial.println(_CAL_FACTOR_vcalB);
  Serial.println(_CAL_FACTOR_icalB);
  Serial.println("... done.");

  config_save_settings_spiffs(interval, vcalA, icalA, vcalB, icalB); 
}


//-----------------
// Wh accumulator calc.
//-----------------
double Wh_accumulate(float amps_value, float volts_value, int elapsed_seconds) {
  double watts = amps_value * volts_value;
  double watt_seconds = watts * (float)elapsed_seconds;
  double wh = watt_seconds / 3600;
  return wh;
}


//-----------------
// Wh accumulator calc.
//-----------------
double Ah_calculate(float amps_value, int elapsed_seconds) {
  double _Amp_hours = (amps_value * elapsed_seconds) / 3600.0;
  return _Amp_hours;
}


//-----------------
// TBD
//-----------------
float time_until_discharged(void) {
  //t = Rt(C/IRt)^k [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  float IRt = Current_B * rated_discharge_time; // for channel B is connected to battery.
  float C_IRt = rated_capacity / IRt;
  float Rt_C_IRt = rated_discharge_time * C_IRt;
  float _time_until_discharged = pow(Rt_C_IRt, 2.0);
  return _time_until_discharged;
}
//-----------------
// TBD
//-----------------
float effective_capacity(void) {
  // It = C (C / I*H)^k-1 [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  float _IH = Current_B * rated_discharge_time; // for channel B is connected to battery.
  float _C_IH = rated_capacity / _IH;
  float _C_C_IH = rated_capacity * _C_IH;
  float _k_neg_one = 1 / peukert_exponent;
  float _result_It = pow(_C_C_IH, _k_neg_one);
  return _result_It;
}



float volts_to_adc_reading_ratio_function(void) {
  float _volts_to_adc_reading_ratio = 3.3000 / VREF33_AVERAGED;
  return (_volts_to_adc_reading_ratio);
}


float Vcal_coefficient_A_pre = (R1_A + R2_A); // resistor divider calc.
float Vcal_coefficient_A = R2_A / Vcal_coefficient_A_pre;
float Vcal_coefficient_B_pre = (R1_B + R2_B); // resistor divider calc.
float Vcal_coefficient_B = R2_B / Vcal_coefficient_B_pre;

// adc value to Volts function
float make_readable_Volts (float _adcValue, String _chan) {
  float _readableV1 = _adcValue * volts_to_adc_reading_ratio;
  if (_chan == chanA) {
    float _readableVolts = _readableV1 / Vcal_coefficient_A;
    _readableVolts += offset_correction_voltsA;
    return (_readableVolts);
  }
  else {
    float _readableVolts = _readableV1 / Vcal_coefficient_B;
    _readableVolts += offset_correction_voltsB;
    return (_readableVolts);
  }
}

// adc value to Amps function
float make_readable_Amps (float _adcValue, String _chan, float _gain)
{
  float _readable_1 = _adcValue * volts_to_adc_reading_ratio;
  float _readable_2 = _readable_1 / _gain; // gives mV signal value.

  if (_chan == chanA) {
    float _readableAmps = _readable_2 / Rshunt_A; // I=V/R
    _readableAmps += offset_correction_ampsA; // shunt monitor offset calibration
    return (_readableAmps);
  }
  else {
    float _readableAmps = _readable_2 / Rshunt_B; // I=V/R
    _readableAmps += offset_correction_ampsB; // shunt monitor offset calibration
    return (_readableAmps);
  }
}

//---------------
// SD CARD
//---------------
void save_to_SDcard(void) {
  File dataFile = SD.open(datalogFilename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(ADC_KeyValue_String);
    dataFile.close();
    // print to the serial port too:
    Serial.println("Save to SD card.");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}