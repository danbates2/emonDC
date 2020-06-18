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
#include "oled.h"
#include "AH_MCP320x.h"

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String hw_version = ESCAPEQUOTE(HW_TAG);

// -------------------------------------------------------------------
// change the following variables for your system, values will be overwritten by EEPROM save function (triggered in web_server in emonDC save settings), and EEPROM values are then used instead.
// -------------------------------------------------------------------
uint16_t main_interval_seconds = 5; // interval to post data
uint16_t readings_interval = 1; // one second
bool chanA_VrefSet = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
bool chanB_VrefSet = 1; // Channel reference select: 0 for unidirectional, 1 for bidirectional.
uint16_t channelA_gain = 100; // shunt amp gain.
uint16_t channelB_gain = 100;
// values of resistor divider for voltage reading.
uint32_t R1_A = 1000000; // 1Mohm default.
uint32_t R2_A = 75000; // 75kohm default.
uint32_t R1_B = 1000000;
uint32_t R2_B = 75000;
double Rshunt_A = 0.001; // default value of Rsense in Ohms.
double Rshunt_B = 0.01; // Y14870R01000B9R Yishay Foil Resistor 0.1% 0.01R

// Calibration values for manual adjustment.
double icalA = 1.000; // CURRENT_A.
double vcalA = 1.000; // VOLTAGE_A.
double icalB = 1.000; // CURRENT_B.
double vcalB = 1.000; // VOLTAGE_B.
// Amplifier offset corrections for zero measurements, ideally these would be maps derived from experiment.
double AmpOffset_A = 0.00;
double VoltOffset_A = 0.0;
double AmpOffset_B = 0.0;
double VoltOffset_B = 0.0;
// Battery State of Charge variables [https://en.wikipedia.org/wiki/Peukert's_law#Formula]
//double battery_voltage_nominal = 12.0; // just in case useful later
//double bat_max = 14.8; double bat_min = 10.6; // these actually change with temperature, and are therefore crude values.

// battery data.
uint16_t BattType = 1; // for automatic settings idea, unused. 1 for flooded lead-acid, 2 for gel lead-acid, 3 for li-ion...
double BattCapacity = 90.0; // Amp-hour capacity at rated hours (normally 20 hours).
uint16_t BattCapHr = 20.0; // hour rating default to 20.
uint16_t BattNom = 12.0; // for automatic settings idea, unused.
double BattVoltsAlarmHigh = 15.0; // useful alarm?
double BattVoltsAlarmLow = 10.5; // useful alarm?
double BattPeukert = 1.3; // see smartgauge.co.uk/ to learn about the Peukert constant.
double BattTempCo = 0.3; // battery charge temperature coefficient, normally for lead-acid (mV/°C) for use in conjunction with temperature probe on CH8, untested unused.


// -------------------------------------------------------------------
// Other Settings.
// -------------------------------------------------------------------

// Misc:
double state_of_charge = 1.00 ; // 1 = 100% percent
double time_until_discharged; // seconds until discharged


// OLED Display
uint32_t oled_interval = 5000; // cycle through data every 5 seconds.
uint32_t oled_previousMillis;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET  -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int screentog = -1; // rotating variable for knowing which data to present on OLED.
int max_screentog = 4; // ...
bool oled_button_flag = false; // if a button press is detected, the screen wil go off after 10seconds.
#define IMAGE_WIDTH    128
#define IMAGE_HEIGHT   32

// SD CARD
const int chipSelectSD = 15; // SD card chip SPI chip select
SPISettings mySettings(250000, MSBFIRST, SPI_MODE0); // 10kHz is the minimum clock speed : MCP3208 datasheet.

// sample rate in this sketch is around 450 samples per channel per second at 400kHz.
// ADC
const int chipSelectADC = 2; // SD card chip SPI chip select
AH_MCP320x ADC_SPI(chipSelectADC, mySettings); // ADC SPI chip select.

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
unsigned long numberofsamplesext = 0; // for outputting sample count before clearing accumulators.

