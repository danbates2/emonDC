// emonDC code additions, Dan Bates 2019.
// Free to use and modify.

#include "emondc.h"
#include "input.h"
#include "config.h"
#include "wifi.h"

unsigned long main_emondc_interval = 10000;

byte emondc_settings = B10010011;

int ADC_DelayTest = 2; // microsecond delay in ADC routine, to improve accuracy, related to the Sample and Hold architecture of the ADC.


// The RTC is an option for off-grid stand alone dataloggers without an internet connection.
// PCF8523 Real-time Clock setup
#include <Wire.h>
#include "RTClib.h"
#define ESP8266
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

// MCP3208
#define CHIP_SELECT_MCP    2 // CS
#define DATAOUT           13 // MOSI
#define DATAIN            12 // MISO
#define SPICLOCK          14 // CLK

unsigned long previousMillis = 0;
int CH_A_CURRENT = 0;
int CH_A_VOLTAGE = 0;
int CH_B_CURRENT = 0;
int CH_B_VOLTAGE = 0;
int VREF_1 = 0;
int VREF_2 = 0;
int VREF33 = 0;
int CH6 = 0;
int CH7 = 0;
double CH_A_CURRENT_ACCUMULATOR = 0.000;
double CH_A_VOLTAGE_ACCUMULATOR = 0.000;
double CH_B_CURRENT_ACCUMULATOR = 0.000;
double CH_B_VOLTAGE_ACCUMULATOR = 0.000;
double VREF_1_ACCUMULATOR = 0.000;
double VREF_2_ACCUMULATOR = 0.000;
double VREF33_ACCUMULATOR = 0.000;
double CH7_ACCUMULATOR = 0.000;

double    CH_A_CURRENT_AVERAGED;
double    CH_A_VOLTAGE_AVERAGED;
double    CH_B_CURRENT_AVERAGED;
double    CH_B_VOLTAGE_AVERAGED;
double    VREF_1_AVERAGED;
double    VREF_2_AVERAGED;
double    VREF33_AVERAGED;
double    CH7_AVERAGED;

unsigned long numberofsamples = 0;

String ADC_KeyValue_String = "";

unsigned long currentMillis;

