// Energy Monitoring only example for heatpump monitor board.
// Licence: GPLv3
#include <avr/wdt.h>

#define FirmwareVersion = 1.0
#define DEBUG 0
#define emonTxV3 1
#define PULSE_ENABLE 0

#include "EmonLib.h"           // Include Emon Library:  https://github.com/openenergymonitor/EmonLib
EnergyMonitor shuntmonA;       // Create an instance
EnergyMonitor shuntmonB;       // Create an instance

unsigned long last = 0;
unsigned long now = 0;
unsigned long lastwdtreset = 0;

// pulseCounting
long pulseCount = 0;

int joules_SHUNTMONA = 0;
int joules_SHUNTMONB = 0;
unsigned long SHUNTMONA_Wh = 0;
unsigned long SHUNTMONB_Wh = 0;
unsigned long msgnum = 0;

bool firstrun = true;
unsigned long last_reading = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Startup");

  pinMode(6, OUTPUT);

  shuntmonA.voltage(4, 262.0, 1.7);
  shuntmonA.current(0, 90.9);
  shuntmonB.voltage(5, 262.0, 1.7);
  shuntmonB.current(1, 90.9);
  delay(100);
  if (PULSE_ENABLE) attachInterrupt(1, onPulse, FALLING);

  SHUNTMONA_Wh = 0;
  SHUNTMONB_Wh = 0;

  wdt_enable(WDTO_8S);
  wdt_reset();

}

void loop() {
  now = millis();

  if ((now - last) >= 9800 || firstrun) {
    wdt_reset();
    last = now; firstrun = false;

    // Reading of sensors needs to go here for stability
    // need to double check the reason for this.
    for (int i = 0; i < 10; i++) {
      analogRead(0); analogRead(1); analogRead(4); analogRead(5);
    }
    delay(200);
    shuntmonA.calcVI(30, 2000);
    wdt_reset();
    int OEMshuntmonA = shuntmonA.realPower;
    shuntmonB.calcVI(30, 2000);
    wdt_reset();
    int OEMshuntmonB = shuntmonB.realPower;

    // Accumulating Watt hours
    int interval = millis() - last_reading;
    last_reading = millis();
    
    if (shuntmonA.realPower > 0 && interval>0) {
      unsigned long jouleinc = shuntmonA.realPower * interval *0.001;
      joules_SHUNTMONA += jouleinc;
      SHUNTMONA_Wh += joules_SHUNTMONA / 3600;
      joules_SHUNTMONA = joules_SHUNTMONA % 3600;
    }

    if (shuntmonB.realPower > 0 && interval>0) {
      unsigned long jouleinc = shuntmonB.realPower * interval *0.001;
      joules_SHUNTMONB += jouleinc;
      SHUNTMONB_Wh += joules_SHUNTMONB / 3600;
      joules_SHUNTMONB = joules_SHUNTMONB % 3600;
    }
    wdt_reset();

    msgnum++;
    Serial.print("Msg:"); Serial.print(msgnum);
    Serial.print(",OEMshuntmonA:"); Serial.print(OEMshuntmonA);
    Serial.print(",OEMshuntmonB:"); Serial.print(OEMshuntmonB);
    Serial.print(",OEMshuntmonAWh:"); Serial.print(SHUNTMONA_Wh);
    Serial.print(",OEMshuntmonBWh:"); Serial.print(SHUNTMONB_Wh);
    if (PULSE_ENABLE) {
      Serial.print(",PulseCount:"); Serial.print(pulseCount);
    }
    Serial.println();
    digitalWrite(6, HIGH);
    delay(200);
    digitalWrite(6, LOW);
  }

  if ((millis() - lastwdtreset) > 1000) {
    lastwdtreset = millis();
    wdt_reset();
  }
}

// The interrupt routine - runs each time a falling edge of a pulse is detected
void onPulse()
{
  pulseCount++;                                               // count pulse
}
