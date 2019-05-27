/*
 * ESP8266 Deep sleep mode example
 * Rui Santos 
 * Complete Project Details http://randomnerdtutorials.com
 * modified by DB, 2019.
 */
 
void setup() {
  Serial.begin(115200);
  
}

void loop() {
  // Deep sleep mode for n milliseconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
  Serial.println("I'm awake, but I'm going into deep sleep mode for 10 seconds");
  ESP.deepSleep(10e6); //10 seconds

  // Deep sleep mode until RESET pin is connected to a LOW signal (for example pushbutton or magnetic reed switch)
  //Serial.println("I'm awake, but I'm going into deep sleep mode until RESET pin is connected to a LOW signal");
//ESP.deepSleep(0); 
}
