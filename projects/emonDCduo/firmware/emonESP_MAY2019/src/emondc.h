// Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONDC_H
#define _EMONDC_H

#include <WString.h>

extern unsigned long main_emondc_interval; // default milliseconds interval for averaging samples and sending to emonCMS.

extern void emondc_setup(void);
extern void emondc_loop(void);

extern double Vcal1;
extern double Ccal1;
extern double Vcal2;
extern double Ccal2;

extern bool chanAref;
extern bool chanBref;

extern bool sendData_go;

extern bool RFM69_enabled;

extern unsigned int waking_time;
extern unsigned int sleeping_time;

extern float Valarm_A_HIGH;
extern float Valarm_A_LOW;
extern float Valarm_A_VERY_LOW;
extern float Valarm_B_HIGH;
extern float Valarm_B_LOW;
extern float Valarm_B_VERY_LOW;
extern float Calarm_A_HIGH;
extern float Calarm_B_HIGH;

extern bool midnight_cal;

int read_adc(int);
void averaging_print_send(void);
void drawvalues_to_OLED(void);



void average_and_calibrate(void);
void print_readable(void);
void clear_accumulators(void);
void forward_to_RFM69Pi(void);
void forward_to_emonESP(void);
double make_readable_Amps(double adcValuePassed, String chan);

extern void config_save_emondc(unsigned long interval);
extern double time_until_discharged(void);
extern void save_to_SDcard(void);

const bool chanRef = 0;

extern int averaging_loop_counter;

extern double CAL0;
extern double CAL1;
extern double CAL2;
extern double CAL3;
extern double CAL7;

const String chanA = "A";
const String chanB = "B";


#endif // _EMONDC_H
