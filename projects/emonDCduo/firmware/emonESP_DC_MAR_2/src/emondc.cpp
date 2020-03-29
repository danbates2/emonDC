// emonDC extras, Dan Bates 2018-2020, free to use and modify.

// This firmware is set-up for for channel A to be connected to solar, and channel B to the battery,
// other configurations will work, however some of the printed data will be less relevant.
// Channels A and B are printed on the PCB.

#include "wifi.h"
#include "emonesp.h"
#include "input.h"
#include "config.h"
#include "emondc.h"
#include "gpio0.h"
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

String hw_version = "HW:v3.6";

// -------------------------------------------------------------------
// change the following variables for your system.
// -------------------------------------------------------------------
// NOTE: SEE THE JSON SETTINGS FILE FOR CHANGING THESE, THE VALUES INDICATED BELOW ARE A BACKUP.
unsigned int main_interval_seconds = 5; // !!! set this in the JSON config file !!! max is 1800 seconds (30 minutes).
unsigned long main_interval_ms= main_interval_seconds * 1000;
bool chanAref = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
bool chanBref = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
double chanA_shuntAmp_gain = 100; // shunt amp gain.
double chanB_shuntAmp_gain = 100;
// values of resistor divider for voltage reading.
double R1_A = 1000000.0; // 1Mohm default.
double R2_A = 75000.0; // 75kohm default.
double R1_B = 1000000.0;
double R2_B = 75000.0;
double Rshunt_A = 0.000500; // value of Rsense in Ohms.
double Rshunt_B = 0.000500;
// Calibration values for manual adjustment.
double _CAL_FACTOR_icalA = 1.00; // CURRENT_A.
double _CAL_FACTOR_vcalA = 1.00; // VOLTAGE_A.
double _CAL_FACTOR_icalB = 1.00; // CURRENT_B.
double _CAL_FACTOR_vcalB = 1.00; // VOLTAGE_B.
// Amplifier offset corrections for zero measurements, ideally these would be maps derived from experiment.
double offset_correction_ampsA = 0.14;
double offset_correction_voltsA = -0.01;
double offset_correction_ampsB = 0.14;
double offset_correction_voltsB = -0.01;



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
int screentog = -1;

const int chipSelectSD = 15; // SD card chip SPI chip select
AH_MCP320x ADC_SPI(2); // ADC SPI chip select. SPI speed set in library.
// sample rate in this sketch is around 450 samples per channel per second.

unsigned int _t_begin; //  human readable time of start
unsigned long _t;//  human readable time since start

// Accumulators
unsigned long CH_A_CURRENT_ACCUMULATOR = 0;
unsigned long CH_A_VOLTAGE_ACCUMULATOR = 0;
unsigned long CH_B_CURRENT_ACCUMULATOR = 0;
unsigned long CH_B_VOLTAGE_ACCUMULATOR = 0;
unsigned long VREF_UNI_ACCUMULATOR = 0;
unsigned long VREF_BI_ACCUMULATOR = 0;
unsigned long VREF33_ACCUMULATOR = 0;
unsigned long CH8_ACCUMULATOR = 0;
unsigned long numberofsamples = 0;
// Averaged ADC channel values.
double CH_A_CURRENT_AVERAGED;
double CH_A_VOLTAGE_AVERAGED;
double CH_B_CURRENT_AVERAGED;
double CH_B_VOLTAGE_AVERAGED;
double VREF_UNI_AVERAGED;
double VREF_BI_AVERAGED;
double VREF33_AVERAGED;
double CH8_AVERAGED;
// Human-readable volts and amps
double Voltage_A;
double Current_A;
double Voltage_B;
double Current_B;
//------------------------------
// Battery variables
//------------------------------
double Ah_level_A = 0.0;
double Ah_level_B = 0.0;
double Ah_positive_A = 0.0;
double Ah_positive_B = 0.0;
double Ah_negative_A = 0.0;
double Ah_negative_B = 0.0;
// Battery State of Charge variables [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
double battery_voltage_nominal = 12.0; // just in case useful later
double bat_max = 14.8; double bat_min = 10.6; // these actually change with temperature, and are therefore crude values.
double state_of_charge = 1.00 ; // 1 = 100% percent
double _time_until_discharged;
// battery data.
double Hr = 20.0; // rated time in hours
double k = 1.25; // Peukert exponent for flooded lead-acid (see link above)
double C = 100.0; // capacity of battery at rated time (Ah)
double Temperature_Room = 16.0; // future use
double Temperature_Bat = 10.0; // future use
// Wh variables
double Wh_chA_positive = 0.0;
double Wh_chB_positive = 0.0;
double Wh_chA_negative = 0.0;
double Wh_chB_negative = 0.0;

