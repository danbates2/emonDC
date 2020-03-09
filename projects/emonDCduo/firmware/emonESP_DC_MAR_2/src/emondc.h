// Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONDC_H
#define _EMONDC_H

#include <WString.h>

extern unsigned int main_emondc_interval; // default milliseconds interval for averaging samples and sending to emonCMS.
extern int averaging_loop_counter;
extern int _t_begin;
const bool chanRef = 0;
const String chanA = "A";
const String chanB = "B";
extern float Vcal1;
extern float Ccal1;
extern float Vcal2;
extern float Ccal2;
extern bool chanAref;
extern bool chanBref;
extern unsigned int _MAIN_INTERVAL;
extern bool chanAref;
extern bool chanBref;
extern float chanA_shuntAmp_gain;
extern float chanB_shuntAmp_gain;
extern float R1_A;
extern float R2_A;
extern float R1_B;
extern float R2_B;
extern float Rshunt_A;
extern float Rshunt_B;
extern float _CAL_FACTOR_icalA;
extern float _CAL_FACTOR_vcalA;
extern float _CAL_FACTOR_icalB;
extern float _CAL_FACTOR_vcalB;
extern unsigned int waking_time;
extern unsigned int sleeping_time;

extern void emondc_setup(void);
extern void emondc_loop(void);
extern void config_save_emondc(unsigned int interval, float vcalA, float icalA, float vcalB, float icalB);

float volts_to_adc_reading_ratio_function(void);
int read_adc(int);
void averaging_print_send(void);
void drawvalues_to_OLED(void);
double Ah_calculate(float amps_value, int elapsed_seconds);
void NTP_update_PCF8523_update(void);
void average_and_calibrate(unsigned long, unsigned long);
void print_readable(void);
void forward_to_emonESP(void);
void clear_accumulators(void);
void forward_to_RFM69Pi(void);
void forward_to_emonESP(void);
float make_readable_Amps(float adcValuePassed, String chan);
float make_readable_Volts (float _adcValue, String _chan);
double Wh_accumulate(float current_value, float voltage_value, int elapsed_seconds);
float time_until_discharged(void);
void save_to_SDcard(void);
float make_readable_Amps (float _adcValue, String _chan, float _gain);
float make_readable_Volts (float _adcValue, String _chan);

#endif // _EMONDC_H
