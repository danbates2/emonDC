//for ESP8266 tested on ESP-12S
#define MCP320X_CHS 8

#define SELPIN 2 //Selection Pin 
#define DATAOUT 13//MOSI 
#define DATAIN  12//MISO 
#define SPICLOCK  14//Clock 
int readvalue;

unsigned long numberofsamples;
int interval = 5000;
unsigned long previousMillis = 0;

void setup() {
  //set pin modes
  pinMode(SELPIN, OUTPUT);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  //disable device to start with
  digitalWrite(SELPIN, HIGH);
  digitalWrite(DATAOUT, LOW);
  digitalWrite(SPICLOCK, LOW);

  Serial.begin(115200);
}

int read_adc(int channel) {
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
  //B10100000
  //allow channel selection
  commandbits |= ((channel - 1) << 3);

  digitalWrite(SELPIN, LOW); //Select adc
  // setup bits to be written
  for (int i = 7; i >= 3; i--) {
    digitalWrite(DATAOUT, commandbits & 1 << i);
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }

  digitalWrite(SPICLOCK, HIGH);   //ignores 2 null bits
  digitalWrite(SPICLOCK, LOW);
  digitalWrite(SPICLOCK, HIGH);
  digitalWrite(SPICLOCK, LOW);

  //read bits from adc
  for (int i = 11; i >= 0; i--) {
    adcvalue += digitalRead(DATAIN) << i;
    //cycle clock
    digitalWrite(SPICLOCK, HIGH);
    digitalWrite(SPICLOCK, LOW);
  }
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}


void loop() {

  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  
    Serial.print("Number of samples: ");
    Serial.println(numberofsamples);
    numberofsamples = 0; //reset
  }
  numberofsamples++;
  for(int i=0; i<MCP320X_CHS; i++){
    int yes = read_adc(i);
  }
}
/*
  readvalue = read_adc(1);
  Serial.print("CH1: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(2);
  Serial.print("CH2: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(3);
  Serial.print("CH3: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(4);
  Serial.print("CH4: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(5);
  Serial.print("CH5: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(6);
  Serial.print("CH6: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(7);
  Serial.print("CH7: ");
  Serial.println(readvalue, DEC);
  readvalue = read_adc(8);
  Serial.print("CH8: ");
  Serial.println(readvalue, DEC);
  Serial.println(" ");
  delay(100);
}
*/
