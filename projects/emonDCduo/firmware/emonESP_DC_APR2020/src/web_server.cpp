/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
   All adaptation GNU General Public License as below.

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

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#define FS_NO_GLOBALS
//#include <FS.h>                       // SPIFFS file-system: store web server html, CSS etc.

#include "emonesp.h"
#include "web_server.h"
#include "web_server_static.h"
#include "config.h"
#include "wifi.h"
#include "mqtt.h"
#include "input.h"
#include "emoncms.h"
#include "ota.h"
#include "debug.h"
#include "emondc.h"

#include "./web_server_files/web_server.config_js.h"
#include "./web_server_files/web_server.home_html.h"
#include "./web_server_files/web_server.style_css.h"
#include "./web_server_files/web_server.lib_js.h"

AsyncWebServer server(80);          // Create class for Web server
AsyncWebSocket ws("/ws");
StaticFileWebHandler staticFile;

// Content Types
const char _CONTENT_TYPE_HTML[] PROGMEM = "text/html";
const char _CONTENT_TYPE_TEXT[] PROGMEM = "text/text";
const char _CONTENT_TYPE_CSS[] PROGMEM = "text/css";
//const char _CONTENT_TYPE_JSON[] PROGMEM = "application/json";
const char _CONTENT_TYPE_JS[] PROGMEM = "application/javascript";

bool enableCors = true;

// Event timeouts
unsigned long wifiRestartTime = 0;
unsigned long mqttRestartTime = 0;
unsigned long systemRestartTime = 0;
unsigned long systemRebootTime = 0;

// Get running firmware version from build tag environment variable
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String currentfirmware = ESCAPEQUOTE(BUILD_TAG);


void dumpRequest(AsyncWebServerRequest *request) {
  if(request->method() == HTTP_GET) {
    DBUGF("GET");
  } else if(request->method() == HTTP_POST) {
    DBUGF("POST");
  } else if(request->method() == HTTP_DELETE) {
    DBUGF("DELETE");
  } else if(request->method() == HTTP_PUT) {
    DBUGF("PUT");
  } else if(request->method() == HTTP_PATCH) {
    DBUGF("PATCH");
  } else if(request->method() == HTTP_HEAD) {
    DBUGF("HEAD");
  } else if(request->method() == HTTP_OPTIONS) {
    DBUGF("OPTIONS");
  } else {
    DBUGF("UNKNOWN");
  }
  DBUGF(" http://%s%s", request->host().c_str(), request->url().c_str());

  if(request->contentLength()){
    DBUGF("_CONTENT_TYPE: %s", request->contentType().c_str());
    DBUGF("_CONTENT_LENGTH: %u", request->contentLength());
  }

  int headers = request->headers();
  int i;
  for(i=0; i<headers; i++) {
    AsyncWebHeader* h = request->getHeader(i);
    DBUGF("_HEADER[%s]: %s", h->name().c_str(), h->value().c_str());
  }

  int params = request->params();
  for(i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    if(p->isFile()){
      DBUGF("_FILE[%s]: %s, size: %u", p->name().c_str(), p->value().c_str(), p->size());
    } else if(p->isPost()){
      DBUGF("_POST[%s]: %s", p->name().c_str(), p->value().c_str());
    } else {
      DBUGF("_GET[%s]: %s", p->name().c_str(), p->value().c_str());
    }
  }
}

// -------------------------------------------------------------------
// Helper function to perform the standard operations on a request
// -------------------------------------------------------------------

bool requestPreProcess(AsyncWebServerRequest *request, AsyncResponseStream *&response, const char *contentType = "application/json")
{
  if (www_username != "" && !request->authenticate(www_username.c_str(), www_password.c_str())) {
    request->requestAuthentication();
    return false;
  }

  response = request->beginResponseStream(contentType);
  if (enableCors) {
    response->addHeader("Access-Control-Allow-Origin", "*");
  }

  return true;
}


