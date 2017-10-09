#include <SPI.h>

#define CS_3202 15

void setup()
{
  Serial.begin(57600);
  pinMode(CS_3202,OUTPUT);
  digitalWrite(CS_3202,HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop()
{
  Serial.print("CH0 =");
  Serial.println(Read3202(0,CS_3202));   
  Serial.print("CH1 =");
  Serial.println(Read3202(1,CS_3202));   
  delay(300);
}

int Read3202(int CHANNEL, int CS){
  int msb;
  int lsb;
  int commandBytes = B10100000 ;              // channel 0          
  if (CHANNEL==1) commandBytes = B11100000 ;  // channel 1
  digitalWrite(CS, LOW);
  SPI.transfer (B00000001);                   // Start bit
  msb = SPI.transfer(commandBytes) ;
  msb = msb & B00001111;
  lsb = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  return ((int) msb) <<8 | lsb;
}  
