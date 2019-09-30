// emonDC code additions, Dan Bates 2019.
// Free to use and modify.


#ifndef _EMONESP_EMONDC_H
#define _EMONESP_EMONDC_H

extern unsigned long main_emondc_interval; // default milliseconds interval for averaging samples and sending to emonCMS.

// extern bool go_or_no = 0; // start or stop adc sampling, saving and posting.

int read_adc(int);
void averaging_print_send(void);
void drawvalues(void);
void emondcsetup(void);
void emondcsampling(void);

extern void config_save_emondc(unsigned long interval);


#endif // _EMONESP_EMONCMS_H
