#include <SPI.h>

#define CS_3202 15

int channel0;
int channel1;
int channel0accumulator;
int channel1accumulator;
int numberofsamples;
int channel0avgA;
int channel1avgA;
double Whaccumulator;

int numberofseconds = 1;

const double ADCsteps = 4096.00;

// Current calibration constants
const double supplyvoltage = 5.00; //voltage dictating Vref for ADC
const double shunt_resistance = 0.00050;
const double shunt_monitor_gain = 100.00;

// Voltage calibration constants
const double Vdiv = 17.949152542;

unsigned long previousMillis = 0;        // will store last time updated

// constants won't change:
const long interval = 1000;           // interval at which to log (milliseconds)

void setup()
{
  Serial.begin(115200);
  pinMode(CS_3202, OUTPUT);
  digitalWrite(CS_3202, HIGH);
}

void loop()
{
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

    double step1currentAcalibrated = channel0avgA / 4096.00;
    double step2currentAcalibrated = step1currentAcalibrated * supplyvoltage;
    double step3currentAcalibrated = step2currentAcalibrated / shunt_monitor_gain;
    double step4currentAcalibrated = step3currentAcalibrated / shunt_resistance;

    double step1voltageAcalibrated = channel1avgA / 4096.00;
    double step2voltageAcalibrated = step1voltageAcalibrated * supplyvoltage;
    double step3voltageAcalibrated = step2voltageAcalibrated * Vdiv;

    double powerA = step4currentAcalibrated * step3voltageAcalibrated;
    Whaccumulator = Whaccumulator + powerA;

    Serial.print("channel0avgA - ");
    Serial.println(channel0avgA);
    Serial.print("step4currentAcalibrated - ");
    Serial.println(step4currentAcalibrated);

    Serial.print("channel1avgA - ");
    Serial.println(channel1avgA);
    Serial.print("step3voltageAcalibrated - ");
    Serial.println(step3voltageAcalibrated);

    Serial.print("powerA - ");
    Serial.println(powerA);
    Serial.print("Whaccumulator - ");
    Serial.println(Whaccumulator / 3600);

    Serial.print("numberofseconds - ");
    Serial.println(numberofseconds);
    Serial.println();
    numberofseconds++;

    numberofsamples = 0;
    channel0accumulator = 0;
    channel1accumulator = 0;
  }
}

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