/*
// -------------------------------------------------------------------
// Load Home page
// url: /
// -------------------------------------------------------------------
//static const char index_html[] PROGMEM = "This is a string stored in flash";
void handleHome(AsyncWebServerRequest *request) {
  if (www_username != ""
      && !request->authenticate(www_username.c_str(),
                                www_password.c_str())
      && wifi_mode == WIFI_MODE_STA) {
    return request->requestAuthentication();
  }

  if (SPIFFS.exists("/home.html")) {
    //request->send(SPIFFS, "/home.html");
    request->send_P(200, "text/html", home_html); // this works!
  } else {
    request->send(200, "text/plain",
                  "/home.html not found, have you flashed the SPIFFS?");
  }
}
*/
// -------------------------------------------------------------------
// Wifi scan /scan not currently used
// url: /scan
//
// First request will return 0 results unless you start scan from somewhere else (loop/setup)
// Do not request more often than 3-5 seconds
// -------------------------------------------------------------------
void
handleScan(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response)) {
    return;
  }

  String json = "[";
  int n = WiFi.scanComplete();
  if (n == -2) {
    WiFi.scanNetworks(true);
  } else if (n) {
    for (int i = 0; i < n; ++i) {
      if (i) json += ",";
      json += "{";
      json += "\"rssi\":" + String(WiFi.RSSI(i));
      json += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
      json += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
      json += ",\"channel\":" + String(WiFi.channel(i));
      json += ",\"secure\":" + String(WiFi.encryptionType(i));
      json += ",\"hidden\":" + String(WiFi.isHidden(i) ? "true" : "false");
      json += "}";
    }
    WiFi.scanDelete();
    if (WiFi.scanComplete() == -2) {
      WiFi.scanNetworks(true);
    }
  }
  json += "]";
  request->send(200, "text/json", json);
}

// -------------------------------------------------------------------
// Handle turning Access point off
// url: /apoff
// -------------------------------------------------------------------
void
handleAPOff(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  response->setCode(200);
  response->print("Turning AP Off");
  request->send(response);

  DBUGLN("Turning AP Off");
  systemRebootTime = millis() + 1000;
}

// -------------------------------------------------------------------
// Save selected network to EEPROM and attempt connection
// url: /savenetwork
// -------------------------------------------------------------------
void
handleSaveNetwork(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  String qsid = request->arg("ssid");
  String qpass = request->arg("pass");

  if (qsid != 0) {
    config_save_wifi(qsid, qpass);

    response->setCode(200);
    response->print("saved");
    wifiRestartTime = millis() + 2000;
  } else {
    response->setCode(400);
    response->print("No SSID");
  }

  request->send(response);
}

// -------------------------------------------------------------------
// Save Emoncms
// url: /saveemoncms
// -------------------------------------------------------------------
void
handleSaveEmoncms(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  config_save_emoncms(request->arg("server"),
                      request->arg("path"),
                      request->arg("node"),
                      request->arg("apikey"),
                      request->arg("fingerprint"));

  char tmpStr[200];
  snprintf(tmpStr, sizeof(tmpStr), "Saved: %s %s %s %s %s",
           emoncms_server.c_str(),
           emoncms_path.c_str(),
           emoncms_node.c_str(),
           emoncms_apikey.c_str(),
           emoncms_fingerprint.c_str());
  DBUGLN(tmpStr);

  response->setCode(200);
  response->print(tmpStr);
  request->send(response);
}

// -------------------------------------------------------------------
// Save MQTT Config
// url: /savemqtt
// -------------------------------------------------------------------
void
handleSaveMqtt(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  config_save_mqtt(request->arg("server"),
                   request->arg("topic"),
                   request->arg("prefix"),
                   request->arg("user"),
                   request->arg("pass"));

  char tmpStr[200];
  snprintf(tmpStr, sizeof(tmpStr), "Saved: %s %s %s %s %s", mqtt_server.c_str(),
           mqtt_topic.c_str(), mqtt_feed_prefix.c_str(), mqtt_user.c_str(), mqtt_pass.c_str());
  DBUGLN(tmpStr);

  response->setCode(200);
  response->print(tmpStr);
  request->send(response);

  // If connected disconnect MQTT to trigger re-connect with new details
  mqttRestartTime = millis();
}
// -------------------------------------------------------------------
// Save the web site user/pass
// url: /saveadmin
// -------------------------------------------------------------------
void handleSaveAdmin(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  String quser = request->arg("user");
  String qpass = request->arg("pass");

  config_save_admin(quser, qpass);

  response->setCode(200);
  response->print("saved");
  request->send(response);
}


