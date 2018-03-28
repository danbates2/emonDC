/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.
   -------------------------------------------------------------------
   Modified by Daniel Bates for integration of ADC, MCP3202, for emonDC
   project, 2017.

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

#include "emonesp.h"
#include "config.h"
#include "wifi.h"
#include "web_server.h"
#include "ota.h"
#include "input.h"
#include "emoncms.h"
#include "mqtt.h"
#include <SPI.h>

#define CS_3202 15

int channel0;
int channel1;
int channel0accumulator;
int channel1accumulator;
int numberofsamples;
int channel0avgA;
int channel1avgA;

float step1currentAcalibrated;
float step2currentAcalibrated;
float step3currentAcalibrated;
float done_currentAcalibrated;

float step1voltageAcalibrated;
float step2voltageAcalibrated;
float done_voltageAcalibrated;

double powerAcalc;

double Whaccumulator;

const float ADCsteps = 4096.00;

// Current calibration constants
float supplyvoltage = 5.00; //voltage dictating Vref for ADC
const float shunt_resistance = 0.00050;
const float shunt_monitor_gain = 100.00;

// Voltage calibration constants
const float Vdiv = 17.949152542;

//Step1 of Calibration should be to read the Vcc then creat a calibration factor to ADC steps 4096

unsigned long previousMillis = 0;        // will store last time updated
const long interval = 1000;           // interval at which to log (milliseconds)
int numberofintervals = 1;

unsigned long previousMillis2 = 0;
const long interval2 = 10000;

String CurrentA = "starttest";
String VoltageA = "starttest";
String PowerA = "starttest";
String TimeMillis = "starttest";
String IntervalNumber = "starttest";

String adcstringinput = "starttest";

// -------------------------------------------------------------------
// SETUP
// -------------------------------------------------------------------
void setup() {
  delay(2000);

  pinMode(CS_3202, OUTPUT);
  digitalWrite(CS_3202, HIGH);

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
  ota_setup();

  DEBUG.println("Server started");

  delay(100);
} // end setup

// -------------------------------------------------------------------
// LOOP
// -------------------------------------------------------------------
void loop()
{
  //
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  channel0 = (Read3202(0, CS_3202));
  channel1 = (Read3202(1, CS_3202));
  SPI.end();
  channel0accumulator = channel0accumulator + channel0;
  channel1accumulator = channel1accumulator + channel1;
  numberofsamples++;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //  Serial.println("START");
    Serial.print("Samples - ");
    Serial.println(numberofsamples);

    channel0avgA = channel0accumulator / numberofsamples;
    channel1avgA = channel1accumulator / numberofsamples;

    step1currentAcalibrated = channel0avgA / ADCsteps;
    step2currentAcalibrated = step1currentAcalibrated * supplyvoltage;
    step3currentAcalibrated = step2currentAcalibrated / shunt_monitor_gain;
    done_currentAcalibrated = step3currentAcalibrated / shunt_resistance;

    step1voltageAcalibrated = channel1avgA / ADCsteps;
    step2voltageAcalibrated = step1voltageAcalibrated * supplyvoltage;
    done_voltageAcalibrated = step2voltageAcalibrated * Vdiv;

    powerAcalc = done_currentAcalibrated * done_voltageAcalibrated;
    Whaccumulator = Whaccumulator + powerAcalc;

    Serial.print("channel0avgA - ");
    Serial.println(channel0avgA);
    Serial.print("done_currentAcalibrated - ");
    Serial.println(done_currentAcalibrated);
    Serial.print("channel1avgA - ");
    Serial.println(channel1avgA);
    Serial.print("done_voltageAcalibrated - ");
    Serial.println(done_voltageAcalibrated);
    Serial.print("powerAcalc - ");
    Serial.println(powerAcalc);
    Serial.print("Whaccumulator - ");
    Serial.println(Whaccumulator / 3600);

    Serial.print("numberofintervals - ");
    Serial.println(numberofintervals);
    Serial.println();
    numberofintervals++;

    numberofsamples = 0;
    channel0accumulator = 0;
    channel1accumulator = 0;
  }
  //

  ota_loop();
  web_server_loop();
  wifi_loop();


  unsigned long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time done
    previousMillis2 = currentMillis2;

    CurrentA = String(done_currentAcalibrated, 2);
    VoltageA = String(done_voltageAcalibrated, 2);
    PowerA = String(powerAcalc, 2);
    TimeMillis = String(millis(), DEC);
    IntervalNumber = String(numberofintervals, DEC);

    //example string CT1:3935,CT2:325,T1:12.5,T2:16.9,T3:11.2,T4:34.7

    String adcstringinput = ("AC:" + CurrentA + ",AV:" + VoltageA + ",AP:" + PowerA);
    Serial.println(adcstringinput);
    Serial.println();

    boolean gotInput = true;
    String input = adcstringinput;

    /*
        String input = "";
        boolean gotInput = input_get(input);
    */

    if (wifi_mode == WIFI_MODE_STA || wifi_mode == WIFI_MODE_AP_AND_STA) {
      if (emoncms_apikey != 0 && gotInput) {
        emoncms_publish(input);
      }
      if (mqtt_server != 0) {
        mqtt_loop();
        if (gotInput) {
          mqtt_publish(input);
        }
      }
    }
  }

} // end loop

int Read3202(int CHANNEL, int CS) {
  int msb;
  int lsb;
  int commandBytes = B10100000 ;// channel 0
  if (CHANNEL == 1) commandBytes = B11100000 ; // channel 1
  digitalWrite(CS, LOW);
  SPI.transfer (B00000001);// Start bit
  msb = SPI.transfer(commandBytes) ;
  msb = msb & B00001111;
  lsb = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  return ((int) msb) << 8 | lsb;
}

