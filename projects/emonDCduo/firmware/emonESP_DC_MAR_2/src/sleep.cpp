#include "sleep.h"
#include "emondc.h"

bool sleep_mode = false;

void sleep_check(void) {
  if (sleep_mode == true) {

  }
}

void wake_from_sleep(void) {
  // wake from sleep
  Serial.println("I'm awake!");
  emondc_setup();
  while (1) {
    emondc_loop();
    // if (averaging_loop_counter >= 100 || digitalRead(0) == LOW) {
    //post to emonCMS.
    //}
  }
}