// for converting the 3.3V reading into a useful calibration value, sets unit accuracy to the accuracy of the voltage regulator.
double volts_to_adc_reading_ratio;
// for storing the main string to be send to SD card and emonESP front-end and emonCMS.
String ADC_KeyValue_String = ""; 
// counting the number of posts.
int averaging_loop_counter = 0;
// future use.
bool low_power_mode = false;
unsigned int waking_time = 6000;
unsigned int sleeping_time = 10000;
// battery and voltage alarm levels for future development.
double Valarm_A_HIGH;
double Valarm_A_LOW;
double Valarm_A_VERY_LOW;
double Valarm_B_HIGH;
double Valarm_B_LOW;
double Valarm_B_VERY_LOW;
double Calarm_A_HIGH;
double Calarm_B_HIGH;
double TempAlarmBatHIGH;
double TempAlarmBatLOW;
// calibrate solar at midnight?
bool midnight_calibration = false; 
// stopping the first post firing off too soon, before samples are gathered.
bool setup_done = 0;
// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long rtc_now_time = 0;
bool rtc_accurate;
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

  main_interval_ms= main_interval_seconds * 1000; // called after JSON is loaded.

  // microSD card init
  pinMode(chipSelectSD, OUTPUT);
  digitalWrite(chipSelectSD, HIGH); delay(1);
  SPISettings mySetting(400000, MSBFIRST, SPI_MODE0);
  if (SD.begin(chipSelectSD, 400000)) { // SPI speed sey here. Clash possible with ADC.
  //if (SD.begin(chipSelectSD, mySetting)) { // SPI speed sey here. Clash possible with ADC.
    Serial.println("SDcard initialised.");
    SD_present = true;
  }
  else {
    Serial.println("SD card not detected.");
    SD_present = false;
  }

  // OLED Display init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("SSD1306 allocation failed."); // Address 0x3C for 128x32
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.println(F("emonDCduo")); // Initial print to OLED, version number.
  display.println(hw_version);
  display.display();
  
  // RTC init
  if (!rtc.begin()) { Serial.println("RTC or I2C not functioning."); rtc_accurate = false; }
  else if (!rtc.initialized()) {
    Serial.println("RTC not running,\r\n  - attempting initialisation with compile time.");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    rtc_accurate = false;
  }
  else {
    Serial.println("RTC initialised.");
    rtc_accurate = true;
  }

  // Start NTP time 
  timeClient.setTimeOffset(time_offset);
  timeClient.begin();

  gpio0_setup();
} // end emonDC setup.


//--------------------------------------------------
// emonDC main loop.
//--------------------------------------------------
void emondc_loop(void) {
  currentMillis = millis();

  CH_A_CURRENT_ACCUMULATOR += ADC_SPI.readCH(0);
  CH_A_VOLTAGE_ACCUMULATOR += ADC_SPI.readCH(1);
  CH_B_CURRENT_ACCUMULATOR += ADC_SPI.readCH(2);
  CH_B_VOLTAGE_ACCUMULATOR += ADC_SPI.readCH(3);
  VREF_UNI_ACCUMULATOR     += ADC_SPI.readCH(4);
  VREF_BI_ACCUMULATOR      += ADC_SPI.readCH(5);
  VREF33_ACCUMULATOR       += ADC_SPI.readCH(6);
  CH8_ACCUMULATOR          += ADC_SPI.readCH(7);
  
  numberofsamples++;

  yield();

  if (currentMillis - previousMillis >= main_interval_ms) {
    //overrunMillis = currentMillis % main_interval_ms;
    previousMillis = currentMillis;
    //previousMillis = previousMillis - overrunMillis;
    NTP_update_PCF8523_update();  // update RTC time, via network if available every 60s.   

    if (numberofsamples >= 500) { // making sure we've got some samples to average.
      yield();
      average_and_calibrate(previousMillis, currentMillis); // readying the readable values, passing necessary time values associated with the posting intervals.
      yield();
      //-----------------------------------
      // what to do with the ready data:
      //-----------------------------------
      forward_to_emonESP(); // sending to emonCMS
      yield();
      if (SD_present) save_to_SDcard(); // save to SD card.
      yield();
      //Serial.println(time_until_discharged()); // testing Peukert calculation
      //Serial.println(effective_capacity()); // testing Peukert calculation
      //-----------------------------------
      clear_accumulators(); // this happens before gathering next samples, or else chaos ensues.
      yield();
      //-----------------------------------
      Serial.print("FreeRAM (bytes): ");  Serial.println(ESP.getFreeHeap()); // for debugging
    }
  }
  
  if (currentMillis - oled_previousMillis >= oled_interval) {
    oled_previousMillis = currentMillis;
    draw_OLED(); // for the 128x32 I2C OLED.
    yield();
  }
} // end emonDC loop


