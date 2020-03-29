#include "emonesp.h"
#include "wifi.h"
#include "config.h"
#include "mqtt.h"
#include "gpio0.h"

int LEDpin = LED_BUILTIN;
unsigned long TimeButtonPressed;
unsigned long button_delay_AP = 6000; // ms for AP mode.
unsigned long button_delay_RST = 11000; // ms for factory reset.
bool buttonISRflag = false;
bool _button_flag_one = false;
bool _button_flag_two = false;

void gpio0_setup() {
  pinMode(0, INPUT);
  attachInterrupt(digitalPinToInterrupt(0), gpio0_isr, FALLING);
}

void ICACHE_RAM_ATTR gpio0_isr() { // ISR
  buttonISRflag = true;
}

void gpio0_loop() { // check the buttonflag and perform action
  // if button released, clear the flags.
  if (buttonISRflag && digitalRead(0)) { 
    Serial.println("Button Released");
    buttonISRflag = false;
    _button_flag_one = false;
    _button_flag_two = false;
    TimeButtonPressed = 0;
  }
  else if (buttonISRflag && TimeButtonPressed == 0) {
    TimeButtonPressed = millis();
  }
  else if (buttonISRflag) {
    if (!_button_flag_one) {
      Serial.println("Button pressed!");
      _button_flag_one = true;
    }
    else if (!_button_flag_two && TimeButtonPressed + button_delay_AP <= millis()) {
      Serial.println("Button held, AP mode!");
      led_flash(300, 300);
      led_flash(300, 300);
      Serial.println("AP mode starting..");
      wifi_mode = WIFI_MODE_AP_ONLY;
      startAP();
      _button_flag_two = true;
    }
    else if (TimeButtonPressed + button_delay_RST <= millis()) {
      Serial.println("Button held, Factor Reset!");
      led_flash(800, 800);
      Serial.println("Commencing factory reset.");
      delay(500);
      config_reset();
      Serial.println("Factory reset complete! Resetting...");
      led_flash(800, 800);
      led_flash(800, 800);
      led_flash(800, 800);
      ESP.restart();
    }
  } //buttonflag
} // end GPIO0 button.

void led_flash(int ton, int toff) {
  digitalWrite(LEDpin,LOW); delay(ton); digitalWrite(LEDpin,HIGH); delay(toff);
}