// -------------------------------------------------------------------
// Handle emonDC sampling setting
// url /emondc
// -------------------------------------------------------------------
void handleEmonDC(AsyncWebServerRequest *request) {
  // Serial.print("request start:"); Serial.println(millis());
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  String qinterval = request->arg("interval");
  String qvcalA = request->arg("vcalA");
  String qicalA = request->arg("icalA");
  String qvcalB = request->arg("vcalB");
  String qicalB = request->arg("icalB");
  String qchanA_VrefSet = request->arg("chanA_VrefSet");
  String qchanB_VrefSet = request->arg("chanB_VrefSet");
  String qchannelA_gain = request->arg("channelA_gain");
  String qchannelB_gain = request->arg("channelB_gain");
  String qR1_A = request->arg("R1_A");
  String qR2_A = request->arg("R2_A");
  String qR1_B = request->arg("R1_B");
  String qR2_B = request->arg("R2_B");
  String qRshunt_A = request->arg("Rshunt_A");
  String qRshunt_B = request->arg("Rshunt_B");
  String qAmpOffset_A = request->arg("AmpOffset_A");
  String qAmpOffset_B = request->arg("AmpOffset_B");
  String qVoltOffset_A = request->arg("VoltOffset_A");
  String qVoltOffset_B = request->arg("VoltOffset_B");

  config_save_emondc(qinterval, qicalA, qvcalA, qicalB, qvcalB, 
  qchanA_VrefSet, qchanB_VrefSet, qchannelA_gain, qchannelB_gain, 
  qR1_A, qR2_A, qR1_B, qR2_B, qRshunt_A, qRshunt_B, qAmpOffset_A, qAmpOffset_B, qVoltOffset_A, qVoltOffset_B);

  response->setCode(200);
  response->print("saved");
  request->send(response);
  // Serial.print("request end:"); Serial.println(millis());
}

// -------------------------------------------------------------------
// Last values on atmega serial
// url: /lastvalues
// -------------------------------------------------------------------
void handleLastValues(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  response->setCode(200);
  response->print(last_datastr);
  request->send(response);
}

// -------------------------------------------------------------------
// Returns status json
// url: /status
// -------------------------------------------------------------------
void
handleStatus(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response)) {
    return;
  }

  String s = "{";
  if (wifi_mode == WIFI_MODE_STA) {
    s += "\"mode\":\"STA\",";
  } else if (wifi_mode == WIFI_MODE_AP_STA_RETRY
             || wifi_mode == WIFI_MODE_AP_ONLY) {
    s += "\"mode\":\"AP\",";
  } else if (wifi_mode == WIFI_MODE_AP_AND_STA) {
    s += "\"mode\":\"STA+AP\",";
  }
  s += "\"networks\":[" + st + "],";
  s += "\"rssi\":[" + rssi + "],";

  s += "\"srssi\":\"" + String(WiFi.RSSI()) + "\",";
  s += "\"ipaddress\":\"" + ipaddress + "\",";
  s += "\"emoncms_connected\":\"" + String(emoncms_connected) + "\",";
  s += "\"packets_sent\":\"" + String(packets_sent) + "\",";
  s += "\"packets_success\":\"" + String(packets_success) + "\",";

  s += "\"mqtt_connected\":\"" + String(mqtt_connected()) + "\",";


  s += "\"free_heap\":\"" + String(ESP.getFreeHeap()) + "\"";

#ifdef ENABLE_LEGACY_API
  s += ",\"version\":\"" + currentfirmware + "\"";
  s += ",\"ssid\":\"" + esid + "\"";
  //s += ",\"pass\":\""+epass+"\""; security risk: DONT RETURN PASSWORDS
  s += ",\"emoncms_server\":\"" + emoncms_server + "\"";
  s += ",\"emoncms_path\":\"" + emoncms_path + "\"";
  s += ",\"emoncms_node\":\"" + emoncms_node + "\"";
  //s += ",\"emoncms_apikey\":\""+emoncms_apikey+"\""; security risk: DONT RETURN APIKEY
  s += ",\"emoncms_fingerprint\":\"" + emoncms_fingerprint + "\"";
  s += ",\"mqtt_server\":\"" + mqtt_server + "\"";
  s += ",\"mqtt_topic\":\"" + mqtt_topic + "\"";
  s += ",\"mqtt_user\":\"" + mqtt_user + "\"";
  //s += ",\"mqtt_pass\":\""+mqtt_pass+"\""; security risk: DONT RETURN PASSWORDS
  s += ",\"mqtt_feed_prefix\":\"" + mqtt_feed_prefix + "\"";
  s += ",\"www_username\":\"" + www_username + "\"";
  //s += ",\"www_password\":\""+www_password+"\""; security risk: DONT RETURN PASSWORDS

