/*
 * -------------------------------------------------------------------
 * EmonESP Serial to Emoncms gateway
 * -------------------------------------------------------------------
 * Adaptation of Chris Howells OpenEVSE ESP Wifi
 * by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
 * All adaptation GNU General Public License as below.
 *
 * -------------------------------------------------------------------
 *
 * This file is part of OpenEnergyMonitor.org project.
 * EmonESP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * EmonESP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with EmonESP; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "emonesp.h"
#include "config.h"
#include "emondc.h"

#include <Arduino.h>
#include <EEPROM.h>                   // Save config settings
#include <String.h>


// Wifi Network Strings
String esid = "d4machine";
String epass = "";

// Web server authentication (leave blank for none)
String www_username = "";
String www_password = "";

// EMONCMS SERVER strings
String emoncms_server = "";
String emoncms_path = "";
String emoncms_node = "";
String emoncms_apikey = "";
String emoncms_fingerprint = "";

// MQTT Settings
String mqtt_server = "";
String mqtt_topic = "";
String mqtt_user = "";
String mqtt_pass = "";
String mqtt_feed_prefix = "";


#define EEPROM_ESID_SIZE          32
#define EEPROM_EPASS_SIZE         64
#define EEPROM_EMON_API_KEY_SIZE  32
#define EEPROM_EMON_SERVER_SIZE   45
#define EEPROM_EMON_PATH_SIZE     16
#define EEPROM_EMON_NODE_SIZE     32
#define EEPROM_MQTT_SERVER_SIZE   45
#define EEPROM_MQTT_TOPIC_SIZE    32
#define EEPROM_MQTT_USER_SIZE     32
#define EEPROM_MQTT_PASS_SIZE     64
#define EEPROM_EMON_FINGERPRINT_SIZE  60
#define EEPROM_MQTT_FEED_PREFIX_SIZE  10
#define EEPROM_WWW_USER_SIZE      16
#define EEPROM_WWW_PASS_SIZE      16
/* EMONDC SETTINGS: */
#define EEPROM_INTERVAL_SIZE      2
#define EEPROM_CHAN_A_SIZE        1
#define EEPROM_CHAN_B_SIZE        1
#define EEPROM_GAIN_A_SIZE        2
#define EEPROM_GAIN_B_SIZE        2
#define EEPROM_R1_A_SIZE          4
#define EEPROM_R2_A_SIZE          4
#define EEPROM_R1_B_SIZE          4
#define EEPROM_R2_B_SIZE          4
#define EEPROM_SHUNT_A_SIZE       8
#define EEPROM_SHUNT_B_SIZE       8
#define EEPROM_VCALA_SIZE         8
#define EEPROM_ICALA_SIZE         8
#define EEPROM_VCALB_SIZE         8
#define EEPROM_ICALB_SIZE         8
#define EEPROM_SIG_SIZE           4

#define EEPROM_SIZE               1024



