#include "emonesp.h"
#include "wifi.h"
#include "config.h"
#include "mqtt.h"
#include "gpio0.h"
#include "emondc.h"

bool initGPIO0 = 0;
int LEDpin = LED_BUILTIN;
unsigned long TimeButtonPressed = 0;
unsigned long TimeButtonPressedExt;
int debounce = 555;                     // millis debounce
unsigned long button_delay_AP = 6000;   // ms for AP mode.
unsigned long button_delay_RST = 11000; // ms for factory reset.
bool buttonISRflag = false;
bool _button_flag_one = false;
bool _button_flag_two = false;

//bool LCD_trigger = false;

void gpio0_setup()
{
  pinMode(0, INPUT);
  attachInterrupt(digitalPinToInterrupt(0), gpio0_isr, FALLING); // going LOW is button pressed
}

void ICACHE_RAM_ATTR gpio0_isr()
{ // ISR
  buttonISRflag = true;
  //Serial.print("BUTTON!");
}

void gpio0_loop()
{ // check the buttonflag and perform action

  if (buttonISRflag && TimeButtonPressed == 0) {
    TimeButtonPressed = millis(); TimeButtonPressedExt = TimeButtonPressed;
  }

  if (buttonISRflag && !_button_flag_one && !digitalRead(0))
  {
    Serial.println("Button pressed!");
    Serial.println(TimeButtonPressed);
    _button_flag_one = true;
    oled_button_flag = true;
    if (OLED_active) screentog++;
    if (screentog > max_screentog) screentog = 0; // reset screen page number.
    OLED_active = true; oled_interval = 1000;
    draw_OLED();
  }
  else if (buttonISRflag && !_button_flag_two && millis() >= TimeButtonPressed + button_delay_AP)
  {
    Serial.println("Button held, AP mode!");
    Serial.println(TimeButtonPressed);
    led_flash(800, 800);
    led_flash(800, 800);
    led_flash(800, 800);
    Serial.println("AP mode starting..");
    wifi_mode = WIFI_MODE_AP_ONLY;
    startAP();
    _button_flag_two = true;
  }
  else if (buttonISRflag && _button_flag_two && millis() >= TimeButtonPressed + button_delay_RST)
  {
    Serial.println("Button held, Factory Reset!");
    Serial.println(TimeButtonPressed);
    led_flash(800, 800);
    Serial.println("Commencing factory reset.");
    delay(500);
    config_reset();
    Serial.println("Factory reset complete! Resetting...");
    led_flash(800, 800);
    led_flash(800, 800);
    led_flash(800, 800);
    ESP.restart();
  } //buttonflag
 

  // if button released, clear the flags.
  if (buttonISRflag && digitalRead(0) && millis() >= TimeButtonPressed + debounce)
  { 
    Serial.println("Button Released");
    buttonISRflag = false;
    _button_flag_one = false;
    _button_flag_two = false;
    TimeButtonPressed = 0;
  }

} // end GPIO0 button.

void led_flash(int ton, int toff)
{
  digitalWrite(LEDpin, LOW);
  delay(ton);
  digitalWrite(LEDpin, HIGH);
  delay(toff);
}