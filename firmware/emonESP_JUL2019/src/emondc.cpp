// emonDC extras, Daniel Bates 2019.
// Free to use and modify.
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
  RFM69_enabled
  chanAref
  chanBref
  R1_A
  R2_A
  R1_B
  R2_B
  Rshunt_A
  Rshunt_B
  CAL0
  CAL1
  CAL2
  CAL3
  CAL7
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
*/

#include "emondc.h"
#include "input.h"
#include "config.h"
#include "wifi.h"

#include <SD.h>

unsigned long main_emondc_interval = 5; // seconds

String datalogFilename = "datalog2.txt";
sd::File datalogFile;
 
bool RFM69_enabled = false; // RFM69Pi compatibility.

bool chanAref = 0; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
bool chanBref = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.

// values of resistor divider for voltage reading.
double R1_A = 1000000.0; // 1Mohm default
double R2_A = 75000.0; // 75kohm default
double R1_B = 1000000.0;
double R2_B = 75000.0;
double Rshunt_A = 0.0005; // value of Rsense in ohms
double Rshunt_B = 0.0005;

// Calibration values for manual adjustment.
double CAL0 = 1.0; // CURRENT_A
double CAL1 = 1.0; // VOLTAGE_A
double CAL2 = 1.0; // CURRENT_B
double CAL3 = 1.0; // VOLTAGE_B
double CAL7 = 1.0; // ADC_CHANNEL_7

// -------------------------------------------------------------------
// 'ADVANCED' STUFF BELOW HERE
// -------------------------------------------------------------------

double CH_A_CURRENT_AVERAGED;
double CH_A_VOLTAGE_AVERAGED;
double CH_B_CURRENT_AVERAGED;
double CH_B_VOLTAGE_AVERAGED;
double VREF_UNI_AVERAGED;
double VREF_BI_AVERAGED;
double VREF33_AVERAGED;
double CH7_AVERAGED;

double Voltage_A;
double Current_A;
double Voltage_B;
double Current_B;

int battery_voltage_nominal = 12;
float State_of_ChargeB; // percentage SoC, related to temperature, rate of discharge and peukert coefficient.
float Temperature_Room;
float Temperature_Bat;
float peukert_exponent = 1.20; // can be derived from the 5h and 20h ratings of a battery.
float rated_discharge_time = 20.0; // hours (Rt, or H)
float rated_capacity = 100.0; // amp-hours (C)

double time_until_discharged(void) {
  //t = Rt(C/IRt)^k [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  double IRt = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB) * rated_discharge_time;
  double C_IRt = rated_capacity / IRt;
  double Rt_C_IRt = rated_discharge_time * C_IRt;
  double _time_until_discharged = pow(Rt_C_IRt, 2.0);
  return _time_until_discharged;
}

double effective_capacity(void) {
  // It = C (C / I*H)^k-1 [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
  double _IH = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB) * rated_discharge_time;
  double _C_IH = rated_capacity / _IH;
  double _C_C_IH = rated_capacity * _C_IH;
  double _k_neg_one = 1 / peukert_exponent;
  double _result_It = pow(_C_C_IH, _k_neg_one);
  return _result_It;
}

double volts_to_adc_reading_ratio;
double volts_to_adc_reading_ratio_function(void) {
  double _volts_to_adc_reading_ratio = 3.3000 / VREF33_AVERAGED;
  return (_volts_to_adc_reading_ratio);
}


double Vcal_coefficient_A_pre = (R1_A + R2_A); // resistor divider calc.
double Vcal_coefficient_A = R2_A / Vcal_coefficient_A_pre;
double Vcal_coefficient_B_pre = (R1_B + R2_B); // resistor divider calc.
double Vcal_coefficient_B = R2_B / Vcal_coefficient_B_pre;

// adc value to Volts function:
double make_readable_Volts (double adcValuePassed, String chan) {
  double readableV1 = adcValuePassed * volts_to_adc_reading_ratio;
  if (chan == chanA) {
    double readableV2 = readableV1 / Vcal_coefficient_A;
    return (readableV2);
  }
  else if (chan == chanB) {
    double readableV2 = readableV1 / Vcal_coefficient_B;
    return (readableV2);
  }
}