// MAX / MIN ADC channel values.
uint16_t CH_A_CURRENT_MAX = 0;
uint16_t CH_A_VOLTAGE_MAX = 0;
uint16_t CH_B_CURRENT_MAX = 0;
uint16_t CH_B_VOLTAGE_MAX = 0;
uint16_t CH_A_CURRENT_MIN = 65000;
uint16_t CH_A_VOLTAGE_MIN = 65000;
uint16_t CH_B_CURRENT_MIN = 65000;
uint16_t CH_B_VOLTAGE_MIN = 65000;

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

double Voltage_A_Max;
double Current_A_Max;
double Voltage_B_Max;
double Current_B_Max;
double Voltage_A_Min;
double Current_A_Min;
double Voltage_B_Min;
double Current_B_Min;
//------------------------------
// Battery variables
//------------------------------
double Ah_level_A = 0.0;
double Ah_level_B = 0.0;
double Ah_positive_A = 0.0;
double Ah_positive_B = 0.0;
double Ah_negative_A = 0.0;
double Ah_negative_B = 0.0;

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
unsigned int averaging_loop_counter = 0;
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




//------------------------------
// RTC TIME SETTINGS
//------------------------------
// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long rtc_now_time = 0;

//------------------------------
// NTP TIME SETTINGS
//------------------------------
//int time_offset = 3600; // timezone correction?
int time_offset = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", time_offset, 300000);
unsigned long previousMillisNTP = 0;
unsigned long intervalNTP = 60000;
// SD CARD
String datalogFilename = "datalog.txt";
bool SD_present;

//---------------
// Misc
//---------------
uint32_t main_interval_ms;
// time variables.
unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long previousPostingMillis = 0;
// calibrate solar at midnight?
bool midnight_calibration = false;
bool OLED_active = 0;
bool OLED_turnedoff = 0;
int tmp_interval;