#endif
  s += "}";

  response->setCode(200);
  response->print(s);
  request->send(response);
}

// -------------------------------------------------------------------
// Returns OpenEVSE Config json
// url: /config
// -------------------------------------------------------------------
void
handleConfig(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response)) {
    return;
  }

  String s = "{";
  s += "\"espflash\":\"" + String(ESP.getFlashChipSize()) + "\",";
  s += "\"version\":\"" + currentfirmware + "\",";

  s += "\"ssid\":\"" + esid + "\",";
  //s += "\"pass\":\""+epass+"\","; security risk: DONT RETURN PASSWORDS
  s += "\"emoncms_server\":\"" + emoncms_server + "\",";
  s += "\"emoncms_path\":\"" + emoncms_path + "\",";
  s += "\"emoncms_node\":\"" + emoncms_node + "\",";
  // s += "\"emoncms_apikey\":\""+emoncms_apikey+"\","; security risk: DONT RETURN APIKEY
  s += "\"emoncms_fingerprint\":\"" + emoncms_fingerprint + "\",";
  s += "\"mqtt_server\":\"" + mqtt_server + "\",";
  s += "\"mqtt_topic\":\"" + mqtt_topic + "\",";
  s += "\"mqtt_feed_prefix\":\"" + mqtt_feed_prefix + "\",";
  s += "\"mqtt_user\":\"" + mqtt_user + "\",";
  //s += "\"mqtt_pass\":\""+mqtt_pass+"\","; security risk: DONT RETURN PASSWORDS
  s += "\"www_username\":\"" + www_username + "\",";
  //s += "\"www_password\":\""+www_password+"\","; security risk: DONT RETURN PASSWORDS
  s += "\"www_interval\":\"" + String(main_interval_seconds) + "\",";
  s += "\"icalA\":\"" + String(icalA,3) + "\",";
  s += "\"vcalA\":\"" + String(vcalA,3) + "\",";
  s += "\"icalB\":\"" + String(icalB,3) + "\",";
  s += "\"vcalB\":\"" + String(vcalB,3) + "\",";
  s += "\"chanA_VrefSet\":\"" + String(chanA_VrefSet) + "\",";
  s += "\"chanB_VrefSet\":\"" + String(chanB_VrefSet) + "\",";
  s += "\"channelA_gain\":\"" + String(channelA_gain) + "\",";
  s += "\"channelB_gain\":\"" + String(channelB_gain) + "\",";
  s += "\"R1_A\":\"" + String(R1_A) + "\",";
  s += "\"R2_A\":\"" + String(R2_A) + "\",";
  s += "\"R1_B\":\"" + String(R1_B) + "\",";
  s += "\"R2_B\":\"" + String(R2_B) + "\",";
  s += "\"Rshunt_A\":\"" + String(Rshunt_A,5) + "\",";
  s += "\"Rshunt_B\":\"" + String(Rshunt_B,5) + "\",";
  s += "\"AmpOffset_A\":\"" + String(AmpOffset_A) + "\",";
  s += "\"AmpOffset_B\":\"" + String(AmpOffset_B) + "\",";
  s += "\"VoltOffset_A\":\"" + String(VoltOffset_A) + "\",";
  s += "\"VoltOffset_B\":\"" + String(VoltOffset_B) + "\"";
  s += "}";

  response->setCode(200);
  response->print(s);
  request->send(response);
}

// -------------------------------------------------------------------
// Reset config and reboot
// url: /reset
// -------------------------------------------------------------------
void
handleRst(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  config_reset();
  ESP.eraseConfig();

  response->setCode(200);
  response->print("1");
  request->send(response);

  systemRebootTime = millis() + 1000;
}