//Setup()
void emondcsetup(void) {
  //MCP3208
  //set pin modes
  pinMode(CHIP_SELECT_MCP, OUTPUT);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  //disable device to start with
  digitalWrite(CHIP_SELECT_MCP, HIGH);
  digitalWrite(DATAOUT, LOW);
  digitalWrite(SPICLOCK, LOW);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
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




void emondcsampling(void) {

  currentMillis = millis();

  CH_A_CURRENT = read_adc(1);
  CH_A_VOLTAGE = read_adc(2);
  CH_B_CURRENT = read_adc(3);
  CH_B_VOLTAGE = read_adc(4);
  VREF_1 = read_adc(6);
  VREF_2 = read_adc(5);
  VREF33 = read_adc(7);
  CH7 = read_adc(8); // Labeled on the physical board as CH7, the channels on the ADC can be described as being from 0-7, or 1-8.

  CH_A_CURRENT_ACCUMULATOR = CH_A_CURRENT_ACCUMULATOR + CH_A_CURRENT;
  CH_A_VOLTAGE_ACCUMULATOR = CH_A_VOLTAGE_ACCUMULATOR + CH_A_VOLTAGE;
  CH_B_CURRENT_ACCUMULATOR = CH_B_CURRENT_ACCUMULATOR + CH_B_CURRENT;
  CH_B_VOLTAGE_ACCUMULATOR = CH_B_VOLTAGE_ACCUMULATOR + CH_B_VOLTAGE;
  VREF_1_ACCUMULATOR = VREF_1_ACCUMULATOR + VREF_1;
  VREF_2_ACCUMULATOR = VREF_2_ACCUMULATOR + VREF_2;
  VREF33_ACCUMULATOR = VREF33_ACCUMULATOR + VREF33;
  CH7_ACCUMULATOR = CH7_ACCUMULATOR + CH7;
  numberofsamples++;

  if (currentMillis - previousMillis >= main_emondc_interval) {
    //int overrunMillis = currentMillis % main_emondc_interval;
    previousMillis = currentMillis;
    //previousMillis = previousMillis - overrunMillis;

    averaging_print_send();
  }
}

void averaging_print_send(void) {
  Serial.print("Micros: ");
  Serial.println(micros());

  CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
  CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
  CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
  CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
  VREF_1_AVERAGED = VREF_1_ACCUMULATOR / numberofsamples;
  VREF_2_AVERAGED = VREF_2_ACCUMULATOR / numberofsamples;
  VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
  CH7_AVERAGED = CH7_ACCUMULATOR / numberofsamples;

  //  double CHANNEL_A_POWER = CH_A_CURRENT_AVERAGED * CH_A_VOLTAGE_AVERAGED;
  //  double CHANNEL_B_POWER = CH_B_CURRENT_AVERAGED * CH_B_VOLTAGE_AVERAGED;

  Serial.print("CH_A_CURRENT_AVERAGED: ");
  Serial.println(CH_A_CURRENT_AVERAGED);
  Serial.print("CH_A_VOLTAGE_AVERAGED: ");
  Serial.println(CH_A_VOLTAGE_AVERAGED);
  Serial.print("CH_B_CURRENT_AVERAGED: ");
  Serial.println(CH_B_CURRENT_AVERAGED);
  Serial.print("CH_B_VOLTAGE_AVERAGED: ");
  Serial.println(CH_B_VOLTAGE_AVERAGED);
  /*
      Serial.print("CHANNEL_A_POWER: ");
      Serial.println(CHANNEL_A_POWER);
      Serial.print("CHANNEL_B_POWER: ");
      Serial.println(CHANNEL_B_POWER);
  */
  Serial.print("VREF_UNI_AVERAGED: ");
  Serial.println(VREF_1_AVERAGED);
  Serial.print("VREF_BI_AVERAGED: ");
  Serial.println(VREF_2_AVERAGED);
  Serial.print("VREF33_AVERAGED: ");
  Serial.println(VREF33_AVERAGED);
  Serial.print("CH7_AVERAGED: ");
  Serial.println(CH7_AVERAGED);

  ADC_KeyValue_String = "Volts_A:" + String(CH_A_VOLTAGE_AVERAGED, 2);
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_A:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH_A_CURRENT_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";/*
      ADC_KeyValue_String = ADC_KeyValue_String + "Power_A:";
      ADC_KeyValue_String = ADC_KeyValue_String + CHANNEL_A_POWER;
      ADC_KeyValue_String = ADC_KeyValue_String + ",";*/
  ADC_KeyValue_String = ADC_KeyValue_String + "Volts_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH_B_VOLTAGE_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Amps_B:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH_B_CURRENT_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";/*
      ADC_KeyValue_String = ADC_KeyValue_String + "Power_B:";
      ADC_KeyValue_String = ADC_KeyValue_String + CHANNEL_B_POWER;
      ADC_KeyValue_String = ADC_KeyValue_String + ",";*/
  ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Uni:";
  ADC_KeyValue_String = ADC_KeyValue_String + VREF_1_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "AmpsRef_Bi:";
  ADC_KeyValue_String = ADC_KeyValue_String + VREF_2_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "3.3V_Ref:";
  ADC_KeyValue_String = ADC_KeyValue_String + VREF33_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "CH7:";
  ADC_KeyValue_String = ADC_KeyValue_String + CH7_AVERAGED;
  ADC_KeyValue_String = ADC_KeyValue_String + ",";
  ADC_KeyValue_String = ADC_KeyValue_String + "Samples:";
  ADC_KeyValue_String = ADC_KeyValue_String + numberofsamples;

  Serial.println(ADC_KeyValue_String);
  input_string = ADC_KeyValue_String;

  Serial.print("Number of samples: ");
  Serial.println(numberofsamples);

  CH_A_CURRENT_ACCUMULATOR = 0;
  CH_A_VOLTAGE_ACCUMULATOR = 0;
  CH_B_CURRENT_ACCUMULATOR = 0;
  CH_B_VOLTAGE_ACCUMULATOR = 0;
  VREF_1_ACCUMULATOR = 0;
  VREF_2_ACCUMULATOR = 0;
  VREF33_ACCUMULATOR = 0;
  CH7_ACCUMULATOR = 0;

  numberofsamples = 0;

  drawvalues();
} // Averaging and print loop end.



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
    digitalWrite(DATAOUT, commandbits & 1 << i);
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
    adcvalue += digitalRead(DATAIN) << i;
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }
  digitalWrite(CHIP_SELECT_MCP, HIGH); //turn off device
  return adcvalue;
}


void drawvalues(void) {
  DateTime now = rtc.now();

  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner

  if (screentog == 0) {
    display.println(F("Volts(A)")); display.println(CH_A_VOLTAGE_AVERAGED);
    screentog = 1;
  }
  else if (screentog == 1) {
    display.println(F("Amps(A)")); display.println(CH_A_CURRENT_AVERAGED);
    screentog = 2;
  }
  else if (screentog == 2) {
    display.println(F("Volts(B)")); display.println(CH_B_VOLTAGE_AVERAGED);
    screentog = 3;
  }
  else if (screentog == 3) {
    display.println(F("Amps(B)")); display.println(CH_B_CURRENT_AVERAGED);
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
}

void config_save_emondc(unsigned long  interval)
{
  main_emondc_interval = interval;

  /*
    EEPROM_write_string(EEPROM_WWW_USER_START, EEPROM_WWW_USER_SIZE, user);
    EEPROM_write_string(EEPROM_WWW_PASS_START, EEPROM_WWW_PASS_SIZE, pass);

    EEPROM.commit();
  */

}
