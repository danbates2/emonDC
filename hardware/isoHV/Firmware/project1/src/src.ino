#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 433.0

#if defined(ADAFRUIT_FEATHER_M0) // Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
#endif

RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission

/************ Other Setup ***************/


unsigned long numberofsamples = 0;

const long interval = 10000; // ms

unsigned long previousMillis = 0;

float CH_A_CURRENT_AVERAGED;
float CH_A_VOLTAGE_AVERAGED;
float CH_B_CURRENT_AVERAGED;
float CH_B_VOLTAGE_AVERAGED;
float Voltage_A;
float Current_A;
float Voltage_B;
float Current_B;
float Wh_chA = 0.0;
float Wh_chB = 0.0;
unsigned long CH_A_CURRENT_ACCUMULATOR = 0;
unsigned long CH_A_VOLTAGE_ACCUMULATOR = 0;
unsigned long CH_B_CURRENT_ACCUMULATOR = 0;
unsigned long CH_B_VOLTAGE_ACCUMULATOR = 0;

String ADC_KeyValue_String = "Hellow";

void setup()
{
  delay(2000);
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(14, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  //rf69.setEncryptionKey(key);

  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  analogReadResolution(12);
}



void loop() {

  int sensor_Volts_B = analogRead(A0);
  int sensor_Amps_B = analogRead(A1);
  int sensor_Volts_A = analogRead(A2);
  int sensor_Amps_A = analogRead(A3);
  CH_A_CURRENT_ACCUMULATOR = CH_A_CURRENT_ACCUMULATOR + sensor_Amps_A;
  CH_A_VOLTAGE_ACCUMULATOR = CH_A_VOLTAGE_ACCUMULATOR + sensor_Volts_A;
  CH_B_CURRENT_ACCUMULATOR = CH_B_CURRENT_ACCUMULATOR + sensor_Amps_B;
  CH_B_VOLTAGE_ACCUMULATOR = CH_B_VOLTAGE_ACCUMULATOR + sensor_Volts_B;
  numberofsamples ++;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    CH_A_CURRENT_AVERAGED = CH_A_CURRENT_ACCUMULATOR / numberofsamples;
    CH_A_VOLTAGE_AVERAGED = CH_A_VOLTAGE_ACCUMULATOR / numberofsamples;
    CH_B_CURRENT_AVERAGED = CH_B_CURRENT_ACCUMULATOR / numberofsamples;
    CH_B_VOLTAGE_AVERAGED = CH_B_VOLTAGE_ACCUMULATOR / numberofsamples;

    CH_A_CURRENT_ACCUMULATOR = 0;
    CH_A_VOLTAGE_ACCUMULATOR = 0;
    CH_B_CURRENT_ACCUMULATOR = 0;
    CH_B_VOLTAGE_ACCUMULATOR = 0;
    numberofsamples = 0;

    ADC_KeyValue_String = "V_A:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH_A_VOLTAGE_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "C_A:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH_A_CURRENT_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "V_B:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH_B_VOLTAGE_AVERAGED;
    ADC_KeyValue_String = ADC_KeyValue_String + ",";
    ADC_KeyValue_String = ADC_KeyValue_String + "C_B:";
    ADC_KeyValue_String = ADC_KeyValue_String + CH_B_CURRENT_AVERAGED;

 
    char radiopacket[60] = "hellow"; // to init
    //itoa(packetnum++, radiopacket + 13, 10);
    strcpy(radiopacket, ADC_KeyValue_String.c_str());
    Serial.print("Sending \"");
    Serial.print(radiopacket);
    Serial.println("\"");

    // Send a message!
    Blink(LED, 10, 1);
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
  }
}

void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i = 0; i < loops; i++)  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
}