//----------------------------------------------------------------------------------------------------
// Create averaged values from ADC channels, apply calibration, make readable amps and volts.
//----------------------------------------------------------------------------------------------------
void average_and_calibrate(unsigned long pre_mills, unsigned long curr_mills) {
  // create averaged values from the sample accumulators.
  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
  VREF_UNI_AVERAGED = VREF_UNI_ACCUMULATOR / numberofsamples;
  VREF_BI_AVERAGED = VREF_BI_ACCUMULATOR / numberofsamples;
  VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
  CH8_AVERAGED = CH8_ACCUMULATOR / numberofsamples;

  // adjust current readings according to Vref values, i.e. reference offset removal.
  if (chanAref == chanRef) CH_A_CURRENT_AVERAGED -= VREF_UNI_AVERAGED;
  else CH_A_CURRENT_AVERAGED -= VREF_BI_AVERAGED;
  if (chanBref == chanRef) CH_B_CURRENT_AVERAGED -= VREF_UNI_AVERAGED;
  else CH_B_CURRENT_AVERAGED -= VREF_BI_AVERAGED;

  volts_to_adc_reading_ratio = volts_to_adc_reading_ratio_function(); // convert VREF33 into 'ADCvalue to Voltage factor'
  yield();
  // make readable voltage and current values, apply calibration.
  Current_A = make_readable_Amps(CH_A_CURRENT_AVERAGED, chanA, chanA_shuntAmp_gain) * _CAL_FACTOR_icalA;
  Voltage_A = make_readable_Volts(CH_A_VOLTAGE_AVERAGED, chanA) * _CAL_FACTOR_vcalA;
  Current_B = make_readable_Amps(CH_B_CURRENT_AVERAGED, chanB, chanB_shuntAmp_gain) * _CAL_FACTOR_icalB;
  Voltage_B = make_readable_Volts(CH_B_VOLTAGE_AVERAGED, chanB) * _CAL_FACTOR_vcalB;

  unsigned long this_interval_ms = curr_mills - pre_mills;
  yield();
  double Ah_period = (Current_B * (this_interval_ms/1000)) / 3600.0; // Coulomb counting.
  double soc_diff = Ah_period / effective_capacity_fromfull(); // state of charge difference this period.
  state_of_charge -= soc_diff; // update state of charge.
  // C *= state_of_charge; don't adjust this on the fly because of it's effect of peukert equation.
  _time_until_discharged = time_until_discharged_fromfull() * 3600  * state_of_charge;
  yield();
  
  
  
  /*
  // calculate amp hours and watt hours.
  
  //double Ah_A = Ah_calculate(Current_A, ((double)this_interval / 1000.0));
  //double Ah_B = Ah_calculate(Current_B, ((double)this_interval / 1000.0));
  Ah_level_A += Ah_A;
  Ah_level_B += Ah_B;
  if (Ah_A > 0) Ah_positive_A += Ah_A;
  else Ah_negative_A += Ah_A;
  if (Ah_B > 0) Ah_positive_B += Ah_B;
  else Ah_negative_B += Ah_B;
  
  Serial.println("Ah_positive_A");
  Serial.println(Ah_positive_A);
  Serial.println("Ah_negative_A");
  Serial.println(Ah_negative_A);
  Serial.println("Ah_positive_B");
  Serial.println(Ah_positive_B);
  Serial.println("Ah_negative_B");
  Serial.println(Ah_negative_B);
  */
  // -- - -- -
  //Wh_chA_positive += Wh_accumulate(Current_A, Voltage_A, this_interval / 1000);
  //Wh_chB_positive += Wh_accumulate(Current_B, Voltage_B, this_interval / 1000);
  /*
  Serial.println("Wh_chA_positive");
  Serial.println(Wh_chA_positive);
  Serial.println("Wh_chB_positive");
  Serial.println(Wh_chB_positive);
  */


  if (setup_done == false) {
    _t_begin = millis() / 1000;
    setup_done = true;
  }
  int _t2 = millis() / 1000;
  _t = _t2 - _t_begin;

  averaging_loop_counter++;

  
  Serial.print("Averaging loop counter: ");
  Serial.print(averaging_loop_counter);
  Serial.print("        _t: ");
  Serial.println(_t);
  Serial.print("connected_network: ");
  Serial.println(connected_network);
  
} // Averaging loop end.