//-----------------------------------
// emonDC Setup
//-----------------------------------
void emondc_setup(void) {

  main_interval_ms = main_interval_seconds * 1000; // called after JSON is loaded.

  // microSD card init
  pinMode(chipSelectSD, OUTPUT);
  digitalWrite(chipSelectSD, HIGH); delay(1);
  
  //if (SD.begin(chipSelectSD, 250000)) { // SPI speed sey here. Clash possible with ADC. // esp8266 framework v1.8
  if (SD.begin(chipSelectSD, mySettings)) { // SPI speed sey here. Clash possible with ADC. // esp8266 framework v2+
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
  display.println(F("emonDCduo"));    // Initial print to OLED, version number.
  display.println(hw_version);
  display.display();
  
  // RTC init
  if (!rtc.begin()) { Serial.println("RTC or I2C not functioning."); }
  else if (!rtc.initialized()) {
    Serial.println("RTC not running,\r\n  - attempting initialisation with compile time.");
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

  gpio0_setup();

  previousMillis = millis(); // for sensible start.
  oled_previousMillis = millis();
  previousPostingMillis = millis();
  _t_begin = millis() / 1000;

} // end emonDC setup.


//--------------------------------------------------
// emonDC main loop.
//--------------------------------------------------
void emondc_loop(void) {
  
  uint16_t ch0_reading = ADC_SPI.readCH(0);
  uint16_t ch1_reading = ADC_SPI.readCH(1);
  uint16_t ch2_reading = ADC_SPI.readCH(2);
  uint16_t ch3_reading = ADC_SPI.readCH(3);
  uint16_t ch4_reading = ADC_SPI.readCH(4);
  uint16_t ch5_reading = ADC_SPI.readCH(5);
  uint16_t ch6_reading = ADC_SPI.readCH(6);
  uint16_t ch7_reading = ADC_SPI.readCH(7);
  
  CH_A_CURRENT_ACCUMULATOR += ch0_reading;
  CH_A_VOLTAGE_ACCUMULATOR += ch1_reading;
  CH_B_CURRENT_ACCUMULATOR += ch2_reading;
  CH_B_VOLTAGE_ACCUMULATOR += ch3_reading;
  VREF_BI_ACCUMULATOR      += ch4_reading;
  VREF_UNI_ACCUMULATOR     += ch5_reading;
  VREF33_ACCUMULATOR       += ch6_reading;
  CH8_ACCUMULATOR          += ch7_reading;
  
  // store maximum and minimum values.
  if (ch0_reading > CH_A_CURRENT_MAX) CH_A_CURRENT_MAX = ch0_reading;
  if (ch1_reading > CH_A_VOLTAGE_MAX) CH_A_VOLTAGE_MAX = ch1_reading;
  if (ch2_reading > CH_B_CURRENT_MAX) CH_B_CURRENT_MAX = ch2_reading;
  if (ch3_reading > CH_B_VOLTAGE_MAX) CH_B_VOLTAGE_MAX = ch3_reading;
  if (ch0_reading < CH_A_CURRENT_MIN) CH_A_CURRENT_MIN = ch0_reading;
  if (ch1_reading < CH_A_VOLTAGE_MIN) CH_A_VOLTAGE_MIN = ch1_reading;
  if (ch2_reading < CH_B_CURRENT_MIN) CH_B_CURRENT_MIN = ch2_reading;
  if (ch3_reading < CH_B_VOLTAGE_MIN) CH_B_VOLTAGE_MIN = ch3_reading;
  
  numberofsamples++;

  currentMillis = millis();
  yield();
  if (currentMillis - previousMillis >= readings_interval*1000) {
    //overrunMillis = currentMillis % main_interval_ms;
    uint32_t _previousMillis = previousMillis;
    previousMillis = currentMillis;
    //previousMillis = previousMillis - overrunMillis;
    NTP_update_PCF8523_update();  // update RTC time, via network if available every 60s.   

    yield();
    average_and_calibrate(_previousMillis, currentMillis); // readying the readable values, passing necessary time values associated with the posting intervals.
    yield();

    if (currentMillis - previousPostingMillis >= main_interval_ms) {
      previousPostingMillis = currentMillis;
      //-----------------------------------
      // what to do with the ready data:
      //-----------------------------------
      _t = (millis() / 1000) - _t_begin;
      Serial.print("time since start:: ");
      Serial.println(_t);
      Serial.print("connected_network: ");
      Serial.println(connected_network);
      forward_to_emonESP(); // sending to emonCMS
      yield();
      if (SD_present) save_to_SDcard(); // save to SD card.
      yield();
      //Serial.println(time_until_discharged()); // testing Peukert calculation
      //Serial.println(effective_capacity()); // testing Peukert calculation
      //-----------------------------------
      yield();
      //-----------------------------------
      numberofsamplesext = 0;
      Serial.print("FreeRAM (bytes): ");  Serial.println(ESP.getFreeHeap()); // for debugging
      clear_accumulators();
    }
  }
  
  //--------------------------------------
  //OLED TIMER
  //--------------------------------------
  if (currentMillis - oled_previousMillis >= oled_interval) {
    oled_previousMillis = currentMillis;
    if (oled_button_flag) { // if the button's been pressed at least once
      oled_interval = 1000; // fast cycling
      if (millis() <= TimeButtonPressedExt + 2000000) { // 20 seconds until OLED turnoff
        OLED_active = true;
        draw_OLED();
      } 
      else {
        display.ssd1306_command(0b10101110); //  turn OLED off, see datasheet.
        OLED_active = false;
        screentog = 0;
        TimeButtonPressedExt = 0;
        OLED_turnedoff = true;
      }
    }
    else draw_OLED(); // default behaviour regardless of button presses.
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
  if (chanA_VrefSet == 0) CH_A_CURRENT_AVERAGED -= VREF_UNI_AVERAGED;
  else CH_A_CURRENT_AVERAGED -= VREF_BI_AVERAGED;
  if (chanB_VrefSet == 0) CH_B_CURRENT_AVERAGED -= VREF_UNI_AVERAGED;
  else CH_B_CURRENT_AVERAGED -= VREF_BI_AVERAGED;

  volts_to_adc_reading_ratio = volts_to_adc_reading_ratio_function(); // convert VREF33 into 'ADCvalue to Voltage factor'
  yield();
  // make readable voltage and current values, apply calibration.
  Current_A = make_readable_Amps(CH_A_CURRENT_AVERAGED, 0, channelA_gain) * icalA;
  Voltage_A = make_readable_Volts(CH_A_VOLTAGE_AVERAGED, 0) * vcalA;
  Current_B = make_readable_Amps(CH_B_CURRENT_AVERAGED, 1, channelB_gain) * icalB;
  Voltage_B = make_readable_Volts(CH_B_VOLTAGE_AVERAGED, 1) * vcalB;
  //Current_B -= 20.0; // testing

  Current_A_Max = make_readable_Amps(CH_A_CURRENT_MAX, 0, channelA_gain) * icalA;
  Voltage_A_Max = make_readable_Volts(CH_A_VOLTAGE_MAX, 0) * vcalA;
  Current_B_Max = make_readable_Amps(CH_B_CURRENT_MAX, 1, channelB_gain) * icalB;
  Voltage_B_Max = make_readable_Volts(CH_B_VOLTAGE_MAX, 1) * vcalB;
  Current_A_Min = make_readable_Amps(CH_A_CURRENT_MIN, 0, channelA_gain) * icalA;
  Voltage_A_Min = make_readable_Volts(CH_A_VOLTAGE_MIN, 0) * vcalA;
  Current_B_Min = make_readable_Amps(CH_B_CURRENT_MIN, 1, channelB_gain) * icalB;
  Voltage_B_Min = make_readable_Volts(CH_B_VOLTAGE_MIN, 1) * vcalB;

  
  unsigned long this_interval_ms = curr_mills - pre_mills;
  yield();
  double Ah_period = (Current_B * (this_interval_ms/1000)) / 3600.0; // Coulomb count this period.
  double soc_diff = Ah_period / effective_capacity_fromfull(); // state of charge difference this period.
  state_of_charge += soc_diff; // update state of charge.
  if (state_of_charge > 1.0) state_of_charge = 1.0;
  // C *= state_of_charge; don't adjust this on the fly because of it's effect of peukert equation.
  if (state_of_charge == 1.0) time_until_discharged = 0;
  else time_until_discharged = time_until_discharged_fromfull() * 3600  * state_of_charge;
  if (time_until_discharged < 0) time_until_discharged *= -1;
  if (Current_B > 0) time_until_discharged = 0;

  yield();
  
  averaging_loop_counter++;
  //Serial.print("Averaging: ");
  //Serial.print(averaging_loop_counter); Serial.print("    ");
  
} // Averaging loop end.

void clear_accumulators(void) {
  CH_A_CURRENT_ACCUMULATOR = 0;
  CH_A_VOLTAGE_ACCUMULATOR = 0;
  CH_B_CURRENT_ACCUMULATOR = 0;
  CH_B_VOLTAGE_ACCUMULATOR = 0;
  VREF_UNI_ACCUMULATOR = 0;
  VREF_BI_ACCUMULATOR = 0;
  VREF33_ACCUMULATOR = 0;
  CH8_ACCUMULATOR = 0;
  numberofsamplesext = numberofsamples;
  numberofsamples = 0;

  CH_A_CURRENT_MAX = 0;
  CH_A_VOLTAGE_MAX = 0;
  CH_B_CURRENT_MAX = 0;
  CH_B_VOLTAGE_MAX = 0;
  CH_A_CURRENT_MIN = 0;
  CH_A_VOLTAGE_MIN = 0;
  CH_B_CURRENT_MIN = 0;
  CH_B_VOLTAGE_MIN = 0;
}

//---------------------------------------------------------------------------
// Build Strign to forward to emonESP, same string is stored to SD card.
//---------------------------------------------------------------------------
void forward_to_emonESP(void)
{
/*
  double Voltage_A_Max;
  double Current_A_Max;
  double Voltage_B_Max;
  double Current_B_Max;

  double Voltage_A_Min;
  double Current_A_Min;
  double Voltage_B_Min;
  double Current_B_Min;
*/

  ADC_KeyValue_String = "Volts_A:" + String(Voltage_A, 2);
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Volts_A_Max:";
  ADC_KeyValue_String += Voltage_A_Max;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Volts_A_Min:";
  ADC_KeyValue_String += Voltage_A_Min;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_A:";
  ADC_KeyValue_String += Current_A;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_A_Max:";
  ADC_KeyValue_String += Current_A_Max;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_A_Min:";
  ADC_KeyValue_String += Current_A_Min;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Volts_B:";
  ADC_KeyValue_String += Voltage_B;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Volts_B_Max:";
  ADC_KeyValue_String += Voltage_B_Max;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Volts_B_Min:";
  ADC_KeyValue_String += Voltage_B_Min;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_B:";
  ADC_KeyValue_String += Current_B;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_B_Max:";
  ADC_KeyValue_String += Current_B_Max;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Amps_B_Min:";
  ADC_KeyValue_String += Current_B_Min;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Ref_Uni:";
  ADC_KeyValue_String += (VREF_UNI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "Ref_Bi:";
  ADC_KeyValue_String += (VREF_BI_AVERAGED * volts_to_adc_reading_ratio);
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "CH8:";
  ADC_KeyValue_String += CH8_AVERAGED;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "SoC(%):";
  ADC_KeyValue_String += (state_of_charge*100);
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "TimeUD(h):";
  ADC_KeyValue_String += time_until_discharged/3600;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "samplecount:";
  ADC_KeyValue_String += numberofsamplesext;
  ADC_KeyValue_String += ",";
  ADC_KeyValue_String += "time:";
  ADC_KeyValue_String += rtc_now_time;
  input_string = ADC_KeyValue_String;

}


//-------------------------
// 128x32 I2C OLED
//-------------------------
void draw_OLED() {
  oled_previousMillis = currentMillis;
  

  if (screentog == -1) {
    screentog = 0;
    return;
  }
  
  
  if (!OLED_active || OLED_turnedoff) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("SSD1306 allocation failed."); // Address 0x3C for 128x32  
    OLED_turnedoff = false;
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
  }

  display.clearDisplay();
  //display.setRotation(2);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  //display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left cornerr

  if (screentog == 0) {
    jumpback_oled:
    display.print(F("Ch A | ")); display.print(Voltage_A,3); display.println(F(" Volts"));
    display.print(F("     | ")); display.print(Current_A,3); display.println(F(" Amps"));
    display.print(F("Ch B | ")); display.print(Voltage_B,3); display.println(F(" Volts"));
    display.print(F("     | ")); display.print(Current_B,3); display.println(F(" Amps"));
    if (!OLED_active) screentog++;
  }
  else if (screentog == 1) {
    display.println(F("State of Charge")); 
    display.print(F("  ")); display.print((state_of_charge*100.0),0); display.println(F(" %"));
    display.println(F("Time to Discharged")); 
    display.print(F("  "));
    float hours_until_discharged = time_until_discharged / 3600.0;
    if (hours_until_discharged > 0 || hours_until_discharged < 2000) {
     display.print(hours_until_discharged,1); display.println(F(" hour(s)"));
    }
    else display.print(F("–––")); 
    if (!OLED_active) screentog++;
    
  }
  else if (screentog == 2) {
    display.print(F("SSID ")); 
    display.println(connected_network);
    display.print(F("IP "));
    display.println(ipaddress);
    
    DateTime now = rtc.now(); rtc_now_time = now.unixtime();
    display.print(F("RTC | "));
    display.print(now.day(), DEC);
    display.print('/');
    display.print(now.month(), DEC);
    display.print('/');
    display.println(now.year(), DEC);
    display.print("      ");
    display.print(now.hour(), DEC);
    display.print(':');
    display.print(now.minute(), DEC);
    display.print(':');
    display.print(now.second(), DEC);
    if (!OLED_active) screentog++;
  }
  else if (screentog == 3) { // hyteresis management, for clearer info display // user sets manual hysteresis?
    if (Current_B > 0.2) {
      display.drawBitmap(0, 0, charging_bmp, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
    }
    else if (Current_B < -0.2) {
      display.drawBitmap(0, 0, discharging_bmp, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
    }
    else {screentog = 0; goto jumpback_oled;}
    if (!OLED_active) screentog++;
  }
  
  if (screentog > max_screentog) screentog = 0; // reset screen page number.
  
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
//double Vcal_coefficient_A = ((double)R2_A / ((double)R1_A + (double)R2_A)); // resistor divider calc.
//double Vcal_coefficient_B = ((double)R2_B / ((double)R1_B + (double)R2_B)); // resistor divider calc.

double make_readable_Volts (double _Value, bool _chan) {

  double _readableV1 = _Value * volts_to_adc_reading_ratio;

  if (_chan == 0) {
    double _readableVolts = _readableV1 / ((double)R2_A / ((double)R1_A + (double)R2_A));
    _readableVolts += VoltOffset_A;
    return (_readableVolts);
  }
  else {
    double _readableVolts = _readableV1 / ((double)R2_B / ((double)R1_B + (double)R2_B));
    _readableVolts += VoltOffset_B;
    return (_readableVolts);
  }
}


//--------------------------------------------------
// ADC averaged value to Amps
//--------------------------------------------------
double make_readable_Amps (double _Value, bool _chan, double _gain) {

  double _readable_2 = (_Value * volts_to_adc_reading_ratio) / _gain; // gives mV signal value.

  if (_chan == 0) {
    double _readableAmps = _readable_2 / Rshunt_A; // I=V/R
    _readableAmps += AmpOffset_A; // shunt monitor offset calibration
    return (_readableAmps);
  }
  else {
    double _readableAmps = _readable_2 / Rshunt_B; // I=V/R
    _readableAmps += AmpOffset_B; // shunt monitor offset calibration
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
  double wh = watt_seconds / 3600.0;
  return wh;
}


//-------------------------
// Battery monitoring.
//-------------------------
double time_until_discharged_fromfull(void) { // returns seconds until discharged from ideal battery condition.
  // http://www.smartgauge.co.uk/peukert2.html
  // T = (Hr*(C/Hr)^n)/(I^n)
  double Peukert_capacity = BattCapHr * pow((BattCapacity/BattCapHr), BattPeukert);
  bool flip = 0;
  if (Current_B < 0) { flip = true; Current_B = fabs(Current_B); }
  double _time_until_discharged = Peukert_capacity / (pow(Current_B, BattPeukert));
  //sprintf(log_buffer, "_time_until_discharged(seconds):%.3f\r\n", _time_until_discharged*3600);
  //printf("%s",log_buffer);
  if (flip) { _time_until_discharged = -_time_until_discharged; Current_B = -Current_B; }
  return _time_until_discharged;
}

double effective_capacity_fromfull(void) { // returns eff. cap. in Ah.
  // http://www.smartgauge.co.uk/technical1.html
  // http://www.smartgauge.co.uk/peukert3.html
  //bool flip = 0;
  //if (Current_B < 0) { flip = true; Current_B = fabs(Current_B); } // deal with negative readings before pow()
  double _effective_capacity = time_until_discharged_fromfull() * Current_B;
  //sprintf(log_buffer, "_effective_capacity(Ah):%.3f\r\n", _effective_capacity);
  //printf("%s",log_buffer);
  //if (flip) { _effective_capacity = -_effective_capacity; Current_B = -Current_B; }
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
/*
int timer1 = 1000;
int timer1_previous;

int timer2 = 5000;
int timer2_previous;

int timer3 = main_interval_ms;
int timer3_previous;

if (millis() >= timer1 + timer1_previous) {
  FastDisplay();
}
 
if (millis() >= timer2 + timer2_previous) {
  MiddleStore();
}

if (millis() >= timer3 + timer3_previous) {
  PostData();
}

void FastDisplay(void) {

}

void MiddleStore(void) {

}

void PostData(void) {

}




*/