/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.
   -------------------------------------------------------------------
   Modified by Daniel Bates for integration of ADC, MCP3204/8, for emonDC
   project, 2017-18.

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
#include "SPI.h"
#include "Mcp3208.h"

#define SPI_CS      15       // SPI slave select
#define ADC_VREF    5000     // Vref
#define ADC_CLK     100000  // SPI clock Freq(Hz)

MCP3208 adc(ADC_VREF, SPI_CS);
/*
 * The plan is to have 6 channels monitored and for a calculation to be made to convert raw ADC values
 * to legible data for posting to emonCMS.
 * The channel pairs are multiplied to create two values of Power.
 * The power is accumulated for each pair as Wh.
 */

// Current calibration constants
float supplyvoltage = 5.00; //voltage dictating Vref for ADC
const float shunt_resistance = 0.00050;
const float shunt_monitor_gain = 100.00;

// Voltage calibration constants
const float R1 = 47000;
const float R2 = 3300;
const float R2 = 3300;


int numberofsamples;

double powerAcalc;
double powerBcalc;

const float ADCsteps = 4096.00;



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

  // configure PIN mode
  pinMode(SPI_CS, OUTPUT);

  // set initial PIN state
  digitalWrite(SPI_CS, HIGH);

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
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
  uint16_t ch0_raw = adc.read(MCP3208::SINGLE_0); //CHANNEL A current
  uint16_t ch1_raw = adc.read(MCP3208::SINGLE_1); //CHANNEL A volts
  uint16_t ch2_raw = adc.read(MCP3208::SINGLE_2); //CHANNEL B current
  uint16_t ch3_raw = adc.read(MCP3208::SINGLE_3); //CHANNEL B volts
  uint16_t ch4_raw = adc.read(MCP3208::SINGLE_4); //BIDIRECTIONAL REF
  //uint16_t ch5_raw = adc.read(MCP3208::SINGLE_5); //3.3V REF 
  SPI.end();
  numberofsamples++;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {

   
    numberofsamples = 0;
    channel0accumulator = 0;
    channel1accumulator = 0;
  }
  //

  ota_loop();
  web_server_loop();
  wifi_loop();


  unsigned long currentMillis10sec = millis();
  if (currentMillis10sec - previousMillis10sec >= interval2) {
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
