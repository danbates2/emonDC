
#ifndef _EMONESP_GPIO0_H
#define _EMONESP_GPIO0_H


extern void gpio0_loop();
extern void gpio0_setup();
extern void ICACHE_RAM_ATTR gpio0_isr();
extern void led_flash(int, int);
extern unsigned long TimeButtonPressedExt;

#endif // _EMONESP_GPIO0_H