#define EEPROM_ESID_START         0
#define EEPROM_ESID_END           (EEPROM_ESID_START + EEPROM_ESID_SIZE)
#define EEPROM_EPASS_START        EEPROM_ESID_END
#define EEPROM_EPASS_END          (EEPROM_EPASS_START + EEPROM_EPASS_SIZE)
#define EEPROM_EMON_API_KEY_START EEPROM_EPASS_END
#define EEPROM_EMON_API_KEY_END   (EEPROM_EMON_API_KEY_START + EEPROM_EMON_API_KEY_SIZE)
#define EEPROM_EMON_SERVER_START  EEPROM_EMON_API_KEY_END
#define EEPROM_EMON_SERVER_END    (EEPROM_EMON_SERVER_START + EEPROM_EMON_SERVER_SIZE)
#define EEPROM_EMON_NODE_START    EEPROM_EMON_SERVER_END
#define EEPROM_EMON_NODE_END      (EEPROM_EMON_NODE_START + EEPROM_EMON_NODE_SIZE)
#define EEPROM_MQTT_SERVER_START  EEPROM_EMON_NODE_END
#define EEPROM_MQTT_SERVER_END    (EEPROM_MQTT_SERVER_START + EEPROM_MQTT_SERVER_SIZE)
#define EEPROM_MQTT_TOPIC_START   EEPROM_MQTT_SERVER_END
#define EEPROM_MQTT_TOPIC_END     (EEPROM_MQTT_TOPIC_START + EEPROM_MQTT_TOPIC_SIZE)
#define EEPROM_MQTT_USER_START    EEPROM_MQTT_TOPIC_END
#define EEPROM_MQTT_USER_END      (EEPROM_MQTT_USER_START + EEPROM_MQTT_USER_SIZE)
#define EEPROM_MQTT_PASS_START    EEPROM_MQTT_USER_END
#define EEPROM_MQTT_PASS_END      (EEPROM_MQTT_PASS_START + EEPROM_MQTT_PASS_SIZE)
#define EEPROM_EMON_FINGERPRINT_START  EEPROM_MQTT_PASS_END
#define EEPROM_EMON_FINGERPRINT_END    (EEPROM_EMON_FINGERPRINT_START + EEPROM_EMON_FINGERPRINT_SIZE)
#define EEPROM_MQTT_FEED_PREFIX_START  EEPROM_EMON_FINGERPRINT_END
#define EEPROM_MQTT_FEED_PREFIX_END    (EEPROM_MQTT_FEED_PREFIX_START + EEPROM_MQTT_FEED_PREFIX_SIZE)
#define EEPROM_WWW_USER_START     EEPROM_MQTT_FEED_PREFIX_END
#define EEPROM_WWW_USER_END       (EEPROM_WWW_USER_START + EEPROM_WWW_USER_SIZE)
#define EEPROM_WWW_PASS_START     EEPROM_WWW_USER_END
#define EEPROM_WWW_PASS_END       (EEPROM_WWW_PASS_START + EEPROM_WWW_PASS_SIZE)
#define EEPROM_EMON_PATH_START    EEPROM_WWW_PASS_END
#define EEPROM_EMON_PATH_END      (EEPROM_EMON_PATH_START + EEPROM_EMON_PATH_SIZE)
/* EMONDC SETTINGS: */
#define EEPROM_INTERVAL_START     EEPROM_EMON_PATH_END
#define EEPROM_INTERVAL_END       (EEPROM_INTERVAL_START + EEPROM_INTERVAL_SIZE)
#define EEPROM_CHAN_A_START       EEPROM_INTERVAL_END
#define EEPROM_CHAN_A_END         (EEPROM_CHAN_A_START + EEPROM_CHAN_A_SIZE)
#define EEPROM_CHAN_B_START       EEPROM_CHAN_A_END
#define EEPROM_CHAN_B_END         (EEPROM_CHAN_B_START + EEPROM_CHAN_B_SIZE)
#define EEPROM_GAIN_A_START       EEPROM_CHAN_B_END
#define EEPROM_GAIN_A_END         (EEPROM_GAIN_A_START + EEPROM_GAIN_B_SIZE)
#define EEPROM_GAIN_B_START       EEPROM_GAIN_A_END
#define EEPROM_GAIN_B_END         (EEPROM_GAIN_B_START + EEPROM_GAIN_B_SIZE)
#define EEPROM_R1_A_START         EEPROM_GAIN_B_END
#define EEPROM_R1_A_END           (EEPROM_R1_A_START + EEPROM_R1_A_SIZE)
#define EEPROM_R1_B_START         EEPROM_R1_A_END
#define EEPROM_R1_B_END           (EEPROM_R1_B_START + EEPROM_R2_A_SIZE)
#define EEPROM_R2_A_START         EEPROM_R1_B_END
#define EEPROM_R2_A_END           (EEPROM_R2_A_START + EEPROM_R1_B_SIZE)
#define EEPROM_R2_B_START         EEPROM_R2_A_END
#define EEPROM_R2_B_END           (EEPROM_R2_B_START + EEPROM_R2_B_SIZE)
#define EEPROM_SHUNT_A_START      EEPROM_R2_B_END
#define EEPROM_SHUNT_A_END        (EEPROM_SHUNT_A_START + EEPROM_SHUNT_A_SIZE)
#define EEPROM_SHUNT_B_START      EEPROM_SHUNT_A_END
#define EEPROM_SHUNT_B_END        (EEPROM_SHUNT_B_START + EEPROM_SHUNT_B_SIZE)
#define EEPROM_VCALA_START        EEPROM_SHUNT_B_END
#define EEPROM_VCALA_END          (EEPROM_VCALA_START + EEPROM_VCALA_SIZE)
#define EEPROM_ICALA_START        EEPROM_VCALA_END
#define EEPROM_ICALA_END          (EEPROM_ICALA_START + EEPROM_ICALA_SIZE)
#define EEPROM_VCALB_START        EEPROM_ICALA_END
#define EEPROM_VCALB_END          (EEPROM_VCALB_START + EEPROM_VCALB_SIZE)
#define EEPROM_ICALB_START        EEPROM_VCALB_END
#define EEPROM_ICALB_END          (EEPROM_ICALB_START + EEPROM_ICALB_SIZE)
#define EEPROM_SIG_START          EEPROM_ICALB_END
#define EEPROM_SIG_END            (EEPROM_SIG_START + EEPROM_SIG_SIZE)