//---------------------------------------------------------------------------
// Build Strign to forward to emonESP, same string is stored to SD card.
//---------------------------------------------------------------------------
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
  ADC_KeyValue_String = ADC_KeyValue_String + "SoC(%):";
  ADC_KeyValue_String = ADC_KeyValue_String + (state_of_charge*100);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "TimeUD(s):";
  ADC_KeyValue_String = ADC_KeyValue_String + _time_until_discharged;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "samplecount:";
  ADC_KeyValue_String = ADC_KeyValue_String + numberofsamples;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "time:";
  ADC_KeyValue_String = ADC_KeyValue_String + rtc_now_time;
  input_string = ADC_KeyValue_String;
}


//-------------------------
// Clear ADC accumulators for next round of samples.
//-------------------------
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


//-------------------------
// 128x32 I2C OLED
//-------------------------
void draw_OLED(void) { // draw to OLED
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
    display.println(F("SoC(%)")); display.println(state_of_charge*100);
    screentog = 5;
  }
  else if (screentog == 5) {
    display.println(F("TimeUD(h)")); display.println(_time_until_discharged/3600);
    screentog = 8;
  }
  /*else if (screentog == 6) {
    display.println(F("effCap:")); display.println(effective_capacity_fromfull());
    screentog = 7;
  }
  else if (screentog == 7) {
    display.println(F("TUD:")); display.println(time_until_discharged_fromfull());
    screentog = 8;
  }*/
  else if (screentog == 8) {
    display.println(F("SSID:")); display.println(connected_network);
    screentog = 9;
  }
  else if (screentog == 9) {
    display.println(F("IP Addr:")); display.println(ipaddress_OLED);
    screentog = 0; // return to zero
  }

  display.display(); // refresh display with buffer contents.
}


//-------------------------
// Update NTP time, set to RTC, read time from RTC.
//-------------------------
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


//-------------------------
// Web server -> Save settings, intermediate function.
//-------------------------
void config_save_emondc(unsigned int interval, double vcalA, double icalA, double vcalB, double icalB)
{
  // update values for emondc_loop
  main_interval_ms = interval * 1000; 
  _CAL_FACTOR_vcalA = vcalA;
  _CAL_FACTOR_icalA = icalA;
  _CAL_FACTOR_vcalB = vcalB;
  _CAL_FACTOR_icalB = icalB;
  
  Serial.println("emonDC saving settings...");
  Serial.println(main_interval_ms);
  Serial.println(_CAL_FACTOR_vcalA);
  Serial.println(_CAL_FACTOR_icalA);
  Serial.println(_CAL_FACTOR_vcalB);
  Serial.println(_CAL_FACTOR_icalB);
  Serial.println("... done.");

  config_save_settings_spiffs(interval, vcalA, icalA, vcalB, icalB); 
}



//--------------------------------------------------
// ADC averaged 3.3V reading to volts per 12-bit ADC division
//--------------------------------------------------
double volts_to_adc_reading_ratio_function(void) {
  double _volts_to_adc_reading_ratio = 3.3000 / VREF33_AVERAGED;
  return (_volts_to_adc_reading_ratio);
}