// -------------------------------------------------------------------
// Restart (Reboot)
// url: /restart
// -------------------------------------------------------------------
void
handleRestart(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  response->setCode(200);
  response->print("1");
  request->send(response);

  systemRestartTime = millis() + 1000;
}

// -------------------------------------------------------------------
// Handle test input API
// url /input
// e.g http://192.168.0.75/input?string=CT1:3935,CT2:325,T1:12.5,T2:16.9,T3:11.2,T4:34.7
// -------------------------------------------------------------------
void
handleInput(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if (false == requestPreProcess(request, response, "text/plain")) {
    return;
  }

  input_string = request->arg("string");

  response->setCode(200);
  response->print(input_string);
  request->send(response);

  DBUGLN(input_string);
}

// -------------------------------------------------------------------
// Update firmware
// url: /update
// -------------------------------------------------------------------
void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index) {
    Serial.println("Update");
    Update.runAsync(true);
    if (!Update.begin(free_space)) {
      Update.printError(Serial);
    }
  }

  Serial.print(".");
  if (Update.write(data, len) != len) {
    Update.printError(Serial);
  }

  if (final) {
    if (!Update.end(true)) {
      Update.printError(Serial);
    } else {
      // restartNow = true; //Set flag so main loop can issue restart call
      Serial.println("Update complete");
      delay(1000);
      ESP.reset();
    }
  }
}





void handleNotFound(AsyncWebServerRequest *request)
{
  DBUG("NOT_FOUND: ");
  dumpRequest(request);

  if(wifi_mode == WIFI_MODE_AP_ONLY) {
    // Redirect to the home page in AP mode (for the captive portal)
    AsyncResponseStream *response = request->beginResponseStream(String(CONTENT_TYPE_HTML));

    String url = F("http://");
    url += ipaddress;

    String s = F("<html><body><a href=\"");
    s += url;
    s += F("\">OpenEVSE</a></body></html>");

    response->setCode(301);
    response->addHeader(F("Location"), url);
    response->print(s);
    request->send(response);
  } else {
    request->send(404);
  }
}