// -------------------------------------------------------------------
// Reset EEPROM, wipes all settings
// -------------------------------------------------------------------
void ResetEEPROM(){
  //DEBUG.println("Erasing EEPROM");
  for (int i = 0; i < EEPROM_SIZE; ++i) {
    EEPROM.write(i, 0);
    //DEBUG.print("#");
  }
  EEPROM.commit();
}

void EEPROM_read_string(int start, int count, String & val) {
  for (int i = 0; i < count; ++i){
    byte c = EEPROM.read(start+i);
    if (c != 0 && c != 255)
      val += (char) c;
  }
}

// https://stackoverflow.com/questions/3991478/building-a-32-bit-float-out-of-its-4-composite-bytes
void EEPROM_read_int(int start, int & val) {
  byte b[2];
  b[0] = EEPROM.read(start+0);
  b[1] = EEPROM.read(start+1);
  memcpy(&val, &b, 2);
}

void EEPROM_read_long(int start, long & val) {
  byte b[4];
  b[0] = EEPROM.read(start+0);
  b[1] = EEPROM.read(start+1);
  b[2] = EEPROM.read(start+2);
  b[3] = EEPROM.read(start+3);
  memcpy(&val, &b, 4);
}

void EEPROM_read_double(int start, double & val) {
  byte b[8];
  b[0] = EEPROM.read(start+0);
  b[1] = EEPROM.read(start+1);
  b[2] = EEPROM.read(start+2);
  b[3] = EEPROM.read(start+3);
  b[4] = EEPROM.read(start+4);
  b[5] = EEPROM.read(start+5);
  b[6] = EEPROM.read(start+6);
  b[7] = EEPROM.read(start+7);
  memcpy(&val, &b, 8);
}






 /*
void int2Bytes(int val, byte & bytes_array){ 
  byte *pb = &bytes_array;
  pb[0] = (int)((val >> 8) & 0XFF);
  pb[1] = (int)((val & 0XFF));
}

void long2Bytes(long val, byte & bytes_array) {
  byte *pb = &bytes_array;
  pb[0] = (int)((val >> 24) & 0xFF) ;
  pb[1] = (int)((val >> 16) & 0xFF) ;
  pb[2] = (int)((val >> 8) & 0XFF);
  pb[3] = (int)((val & 0XFF));
  
  // Create union of shared memory space
  union {
    long variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with variable
  u.variable = val;
  // Assign bytes to input array
  memcpy(&bytes_array, u.temp_array, 4);
  */


void EEPROM_write_string(int start, unsigned int count, String val) {
  for (unsigned int i = 0; i < count; ++i) {
    if (i<val.length()) {
      EEPROM.write(start+i, val[i]);
    } else {
      EEPROM.write(start+i, 0);
    }
  }
}


void EEPROM_write_int(int start, unsigned int count, int val) {
  // https://stackoverflow.com/questions/24420246/c-function-to-convert-float-to-byte-array
  //byte bytes[2];
  //int2Bytes(val, bytes[0]);
    union {
    int variable;
    byte bytes[2];
  } u;
  // Overite bytes of union with variable
  u.variable = val;
  for (unsigned int i = 0; i < count; ++i) {
    EEPROM.write(start+i, u.bytes[i]);
  }
}
 