// adc value to Amps function:
double make_readable_Amps (double adcValuePassed, String chan) {
  double readableC1 = adcValuePassed * volts_to_adc_reading_ratio;
  double readableC2 = readableC1 / 100.0; // gives mV signal value.
  if (chan == chanA) {
    double readableC3 = readableC2 / Rshunt_A;
    return (readableC3);
  }
  else if (chan == chanB) {
    double readableC3 = readableC2 / Rshunt_B;
    return (readableC3);
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

unsigned int ADC_DelayTest = 2; // microsecond delay in ADC routine, to improve accuracy, related to the Sample and Hold architecture of the ADC.

// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup
#include <Wire.h>
#include "RTClib.h"
RTC_PCF8523 rtc;

// OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int screentog = 0;

const int chipSelectSD = 15; //SD card

// MCP3208
#define CHIP_SELECT_MCP    2 // CS
#define DATAOUT_MCP3208    13 // MOSI
#define DATAIN_MCP3208     12 // MISO
#define SPICLOCK          14 // CLK

unsigned long previousMillis = 0;
int CH_A_CURRENT = 0;
int CH_A_VOLTAGE = 0;
int CH_B_CURRENT = 0;
int CH_B_VOLTAGE = 0;
int VREF_UNI = 0;
int VREF_BI = 0;
int VREF33 = 0;
int CH6 = 0;
int CH7 = 0;
unsigned long CH_A_CURRENT_ACCUMULATOR = 0;
unsigned long CH_A_VOLTAGE_ACCUMULATOR = 0;
unsigned long CH_B_CURRENT_ACCUMULATOR = 0;
unsigned long CH_B_VOLTAGE_ACCUMULATOR = 0;
unsigned long VREF_UNI_ACCUMULATOR = 0;
unsigned long VREF_BI_ACCUMULATOR = 0;
unsigned long VREF33_ACCUMULATOR = 0;
unsigned long CH7_ACCUMULATOR = 0;

unsigned long numberofsamples = 0;

String ADC_KeyValue_String = "";

unsigned long currentMillis;

bool initSD() {
  pinMode(chipSelectSD, OUTPUT);
  digitalWrite(chipSelectSD, HIGH);
  if (SD.begin(chipSelectSD)) {
    Serial.println("SDcard initialised");
    return true;
  } else
    Serial.println("SD Card Not Found!");
  return false;
}

void emondc_setup(void) {
  //SD card
  initSD();
  /*
    //Initialize SD card.
    pinMode(chipSelectSD, OUTPUT);
    digitalWrite(chipSelectSD, HIGH);
    Serial.print("Initializing SD card...");
    if (!SD.begin(chipSelectSD)) {
    Serial.println("Card failed, or not present");
    }
    else {
    Serial.println("card initialized.");
    }
  */

  //MCP3208
  //set pin modes
  pinMode(CHIP_SELECT_MCP, OUTPUT);
  pinMode(DATAOUT_MCP3208, OUTPUT);
  pinMode(DATAIN_MCP3208, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  //disable device to start with
  digitalWrite(CHIP_SELECT_MCP, HIGH);
  digitalWrite(DATAOUT_MCP3208, LOW);
  digitalWrite(SPICLOCK, LOW);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    // for (;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.clearDisplay();
  //delay(2000); // Pause for 2 seconds

  delay(50);

  rtc.begin();
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  delay(10);

}


void emondc_loop(void) {

  currentMillis = millis();

  CH_A_CURRENT = read_adc(1);
  CH_A_VOLTAGE = read_adc(2);
  CH_B_CURRENT = read_adc(3);
  CH_B_VOLTAGE = read_adc(4);
  VREF_UNI = read_adc(6);
  VREF_BI = read_adc(5);
  VREF33 = read_adc(7);
  CH7 = read_adc(8); // Labeled on the physical board as CH7, the channels on the ADC can be described as being from 0-7, or 1-8.

  CH_A_CURRENT_ACCUMULATOR = CH_A_CURRENT_ACCUMULATOR + CH_A_CURRENT;
  CH_A_VOLTAGE_ACCUMULATOR = CH_A_VOLTAGE_ACCUMULATOR + CH_A_VOLTAGE;
  CH_B_CURRENT_ACCUMULATOR = CH_B_CURRENT_ACCUMULATOR + CH_B_CURRENT;
  CH_B_VOLTAGE_ACCUMULATOR = CH_B_VOLTAGE_ACCUMULATOR + CH_B_VOLTAGE;
  VREF_UNI_ACCUMULATOR = VREF_UNI_ACCUMULATOR + VREF_UNI;
  VREF_BI_ACCUMULATOR = VREF_BI_ACCUMULATOR + VREF_BI;
  VREF33_ACCUMULATOR = VREF33_ACCUMULATOR + VREF33;
  CH7_ACCUMULATOR = CH7_ACCUMULATOR + CH7;
  numberofsamples++;

  if (currentMillis - previousMillis >= main_emondc_interval * 1000) {
    //int overrunMillis = currentMillis % main_emondc_interval;
    previousMillis = currentMillis;
    //previousMillis = previousMillis - overrunMillis;

    average_and_calibrate();

    forward_to_emonESP();

    save_to_SDcard();

    // print_readable();

    // forward_to_RFM69Pi();

    clear_accumulators();
  }
}


void average_and_calibrate(void) {
  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
  VREF_UNI_AVERAGED = VREF_UNI_ACCUMULATOR / numberofsamples;
  VREF_BI_AVERAGED = VREF_BI_ACCUMULATOR / numberofsamples;
  VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
  CH7_AVERAGED = CH7_ACCUMULATOR / numberofsamples;

  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_AVERAGED * CAL0;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_AVERAGED * CAL1;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_AVERAGED * CAL2;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_AVERAGED * CAL3;
  CH7_AVERAGED = CH7_AVERAGED * CAL7;


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

  Current_A = make_readable_Amps(CH_A_CURRENT_AVERAGED, chanA) * CAL0;
  Voltage_A = make_readable_Volts(CH_A_VOLTAGE_AVERAGED, chanA) * CAL1;
  Current_B = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB) * CAL2;
  Voltage_B = make_readable_Volts(CH_B_VOLTAGE_AVERAGED, chanB) * CAL3;
  CH7_AVERAGED = CH7_AVERAGED * CAL7;

  averaging_loop_counter++;

#ifndef RELEASE
  Serial.print("Number of samples: ");
  Serial.println(numberofsamples);
  Serial.print("Averaging loop counter: ");
  Serial.println(averaging_loop_counter);
  Serial.print("Micros: ");
  Serial.println(micros());
#endif

} // Averaging and print loop end.


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
  Serial.print("CH7_ADC_AVERAGED: ");
  Serial.println(CH7_AVERAGED);
}

void forward_to_emonESP(void)
{
  ADC_KeyValue_String = "Volts_A:" + String(Voltage_A, 2);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_A;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Volts_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Voltage_A;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Uni:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_UNI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Bi:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_BI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "3.3V_Ref:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF33_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "CH7:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH7_AVERAGED;

  input_string = ADC_KeyValue_String;
}


void forward_to_RFM69Pi(void) {
  // ADC_KeyValue_String = "Volts_A:" + String(make_readable_Volts(CH_A_VOLTAGE_AVERAGED, chanA), 2);
  ADC_KeyValue_String = String(Voltage_A, 2);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "Amps_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_A;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "Volts_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Voltage_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "Amps_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + Current_B;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Uni:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_UNI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Bi:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF_BI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "3.3V_Ref:";
  ADC_KeyValue_String = ADC_KeyValue_String + (VREF33_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  //ADC_KeyValue_String = ADC_KeyValue_String + "CH7:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH7_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + "s";
  Serial.println(ADC_KeyValue_String);
}


void clear_accumulators(void) {
  CH_A_CURRENT_ACCUMULATOR = 0;
  CH_A_VOLTAGE_ACCUMULATOR = 0;
  CH_B_CURRENT_ACCUMULATOR = 0;
  CH_B_VOLTAGE_ACCUMULATOR = 0;
  VREF_UNI_ACCUMULATOR = 0;
  VREF_BI_ACCUMULATOR = 0;
  VREF33_ACCUMULATOR = 0;
  CH7_ACCUMULATOR = 0;
  numberofsamples = 0;
}

void save_to_SDcard(void) {
  datalogFile = SD.open(datalogFilename, FILE_WRITE);
  datalogFile.close();
  datalogFile = SD.open(datalogFilename, FILE_WRITE);
  if (datalogFile) {
    datalogFile.println(ADC_KeyValue_String);
    datalogFile.close();
    // print to the serial port too:
    Serial.println("saved to SD card.");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening ");
    Serial.println(datalogFilename);
    //datalogFile = SD.open(datalogFilename, FILE_WRITE);
  }
}


// Read ADC function for MCP3208
int read_adc(int channel) {
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
  //B10100000
  //allow channel selection
  commandbits |= ((channel - 1) << 3);

  digitalWrite(CHIP_SELECT_MCP, LOW); //Select adc
  // 'setup bits' to be sent out
  for (int i = 7; i >= 3; i--) {
    digitalWrite(DATAOUT_MCP3208, commandbits & 1 << i);
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }

  delayMicroseconds(ADC_DelayTest);
  digitalWrite(SPICLOCK, HIGH);   //ignores 2 null bits
  digitalWrite(SPICLOCK, LOW);
  digitalWrite(SPICLOCK, HIGH);
  digitalWrite(SPICLOCK, LOW);

  // 'read bits' from adc
  for (int i = 11; i >= 0; i--) {
    adcvalue += digitalRead(DATAIN_MCP3208) << i;
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }
  digitalWrite(CHIP_SELECT_MCP, HIGH); //turn off device
  return adcvalue;
} // Read ADC function for MCP3208


void drawvalues_to_OLED(void) { // draw to OLED
  DateTime now = rtc.now();

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
    display.println(F("SSID:")); display.println(esid);
    screentog = 5;
  }
  else if (screentog == 5) {
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

    screentog = 6;
  }
  else if (screentog == 6) {
    display.println(F("IP Addr:")); display.println(ipaddress);
    //display.display(); /////
    screentog = 0;
  }
  display.display();
} // draw to OLED


void config_save_emondc(unsigned long  interval)
{
  main_emondc_interval = interval;

  /*
    EEPROM_write_string(EEPROM_WWW_USER_START, EEPROM_WWW_USER_SIZE, user);
    EEPROM_write_string(EEPROM_WWW_PASS_START, EEPROM_WWW_PASS_SIZE, pass);

    EEPROM.commit();
  */

}
