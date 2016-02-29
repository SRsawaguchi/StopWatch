#ifndef __SW_H__
#define __SW_H__

void sw_init(void);
void sw_set_sw1_handler(void (*handler)(void));
void sw_set_sw2_handler(void (*handler)(void));
void sw_set_int_handler(void (*handler)(void));
void sw_init(void);

#endif // __SW_H__