void EEPROM_write_long(int start, unsigned int count, long val) {
  //byte bytes[4];
  union {
    long variable;
    byte bytes[4];
  } u;
  u.variable = val;
  //long2Bytes(val, bytes[0]);
  for (unsigned int i = 0; i < count; ++i) {
    EEPROM.write(start+i, u.bytes[i]);
  }
}

void EEPROM_write_double(int start, unsigned int count, double val) { // https://stackoverflow.com/questions/24420246/c-function-to-convert-float-to-byte-array
  //byte bytes[8]; 
  union {
    double variable;
    byte bytes[8];
  } u;
  // Overite bytes of union with variable
  u.variable = val;
  // Assign bytes to input array
  for (unsigned int i = 0; i < count; ++i) {
    EEPROM.write(start+i, u.bytes[i]);
  }
}



// -------------------------------------------------------------------
// Load saved settings from EEPROM
// -------------------------------------------------------------------
void config_load_settings()
{
  EEPROM.begin(EEPROM_SIZE);
  

  // Load WiFi values
  EEPROM_read_string(EEPROM_ESID_START, EEPROM_ESID_SIZE, esid);
  EEPROM_read_string(EEPROM_EPASS_START, EEPROM_EPASS_SIZE, epass);

  // EmonCMS settings
  EEPROM_read_string(EEPROM_EMON_API_KEY_START, EEPROM_EMON_API_KEY_SIZE, emoncms_apikey);
  EEPROM_read_string(EEPROM_EMON_SERVER_START, EEPROM_EMON_SERVER_SIZE, emoncms_server);
  EEPROM_read_string(EEPROM_EMON_PATH_START, EEPROM_EMON_PATH_SIZE, emoncms_path);
  EEPROM_read_string(EEPROM_EMON_NODE_START, EEPROM_EMON_NODE_SIZE, emoncms_node);
  EEPROM_read_string(EEPROM_EMON_FINGERPRINT_START, EEPROM_EMON_FINGERPRINT_SIZE, emoncms_fingerprint);

  // MQTT settings
  EEPROM_read_string(EEPROM_MQTT_SERVER_START, EEPROM_MQTT_SERVER_SIZE, mqtt_server);
  EEPROM_read_string(EEPROM_MQTT_TOPIC_START, EEPROM_MQTT_TOPIC_SIZE, mqtt_topic);
  EEPROM_read_string(EEPROM_MQTT_FEED_PREFIX_START, EEPROM_MQTT_FEED_PREFIX_SIZE, mqtt_feed_prefix);
  EEPROM_read_string(EEPROM_MQTT_USER_START, EEPROM_MQTT_USER_SIZE, mqtt_user);
  EEPROM_read_string(EEPROM_MQTT_PASS_START, EEPROM_MQTT_PASS_SIZE, mqtt_pass);

  //----------------------
  // emonDC settings
  //----------------------
  char sig[4];
  sig[0] = EEPROM.read(EEPROM_SIG_START);
  sig[1] = EEPROM.read(EEPROM_SIG_START+1);
  sig[2] = EEPROM.read(EEPROM_SIG_START+2);
  sig[3] = EEPROM.read(EEPROM_SIG_START+3);
  //EEPROM_read_string(EEPROM_SIG_START, EEPROM_SIG_SIZE, sig);
  Serial.print("signature:");Serial.println(sig);
  if (!strcmp(sig, "OEM")) ; // to stop fault while dev'ing
  else return;
  Serial.println("emonDC Settings from Flash:");
  EEPROM_read_int(EEPROM_INTERVAL_START, main_interval_seconds);
  Serial.print("main_interval_seconds:"); Serial.println(main_interval_seconds);
  EEPROM_read_int(EEPROM_GAIN_A_START, channelA_gain);
  Serial.print("channelA_gain:"); Serial.println(channelA_gain);
  EEPROM_read_int(EEPROM_GAIN_B_START, channelB_gain);
  Serial.print("channelB_gain:"); Serial.println(channelB_gain);
  EEPROM_read_long(EEPROM_R1_A_START, R1_A);
  Serial.print("R1_A:"); Serial.println(R1_A);
  EEPROM_read_long(EEPROM_R2_A_START, R2_A);
  Serial.print("R2_A:"); Serial.println(R2_A);
  EEPROM_read_long(EEPROM_R1_B_START, R1_B);
  Serial.print("R1_B:"); Serial.println(R1_B);
  EEPROM_read_long(EEPROM_R2_B_START, R2_B);
  Serial.print("R2_B:"); Serial.println(R2_B);


  chanA_VrefSet = bitRead(EEPROM.read(EEPROM_CHAN_A_START), 0);
  Serial.print("chanA_VrefSet:"); Serial.println(chanA_VrefSet);
  chanB_VrefSet = bitRead(EEPROM.read(EEPROM_CHAN_B_START), 0);
  Serial.print("chanB_VrefSet:"); Serial.println(chanB_VrefSet);


  EEPROM_read_double(EEPROM_SHUNT_A_START, Rshunt_A);
  Serial.print("Rshunt_A:"); Serial.println(Rshunt_A,5);
  EEPROM_read_double(EEPROM_SHUNT_B_START, Rshunt_B);
  Serial.print("Rshunt_B:"); Serial.println(Rshunt_B,5);
  EEPROM_read_double(EEPROM_ICALA_START, icalA);
  Serial.print("icalA:"); Serial.println(icalA);
  EEPROM_read_double(EEPROM_VCALA_START, vcalA);
  Serial.print("vcalA:"); Serial.println(vcalA);
  EEPROM_read_double(EEPROM_ICALB_START, icalB);
  Serial.print("icalB:"); Serial.println(icalB);
  EEPROM_read_double(EEPROM_ICALB_START, vcalB);
  Serial.print("vcalB:"); Serial.println(vcalB);
}

