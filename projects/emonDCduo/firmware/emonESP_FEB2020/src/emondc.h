// Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONDC_H
#define _EMONDC_H

#include <WString.h>


extern unsigned int main_emondc_interval; // default milliseconds interval for averaging samples and sending to emonCMS.

extern int _t_begin;
extern void emondc_setup(void);
extern void emondc_loop(void);
extern float Vcal1;
extern float Ccal1;
extern float Vcal2;
extern float Ccal2;

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

//RTC_PCF8523_GET()
void RTC_PCF8523_GET(void);
void RTC_PCF8523_PRINT(void);
void average_and_calibrate(unsigned long, unsigned long);
void print_readable(void);
void forward_to_emonESP(void);
void clear_accumulators(void);
void forward_to_RFM69Pi(void);
void forward_to_emonESP(void);
float make_readable_Amps(float adcValuePassed, String chan);
float make_readable_Volts (float _adcValue, String _chan);
extern void Wh_accumulate(float current_reading, int elapsed_seconds);

extern void config_save_emondc(unsigned int interval, float vcalA, float icalA, float vcalB, float icalB);
extern float time_until_discharged(void);
extern void save_to_SDcard(void);

const bool chanRef = 0;

extern int averaging_loop_counter;

const String chanA = "A";
const String chanB = "B";


#endif // _EMONDC_H
