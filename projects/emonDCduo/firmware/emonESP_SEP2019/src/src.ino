/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.

   Changed for emonDC by Daniel Bates.
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

// NTC library: https://github.com/arduino-libraries/NTPClient
// RTC library: https://github.com/adafruit/RTClib
// PubSubClient library: https://github.com/knolleary/pubsubclient
// AsyncWebServer library: https://github.com/me-no-dev/ESPAsyncWebServer

#define RELEASE false
#define NTCENABLE false
#define GPIO0YES false
//#define MICROSDEBUG

#include "emonesp.h"
#include "config.h"
#include "wifi.h"
#include "web_server.h"
#include "ota.h"
#include "input.h"
#include "emoncms.h"
#include "mqtt.h"

#include "emondc.h" // emonDC code additions.
#include "gpio0.h" // button pressing.
#include "sleep.h" // deep-sleep mode management.

/*
  #ifdef NTCENABLE true
  // NTCclient stuff below.
  int time_offset = 3600;
  #include <WiFiUdp.h>
  #include <NTPClient.h>
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", time_offset, 300000);
  unsigned long previousMillisNTC = 0;
  const long intervalNTC = 5000;
  #endif
*/



// -------------------------------------------------------------------
// SETUP
// -------------------------------------------------------------------
void setup() {
  delay(100);
  Serial.begin(115200);

  currentfirmware = "2.3.2_emonDCmod";

  DEBUG.println();
  DEBUG.print("EmonESP ");
  DEBUG.println(ESP.getChipId());

  DEBUG.println("Firmware: " + currentfirmware);

  DEBUG.print("Reset cause: ");
  DEBUG.println(ESP.getResetReason());

  if (ESP.getResetReason() == "Deep-Sleep Wake") {
    // wake_from_sleep();
  }
  else {
    // normal boot

    // Read saved settings from the config
    config_load_settings();
    /*
      #ifdef NTCENABLE true

        timeClient.setTimeOffset(time_offset);
      #endif
    */
    // Initialise the WiFi
    wifi_setup();

    // Bring up the web server
    web_server_setup();

    // Start the OTA update systems
    ota_setup();

    emondc_setup(); // HEY HEY!


    /*
      #ifdef NTCENABLE true
      timeClient.begin();
      #endif
    */


    DEBUG.println("Server started");

    if (RFM69_enabled) {
      delay(2000);
      DEBUG.println("2i");
      delay(500);
      DEBUG.println("4b");
      delay(500);
      DEBUG.println("210g");
      delay(500);
    }
  }

} // end setup

// -------------------------------------------------------------------
// LOOP
// -------------------------------------------------------------------
void loop() {
  //ESP.deepSleep(2000000, WAKE_RF_DEFAULT);
  //Serial.println(millis());
  //sleep_check();

  ota_loop();
  web_server_loop();
  wifi_loop();

  emondc_loop();
  //gpio0_loop();

  /*
    #ifdef NTCENABLE true
    if (WiFi.status() == WL_CONNECTED) {
      unsigned int currentMillisNTC = millis();
      if (currentMillisNTC - previousMillisNTC >= intervalNTC) {
        previousMillisNTC = currentMillisNTC;
        timeClient.update();
        Serial.print("testNTC:");
        Serial.println(timeClient.getFormattedTime());
        Serial.println(timeClient.getEpochTime());
        Serial.println();
        Serial.print("seconds on internal rtc:");
        Serial.println(millis() / 1000.0);
      }
    }
    #endif
  */
  String input = "";
  boolean gotInput = input_get(input);

  if (wifi_mode == WIFI_MODE_STA || wifi_mode == WIFI_MODE_AP_AND_STA) {
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
}