void config_save_emoncms(String server, String path, String node, String apikey, String fingerprint)
{
  emoncms_server = server;
  emoncms_path = path;
  emoncms_node = node;
  emoncms_apikey = apikey;
  emoncms_fingerprint = fingerprint;

  // save apikey to EEPROM
  EEPROM_write_string(EEPROM_EMON_API_KEY_START, EEPROM_EMON_API_KEY_SIZE, emoncms_apikey);

  // save emoncms server to EEPROM max 45 characters
  EEPROM_write_string(EEPROM_EMON_SERVER_START, EEPROM_EMON_SERVER_SIZE, emoncms_server);

  // save emoncms server to EEPROM max 16 characters
  EEPROM_write_string(EEPROM_EMON_PATH_START, EEPROM_EMON_PATH_SIZE, emoncms_path);

  // save emoncms node to EEPROM max 32 characters
  EEPROM_write_string(EEPROM_EMON_NODE_START, EEPROM_EMON_NODE_SIZE, emoncms_node);

  // save emoncms HTTPS fingerprint to EEPROM max 60 characters
  EEPROM_write_string(EEPROM_EMON_FINGERPRINT_START, EEPROM_EMON_FINGERPRINT_SIZE, emoncms_fingerprint);

  EEPROM.commit();
}

void config_save_mqtt(String server, String topic, String prefix, String user, String pass)
{
  mqtt_server = server;
  mqtt_topic = topic;
  mqtt_feed_prefix = prefix;
  mqtt_user = user;
  mqtt_pass = pass;

  // Save MQTT server max 45 characters
  EEPROM_write_string(EEPROM_MQTT_SERVER_START, EEPROM_MQTT_SERVER_SIZE, mqtt_server);

  // Save MQTT topic max 32 characters
  EEPROM_write_string(EEPROM_MQTT_TOPIC_START, EEPROM_MQTT_TOPIC_SIZE, mqtt_topic);

  // Save MQTT topic separator max 10 characters
  EEPROM_write_string(EEPROM_MQTT_FEED_PREFIX_START, EEPROM_MQTT_FEED_PREFIX_SIZE, mqtt_feed_prefix);

  // Save MQTT username max 32 characters
  EEPROM_write_string(EEPROM_MQTT_USER_START, EEPROM_MQTT_USER_SIZE, mqtt_user);

  // Save MQTT pass max 64 characters
  EEPROM_write_string(EEPROM_MQTT_PASS_START, EEPROM_MQTT_PASS_SIZE, mqtt_pass);

  EEPROM.commit();
}

