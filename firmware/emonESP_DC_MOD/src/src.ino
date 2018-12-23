/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.

   Changed for emonDC ADC sampling by Daniel Bates.
   -------------------------------------------------------------------

   This file is part of OpenEnergyMonitor.org project.
   EmonESP is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
   EmonESP is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with EmonESP; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

const int main_adcpost_interval = 5000; // milliseconds interval for averaging samples and sending to emonCMS.

int ADC_DelayTest = 5; //microsecond delay in ADC routine, to improve accuracy,
                      // related to the Sample and Hold architecture of the ADC.

#include "emonesp.h"
#include "config.h"
#include "wifi.h"
#include "web_server.h"
//#include "ota.h"
#include "input.h"
#include "emoncms.h"
#include "mqtt.h"

/* Possibly the RTC is not necessary as emonESP gets the time from a connected client over WiFi.
// PCF8523 Real-time Clock setup
#include <Wire.h>
#include "RTClib.h"
#define ESP8266
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
*/

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
int VREF_BIAS1 = 0;
int VREF_BIAS2 = 0;
int VREF33 = 0;
int CH6 = 0;
int CH7 = 0;
double CH_A_CURRENT_ACCUMULATOR = 0.000;
double CH_A_VOLTAGE_ACCUMULATOR = 0.000;
double CH_B_CURRENT_ACCUMULATOR = 0.000;
double CH_B_VOLTAGE_ACCUMULATOR = 0.000;
double VREF_BIAS1_ACCUMULATOR = 0.000;
double VREF_BIAS2_ACCUMULATOR = 0.000;
double VREF33_ACCUMULATOR = 0.000;
double CH6_ACCUMULATOR = 0.000;
double CH7_ACCUMULATOR = 0.000;
long numberofsamples = 0;

String ADC_KeyValue_String = "";

// -------------------------------------------------------------------
// SETUP
// -------------------------------------------------------------------
void setup() {
  delay(100);
/*
  //PCF8523
  rtc.begin();
  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
*/
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

  Serial.begin(115200);
#ifdef DEBUG_SERIAL1
  Serial1.begin(115200);
#endif

  DEBUG.println();
  DEBUG.print("EmonESP ");
  DEBUG.println(ESP.getChipId());
  DEBUG.println("Firmware: " + currentfirmware);

  // Read saved settings from the config
  config_load_settings();

  // Initialise the WiFi
  wifi_setup();

  // Bring up the web server
  web_server_setup();

  // Start the OTA update systems
  //ota_setup();

  DEBUG.println("Server started");

  delay(50);

} // end setup

// -------------------------------------------------------------------
// LOOP
// -------------------------------------------------------------------
void loop()
{
  //  ota_loop();
  web_server_loop();
  wifi_loop();

  String input = "";
  boolean gotInput = input_get(input);

  if (wifi_mode == WIFI_MODE_STA || wifi_mode == WIFI_MODE_AP_AND_STA)
  {
    if (emoncms_apikey != 0 && gotInput) {
      emoncms_publish(input);
    }
    if (mqtt_server != 0)
    {
      mqtt_loop();
      if (gotInput) {
        mqtt_publish(input);
      }
    }
  }

  // Sampling and Averaging Loop section for the MCP3208 ADC.
  unsigned long currentMillis = millis();

  CH_A_CURRENT = read_adc(1);
  CH_A_VOLTAGE = read_adc(2);
  CH_B_CURRENT = read_adc(3);
  CH_B_VOLTAGE = read_adc(4);
  VREF_BIAS2 = read_adc(5);
  VREF33 = read_adc(6);
  CH6 = read_adc(7); // Labeled on the physical board as CH6
  CH7 = read_adc(8); // Labeled on the physical board as CH7, the channels on the ADC can be described as being from 0-7, or 1-8.

  CH_A_CURRENT_ACCUMULATOR = CH_A_CURRENT_ACCUMULATOR + CH_A_CURRENT;
  CH_A_VOLTAGE_ACCUMULATOR = CH_A_VOLTAGE_ACCUMULATOR + CH_A_VOLTAGE;
  CH_B_CURRENT_ACCUMULATOR = CH_B_CURRENT_ACCUMULATOR + CH_B_CURRENT;
  CH_B_VOLTAGE_ACCUMULATOR = CH_B_VOLTAGE_ACCUMULATOR + CH_B_VOLTAGE;
  VREF_BIAS2_ACCUMULATOR = VREF_BIAS2_ACCUMULATOR + VREF_BIAS2;
  VREF33_ACCUMULATOR = VREF33_ACCUMULATOR + VREF33;
  CH6_ACCUMULATOR = CH6_ACCUMULATOR + CH6;
  CH7_ACCUMULATOR = CH7_ACCUMULATOR + CH7;
  numberofsamples++;

  if (currentMillis - previousMillis >= main_adcpost_interval) {
    Serial.print("Micros: ");
    Serial.println(micros());

    int overrunMillis = currentMillis % main_adcpost_interval;
    previousMillis = currentMillis;
    previousMillis = previousMillis - overrunMillis;

    double CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
    double CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
    double CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
    double CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;
    double VREF_BIAS2_AVERAGED = VREF_BIAS2_ACCUMULATOR / numberofsamples;
    double VREF33_AVERAGED = VREF33_ACCUMULATOR / numberofsamples;
    double CH6_AVERAGED = CH6_ACCUMULATOR / numberofsamples;
    double CH7_AVERAGED = CH7_ACCUMULATOR / numberofsamples;

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
    Serial.print("VREF_BIAS2_AVERAGED: ");
    Serial.println(VREF_BIAS2_AVERAGED);
    Serial.print("VREF33_AVERAGED: ");
    Serial.println(VREF33_AVERAGED);
    Serial.print("CH6_AVERAGED: ");
    Serial.println(CH6_AVERAGED);
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
    ADC_KeyValue_String = ADC_KeyValue_String + "Amps_AB_Ref:";
    ADC_KeyValue_String = ADC_KeyValue_String + VREF_BIAS2_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "3.3V_Ref:";
    ADC_KeyValue_String = ADC_KeyValue_String + VREF33_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "CH6:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH6_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "CH7:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH7_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "Samples:";
    ADC_KeyValue_String = ADC_KeyValue_String + numberofsamples;
    /*
    // Real-time Clock
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "Time:";
    DateTime now = rtc.now();
    long timenow1 = now.unixtime();
    ADC_KeyValue_String = ADC_KeyValue_String + timenow1;
*/

    Serial.println(ADC_KeyValue_String);
    input_string = ADC_KeyValue_String;

    Serial.print("Number of samples: ");
    Serial.println(numberofsamples);

    CH_A_CURRENT_ACCUMULATOR = 0;
    CH_A_VOLTAGE_ACCUMULATOR = 0;
    CH_B_CURRENT_ACCUMULATOR = 0;
    CH_B_VOLTAGE_ACCUMULATOR = 0;
    VREF_BIAS2_ACCUMULATOR = 0;
    VREF33_ACCUMULATOR = 0;
    CH6_ACCUMULATOR = 0;
    CH7_ACCUMULATOR = 0;

    numberofsamples = 0;
  }
  // Sampling and Averaging loop end.

} // end loop

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
