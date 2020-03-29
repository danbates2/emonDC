// Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONDC_H
#define _EMONDC_H

#include <WString.h>

extern unsigned int main_interval_seconds;
extern unsigned long main_interval_;
extern int averaging_loop_counter;
extern unsigned int _t_begin;
extern double Vcal1;
extern double Ccal1;
extern double Vcal2;
extern double Ccal2;
extern bool chanAref;
extern bool chanBref;
extern double chanA_shuntAmp_gain;
extern double chanB_shuntAmp_gain;
extern double R1_A;
extern double R2_A;
extern double R1_B;
extern double R2_B;
extern double Rshunt_A;
extern double Rshunt_B;
extern double _CAL_FACTOR_icalA;
extern double _CAL_FACTOR_vcalA;
extern double _CAL_FACTOR_icalB;
extern double _CAL_FACTOR_vcalB;
extern unsigned int waking_time;
extern unsigned int sleeping_time;

const bool chanRef = 0;
const String chanA = "A";
const String chanB = "B";

extern void emondc_setup(void);
extern void emondc_loop(void);

void config_save_emondc(unsigned int interval, double vcalA, double icalA, double vcalB, double icalB);
double volts_to_adc_reading_ratio_function(void);
//int read_adc(int);
void averaging_print_send(void);
void draw_OLED(void);
void NTP_update_PCF8523_update(void);
void average_and_calibrate(unsigned long, unsigned long);
void print_readable(void);
void forward_to_emonESP(void);
void clear_accumulators(void);
void forward_to_RFM69Pi(void);
void forward_to_emonESP(void);
double Wh_accumulate(double current_value, double voltage_value, int elapsed_seconds);
double Ah_calculate(double amps_value, int elapsed_seconds);
void save_to_SDcard(void);
double make_readable_Amps(double _adcValue, String _chan, double _gain);
double make_readable_Volts(double _adcValue, String _chan);


double time_until_discharged_fromfull(void);
double effective_capacity_fromfull(void);
double Ah_calculate(double amps_value, uint16_t elapsed_seconds);
void reset_ah_capacity(void);

#endif // _EMONDC_H
