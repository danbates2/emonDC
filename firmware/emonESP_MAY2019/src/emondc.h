// emonDC code additions, Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONESP_EMONDC_H
#define _EMONESP_EMONDC_H

extern unsigned long main_emondc_interval; // default milliseconds interval for averaging samples and sending to emonCMS.

extern double Vcal1;
extern double Ccal1;
extern double Vcal2;
extern double Ccal2;

extern bool sendData_go;

extern bool RFMenable;

extern int waking_time;
extern int sleeping_time;

extern float alarmlevel1;
extern float alarmlevel2;

extern bool midnight_calibration;

int read_adc(int);
void averaging_print_send(void);
void drawvalues(void);
void emondcsetup(void);
void emondc_loop(void);

extern void config_save_emondc(unsigned long interval);


#endif // _EMONESP_EMONCMS_H