void config_save_admin(String user, String pass)
{
  www_username = user;
  www_password = pass;

  EEPROM_write_string(EEPROM_WWW_USER_START, EEPROM_WWW_USER_SIZE, user);
  EEPROM_write_string(EEPROM_WWW_PASS_START, EEPROM_WWW_PASS_SIZE, pass);

  EEPROM.commit();
}

void config_save_wifi(String qsid, String qpass)
{
  esid = qsid;
  epass = qpass;

  EEPROM_write_string(EEPROM_ESID_START, EEPROM_ESID_SIZE, qsid);
  EEPROM_write_string(EEPROM_EPASS_START, EEPROM_EPASS_SIZE, qpass);

  EEPROM.commit();
}

void config_save_emondc(String qinterval, String qicalA, String qvcalA, String qicalB, String qvcalB)
{
  char char_array[10]; 
  strcpy(char_array, qinterval.c_str());
  main_interval_seconds = atoi(char_array);
  main_interval_ms = main_interval_seconds*1000; // update for loop.
  strcpy(char_array, qicalA.c_str());
  icalA = atoi(char_array);
  strcpy(char_array, qvcalA.c_str());
  vcalA = atoi(char_array);
  strcpy(char_array, qicalB.c_str());
  icalB = atof(char_array);
  strcpy(char_array, qvcalB.c_str());
  vcalB = atof(char_array);

  // save web_server entered values to flash:
  EEPROM_write_int(EEPROM_INTERVAL_START, EEPROM_INTERVAL_SIZE, main_interval_seconds);
  EEPROM_write_double(EEPROM_ICALA_START, EEPROM_ICALA_SIZE, icalA);
  EEPROM_write_double(EEPROM_VCALA_START, EEPROM_VCALA_SIZE, vcalA);
  EEPROM_write_double(EEPROM_ICALB_START, EEPROM_ICALB_SIZE, icalB);
  EEPROM_write_double(EEPROM_VCALB_START, EEPROM_ICALB_SIZE, vcalB);

  
  // wip:
  chanA_VrefSet = 1; 
  chanB_VrefSet = 1; 
  channelA_gain = 100;
  channelB_gain = 100;
  R1_A = 1000000;
  R2_A = 75000;
  R1_B = 1000000;
  R2_B = 75000;
  Rshunt_A = 0.0005;
  Rshunt_B = 0.0005;
  byte tb;  bitWrite(tb, 0, 1);
  EEPROM.write(EEPROM_CHAN_A_START, tb);
  EEPROM.write(EEPROM_CHAN_B_START, tb);
  EEPROM_write_int(EEPROM_GAIN_A_START, EEPROM_GAIN_B_SIZE, channelA_gain);
  EEPROM_write_int(EEPROM_GAIN_B_START, EEPROM_GAIN_B_SIZE, channelB_gain);
  EEPROM_write_long(EEPROM_R1_A_START, EEPROM_R1_A_SIZE, R1_A);
  EEPROM_write_long(EEPROM_R2_A_START, EEPROM_R1_A_SIZE, R2_A);
  EEPROM_write_long(EEPROM_R1_B_START, EEPROM_R1_A_SIZE, R1_B);
  EEPROM_write_long(EEPROM_R2_B_START, EEPROM_R1_A_SIZE, R2_B);
  EEPROM_write_double(EEPROM_SHUNT_A_START, EEPROM_SHUNT_A_SIZE, Rshunt_A);
  EEPROM_write_double(EEPROM_SHUNT_B_START, EEPROM_SHUNT_A_SIZE, Rshunt_B);

  // using signature for init.
  EEPROM_write_string(EEPROM_SIG_START, EEPROM_SIG_SIZE, F("OEM"));

  Serial.println("emondc_save_config done.");

  EEPROM.commit();
}

void config_reset()
{
  ResetEEPROM();
}


