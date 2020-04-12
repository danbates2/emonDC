// Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONDC_H
#define _EMONDC_H

#include <WString.h>

extern uint16_t main_interval_seconds;
extern uint32_t main_interval_ms;
extern unsigned int averaging_loop_counter;
extern unsigned int _t_begin;
extern bool chanA_VrefSet; 
extern bool chanB_VrefSet;
extern uint16_t channelA_gain;
extern uint16_t channelB_gain;
extern uint32_t R1_A;
extern uint32_t R2_A;
extern uint32_t R1_B;
extern uint32_t R2_B;
extern double Rshunt_A;
extern double Rshunt_B;
extern double icalA;
extern double vcalA;
extern double icalB;
extern double vcalB;
extern unsigned int waking_time;
extern unsigned int sleeping_time;
extern double AmpOffset_A;
extern double VoltOffset_A;
extern double AmpOffset_B;
extern double VoltOffset_B;

// false for chanA, true for chanB.
const bool chanBool = 0;
extern int screentog;
extern bool oled_button_flag;
extern void emondc_setup(void);
extern void emondc_loop(void);

void config_save_emondc(unsigned int interval, double vcalA, double icalA, double vcalB, double icalB);
double volts_to_adc_reading_ratio_function(void);
void averaging_print_send(void);
extern void draw_OLED();
void NTP_update_PCF8523_update(void);
void save_to_SDcard(void);
void print_readable(void);
void forward_to_emonESP(void);
void forward_to_emonESP(void);
void average_and_calibrate(unsigned long, unsigned long);
double Wh_accumulate(double current_value, double voltage_value, int elapsed_seconds);
double Ah_calculate(double amps_value, int elapsed_seconds);
double make_readable_Amps(double _adcValue, bool _chan, double _gain);
double make_readable_Volts(double _adcValue, bool _chan);
double time_until_discharged_fromfull(void);
double effective_capacity_fromfull(void);
double Ah_calculate(double amps_value, uint16_t elapsed_seconds);
void reset_ah_capacity(void);
void clear_accumulators(void);

#endif // _EMONDC_H
