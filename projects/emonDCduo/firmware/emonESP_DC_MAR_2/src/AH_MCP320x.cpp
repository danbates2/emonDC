
/*************************************************************************
**  Device: MCP3204/MCP3208                                             **
**  File:   AH_MCP320x.h - Arduino Library for 12bit ADC	        **
**								    	**
**  Created by A. Hinkel 2012-04-26                                 	**
**  download from   							**
**									**
**  Based on Code from:							**
**  http://arduino.cc/playground/Code/MCP3208				**
**  http://arduino.cc/forum/index.php?topic=53082.0			**
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
*************************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AH_MCP320x.h"
#include "SPI.h"



//************************************************************************
//FREE PIN MODE INIT
//************************************************************************
AH_MCP320x::AH_MCP320x(int CS, int DOUT,int DIN, int CLK)
{
  _CS      = CS;
  _DATAOUT = DOUT;
  _DATAIN  = DIN;
  _CLK     = CLK;
  _MODE    = 0; //PIN mode

  int i;
  for (i=0;i<8;i++){ 
     _CONFIGURATION[i] = 1;
  }

  pinMode(_CS, OUTPUT); 
  pinMode(_DATAOUT, OUTPUT); 
  pinMode(_DATAIN, INPUT); 
  pinMode(_CLK, OUTPUT); 

  //disable device to start with 
  digitalWrite(_CS,HIGH); 
  digitalWrite(_DATAOUT,LOW); 
  digitalWrite(_CLK,LOW); 

}

//************************************************************************
//SPI MODE INIT
//************************************************************************
AH_MCP320x::AH_MCP320x(int CS)
{
  _CS   = CS;
  _MODE = 1; //SPI mode
  
  int i;
  for (i=0;i<8;i++){ 
     _CONFIGURATION[i] = 1;
  }

  pinMode(_CS,OUTPUT);
  digitalWrite(_CS,HIGH);
  //SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
}

//************************************************************************
void AH_MCP320x::getCONFIG(int config[], int n)
{
  for (int i=0;i<n;i++){
	config[i] = _CONFIGURATION[i];
  }
}
//************************************************************************ 

void AH_MCP320x::setCONFIG(int CHANNEL, boolean MODE)
{
   if (MODE)
	_CONFIGURATION[CHANNEL] = 1;  
   else
	_CONFIGURATION[CHANNEL] = 0;
}

//************************************************************************ 

void AH_MCP320x::setCONFIG_allSingle(int n)
{
    for(int i=0;i<n;i++){
        _CONFIGURATION[i] = 1;
    }
}

//************************************************************************ 
void AH_MCP320x::setCONFIG_allDiff(int n)
{
    for(int i=0;i<n;i++){
        _CONFIGURATION[i] = 0;
    }
}

//************************************************************************
void AH_MCP320x::readALL(int values[], int n)
{

   if (_MODE){
   	for(int i=0;i<n;i++){
    	    values[i] = readADC_SPI(i);
    	}
   }
   else{
     	for(int i=0;i<n;i++){
    	    values[i] = readADC_pin(i);
     	}
   }
} 

//************************************************************************ 
int AH_MCP320x::readCH(int CHANNEL)
{
   if (_MODE){
    	return readADC_SPI(CHANNEL);
   }
   else{
    	return readADC_pin(CHANNEL);
   }
}

//************************************************************************
float AH_MCP320x::calcVOLT(float VREF, int ADCx){

    float volt = (float) ADCx * VREF/4096;
    return volt;
}

//************************************************************************ 
 
int AH_MCP320x::readADC_SPI(int CHANNEL)
{
  int msb;
  int lsb; 

  //Channel must be from 0 to 7 for MCP3208
  //uint16_t
  int commandBytes = B00000100;   		 	//Start bit
  commandBytes |= (_CONFIGURATION[CHANNEL]<<1);	 	//Config mode bit for this channel
  commandBytes  = (commandBytes<<8);		 
  commandBytes |= (CHANNEL<<6);			 	//3 bits of channel number

  digitalWrite(_CS, LOW);			 	//Select ADC

    SPI.transfer((commandBytes>>8) & 0xff);	 	//send data

    msb = SPI.transfer((byte)commandBytes & 0xff) & B00001111;
    lsb = SPI.transfer(0x00);
  
  digitalWrite(_CS,HIGH);				//turn off device
  
  // cast before shiting the byte
  return ((int) msb) <<8 | lsb;

}
 
//************************************************************************ 

int AH_MCP320x::readADC_pin(int CHANNEL){

  int ADCvalue = 0;

  byte commandMSB = B10000000; 			//Start bit
  commandMSB |= (_CONFIGURATION[CHANNEL]<<6);	//Config mode bit for channel
  commandMSB |= (CHANNEL<<3);			//3 bits of channel number

  digitalWrite(_CS,LOW); 			//Select ADC

    for (int i=7; i>=3; i--){			//setup bits to be written
      digitalWrite(_DATAOUT,commandMSB&1<<i);
      CLK_clock(1);    				//cycle clock 1x
    }

    CLK_clock(2);	
				 //ignores 2 null bits

    for (int i=11; i>=0; i--){  			//read bits from adc
      ADCvalue+=digitalRead(_DATAIN)<<i;    
      CLK_clock(1);				//cycle clock 1x
    }

  digitalWrite(_CS, HIGH); 			//turn off device

  return ADCvalue;

}
 

//************************************************************************ 
//*** only for PIN mode ***
//************************************************************************

void AH_MCP320x::CLK_clock(int n){   

  for (int i=0;i<n;i++){			
    digitalWrite(_CLK,HIGH);    			
    digitalWrite(_CLK,LOW);
  }

}