//--------------------------------------------------
// ADC averaged value to Volts
//--------------------------------------------------
double Vcal_coefficient_A_pre = (R1_A + R2_A); // resistor divider calc.
double Vcal_coefficient_A = R2_A / Vcal_coefficient_A_pre;
double Vcal_coefficient_B_pre = (R1_B + R2_B); // resistor divider calc.
double Vcal_coefficient_B = R2_B / Vcal_coefficient_B_pre;

double make_readable_Volts (double _Value, String _chan) {
  double _readableV1 = _Value * volts_to_adc_reading_ratio;
  if (_chan == chanA) {
    double _readableVolts = _readableV1 / Vcal_coefficient_A;
    _readableVolts += offset_correction_voltsA;
    return (_readableVolts);
  }
  else {
    double _readableVolts = _readableV1 / Vcal_coefficient_B;
    _readableVolts += offset_correction_voltsB;
    return (_readableVolts);
  }
}



//--------------------------------------------------
// ADC averaged value to Amps
//--------------------------------------------------
double make_readable_Amps (double _Value, String _chan, double _gain)
{
  double _readable_1 = _Value * volts_to_adc_reading_ratio;
  double _readable_2 = _readable_1 / _gain; // gives mV signal value.

  if (_chan == chanA) {
    double _readableAmps = _readable_2 / Rshunt_A; // I=V/R
    _readableAmps += offset_correction_ampsA; // shunt monitor offset calibration
    return (_readableAmps);
  }
  else {
    double _readableAmps = _readable_2 / Rshunt_B; // I=V/R
    _readableAmps += offset_correction_ampsB; // shunt monitor offset calibration
    return (_readableAmps);
  }
}



//-------------------------
// SD CARD
//-------------------------
void save_to_SDcard(void) {
  File dataFile = SD.open(datalogFilename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(ADC_KeyValue_String);
    dataFile.close();
    // print to the serial port too:
    Serial.println("SD card save OK");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}


//-------------------------
// Wh accumulator calc.
//-------------------------
double Wh_accumulate(double amps_value, double volts_value, int elapsed_seconds) {
  double watts = amps_value * volts_value;
  double watt_seconds = watts * (double)elapsed_seconds;
  double wh = watt_seconds / 3600;
  return wh;
}


//-------------------------
// Battery monitoring.
//-------------------------
double time_until_discharged_fromfull(void) { // returns time until discharged from ideal battery condition.
  // http://www.smartgauge.co.uk/peukert2.html
  // T = (Hr*(C/Hr)^n)/(I^n)
  double Peukert_capacity = Hr * pow((C/Hr), k);
  bool flip = 0;
  if (Current_B < 0) { flip = true; Current_B = fabs(Current_B); }
  double _time_until_discharged = Peukert_capacity / (pow(Current_B, k));
  //sprintf(log_buffer, "_time_until_discharged(seconds):%.3f\r\n", _time_until_discharged*3600);
  //printf("%s",log_buffer);
  if (flip) { _time_until_discharged = -_time_until_discharged; Current_B = -Current_B; }
  return _time_until_discharged;
}

double effective_capacity_fromfull(void) { // returns eff. cap. in Ah.
  // http://www.smartgauge.co.uk/technical1.html
  // http://www.smartgauge.co.uk/peukert3.html
  bool flip = 0;
  if (Current_B < 0) { flip = true; Current_B = fabs(Current_B); } // deal with negative readings before pow()
  double _effective_capacity = time_until_discharged_fromfull() * Current_B;
  //sprintf(log_buffer, "_effective_capacity(Ah):%.3f\r\n", _effective_capacity);
  //printf("%s",log_buffer);
  if (flip) { _effective_capacity = -_effective_capacity; Current_B = -Current_B; }
  return _effective_capacity;
}

/*
void reset_ah_capacity(void) {
  // check the voltage level, check the current flowing into the battery,
  // if the battery is held at a certain voltage and reads a particular current in or out, 
  // for a certain length of time, then battery is full.
  // for example, lead-acid generally, this could be a battery at >14.0V with less than .1A flowing in or out per cell, for 30 minutes.
  // check battery manufactures instructions and the charger specification and charging modes to know what's expected.
  // https://batteryuniversity.com/learn/article/charging_the_lead_acid_battery
  if (Voltage_B >= 14.0 && Current_B <= 0.1) {
    bool charged_inquiry = true;
    //uint32_t time1_ah_reset = currentMillis;
  }
}
*/