void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_CONNECT) {
    DBUGF("ws[%s][%u] connect", server->url(), client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT) {
    DBUGF("ws[%s][%u] disconnect: %u", server->url(), client->id());
  } else if(type == WS_EVT_ERROR) {
    DBUGF("ws[%s][%u] error(%u): %s", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG) {
    DBUGF("ws[%s][%u] pong[%u]: %s", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA) {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      DBUGF("ws[%s][%u] %s-message[%u]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", len);
    } else {
      // TODO: handle messages that are comprised of multiple frames or the frame is split into multiple packets
    }
  }
}

/*
// -------------------------------------------------------------------
// Update firmware
// url: /update
// -------------------------------------------------------------------
void
handleUpdateGet(AsyncWebServerRequest *request) {
  AsyncResponseStream *response;
  if(false == requestPreProcess(request, response, CONTENT_TYPE_HTML)) {
    return;
  }

  response->setCode(200);
  response->print(
    F("<html><form method='POST' action='/update' enctype='multipart/form-data'>"
        "<input type='file' name='firmware'> "
        "<input type='submit' value='Update'>"
      "</form></html>"));
  request->send(response);
}

void
handleUpdatePost(AsyncWebServerRequest *request) {
  bool shouldReboot = !Update.hasError();
  AsyncWebServerResponse *response = request->beginResponse(200, CONTENT_TYPE_TEXT, shouldReboot ? "OK" : "FAIL");
  response->addHeader("Connection", "close");
  request->send(response);

  if(shouldReboot) {
    delay(1000);
    ESP.reset();
    //systemRestartTime = millis() + 1000;
  }
}

extern "C" uint32_t _SPIFFS_start;
extern "C" uint32_t _SPIFFS_end;
static int lastPercent = -1;

void
handleUpdateUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if(!index)
  {
    dumpRequest(request);

    DBUGF("Update Start: %s", filename.c_str());
    Serial.println("Update Start.");

    // DBUGVAR(data[0]);
    //int command = data[0] == 0xE9 ? U_FLASH : U_SPIFFS;
    int command = U_FLASH;
    size_t updateSize = U_FLASH == command ?
      (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000 :
      ((size_t) &_SPIFFS_end - (size_t) &_SPIFFS_start);

    // DBUGVAR(command);
    // DBUGVAR(updateSize);

    // lcd_display(U_FLASH == command ? F("Updating WiFi") : F("Updating SPIFFS"), 0, 0, 0, LCD_CLEAR_LINE);
    // lcd_display(F(""), 0, 1, 10 * 1000, LCD_CLEAR_LINE);
    // lcd_loop();

    Update.runAsync(true);
    if(!Update.begin(updateSize, command)) {
#ifdef ENABLE_DEBUG
      Update.printError(DEBUG_PORT);
#endif
    }
  }
  if(!Update.hasError())
  {
    DBUGF("Update Writing %d", index);
    // String text = String(index);
    size_t contentLength = request->contentLength();
    // DBUGVAR(contentLength);
    if(contentLength > 0)
    {
      int percent = index / (contentLength / 100);
      // DBUGVAR(percent);
      // DBUGVAR(lastPercent);
      if(percent != lastPercent) {
       // String text = String(percent) + F("%");
       // lcd_display(text, 0, 1, 10 * 1000, LCD_DISPLAY_NOW);
        lastPercent = percent;
      }
    }
    if(Update.write(data, len) != len) {
#ifdef ENABLE_DEBUG
      Update.printError(DEBUG_PORT);
#endif
    }
  }
  if(final)
  {
    if(Update.end(true)) {
      //DBUGF("Update Success: %uB", index+len);
      //lcd_display(F("Complete"), 0, 1, 10 * 1000, LCD_CLEAR_LINE | LCD_DISPLAY_NOW);
    } else {
      //lcd_display(F("Error"), 0, 1, 10 * 1000, LCD_CLEAR_LINE | LCD_DISPLAY_NOW);
#ifdef ENABLE_DEBUG
      Update.printError(DEBUG_PORT);
#endif
    }
  }
}
*/

void
web_server_setup()
{
  // SPIFFS.begin(); // mount the fs

  // Setup the static files
  // server.serveStatic("/", SPIFFS, "/")
  // .setDefaultFile("index.html")
  // .setAuthentication(www_username.c_str(), www_password.c_str()); 

  // Add the Web Socket server
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.addHandler(&staticFile);

  // Start server & server root html /
  // server.on("/", handleHome);
  // Handle HTTP web interface button presses
  // server.on("/generate_204", handleHome);  //Android captive portal. Maybe not needed. Might be handled by notFound
  // server.on("/fwlink", handleHome);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound
  server.on("/status", handleStatus);
  server.on("/config", handleConfig);

  server.on("/savenetwork", handleSaveNetwork);
  server.on("/saveemoncms", handleSaveEmoncms);
  server.on("/savemqtt", handleSaveMqtt);
  server.on("/saveadmin", handleSaveAdmin);

  server.on("/reset", handleRst);
  server.on("/restart", handleRestart);

  server.on("/scan", handleScan);
  server.on("/apoff", handleAPOff);
  server.on("/input", handleInput);
  server.on("/lastvalues", handleLastValues);

  server.on("/savedc", handleEmonDC);

  // Simple Firmware Update Form
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  }, handle_update_progress_cb);

  // Simple Firmware Update Form
  //server.on("/upload", HTTP_GET, handleUpdateGet);
  //server.on("/update", HTTP_POST, handleUpdatePost, handleUpdateUpload);

  server.onNotFound(handleNotFound);
  //server.onNotFound(handleHome);
  
  server.begin();
}

void
web_server_loop() {
  // Do we need to restart the WiFi?
  if (wifiRestartTime > 0 && millis() > wifiRestartTime) {
    wifiRestartTime = 0;
    wifi_restart();
  }

  // Do we need to restart MQTT?
  if (mqttRestartTime > 0 && millis() > mqttRestartTime) {
    mqttRestartTime = 0;
    mqtt_restart();
  }

  // Do we need to restart the system?
  if (systemRestartTime > 0 && millis() > systemRestartTime) {
    systemRestartTime = 0;
    wifi_disconnect();
    ESP.restart();
  }

  // Do we need to reboot the system?
  if (systemRebootTime > 0 && millis() > systemRebootTime) {
    systemRebootTime = 0;
    wifi_disconnect();
    ESP.reset();
  }
}
