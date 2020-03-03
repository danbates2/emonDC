
/*************************************************************************
**  Device: MCP3204/MCP3208                                          	**
**  File:   AH_MCP320x.h - Arduino Library for 12bit ADC	    	**
**			  		     				**
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


#ifndef AH_MCP320x_h
#define AH_MCP320x_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SINGLE       true
#define DIFFERENTIAL false


class AH_MCP320x
{
  public:
    AH_MCP320x(int CS, int DOUT,int DIN, int CLK);
    AH_MCP320x(int CS);
    int readCH(int CHANNEL);
    void readALL(int values[], int n);
    void getCONFIG(int config[],int n);
    void setCONFIG(int CHANNEL, boolean MODE);
    void setCONFIG_allDiff(int n);
    void setCONFIG_allSingle(int n);
    float calcVOLT(float VREF, int ADCx);


  private:
    int _CONFIGURATION[8];
    int _CS;
    int _DATAOUT;
    int _DATAIN;
    int _CLK;
    bool _MODE;
    int readADC_SPI(int CHANNEL);
    int readADC_pin(int CHANNEL);
    void CLK_clock(int n);
 
};

#endif
