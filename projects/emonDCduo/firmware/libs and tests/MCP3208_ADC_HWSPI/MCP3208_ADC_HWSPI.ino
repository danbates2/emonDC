// SELPIN   10   //Selection Pin (CS)
// DATAOUT  11   //MOSI 
// DATAIN   12   //MISO 
// SPICLOCK 13   //Clock

#include "AH_MCP320x.h"
#include <SPI.h>

int ADCValue = 0;
float time_in;
float time_out;

unsigned long numberofsample_8CH;
int interval = 5000;
unsigned long previousMillis = 0;
AH_MCP320x ADC_SPI(2);             //Object definition with D9 as CS, other pins are defined by spi interface (faster mode).
//AH_MCP320x ADC_pin(10,11,12,13);  //Object definition with free definition of communication pins (slower mode).

void setup()
{
  Serial.begin(115200);
  //SPI.setClockDivider(SPI_CLOCK_DIV128);
}

void loop()
{  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
   
    Serial.print("Number of samples: ");
    Serial.println(numberofsample_8CH);
    numberofsample_8CH = 0; //reset
  }
  numberofsample_8CH++;
  for(int i=0; i<8; i++){
    int yes = ADC_SPI.readCH(i);
  }
}


/*
  
  for(int i=0;i<8;i++)   // i<8 at MCP3208 
  {  
    Serial.print("CH"); 
    Serial.print(i);     
    Serial.print(": ");   
    time_in = micros();  
    ADCValue = ADC_SPI.readCH(i);      //read analog value from IC (SPI mode)
//  ADCValue = ADC_pin.readCH(i);      //read analog value from IC (Pin mode)   
    time_out = micros();
    
    Serial.println(ADCValue);   
    
    Serial.print("Voltage:");
    Serial.print(ADC_SPI.calcVOLT(5.0,ADCValue));     //calculate the input voltage at 5V VREF
//    Serial.print(ADC_pin.calcVOLT(5.0,ADCValue));   //calculate the input voltage
    Serial.println(" V");
    
    Serial.print("Time: ");     
    Serial.print(time_out-time_in);   
    Serial.println(" ms");     
    Serial.println();
  
    delay(10);
  }
  delay (1000);
}
*/
