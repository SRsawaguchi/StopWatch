#ifndef __LED_H__
#define __LED_H__

#define LEDS (GPIO_PIN_0|GPIO_PIN_1 |GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6)

void led_init(void);
void led_all_off(void);
void led_all_on(void);
unsigned char led_get_state(void);
void led_set_state(unsigned char state);
void led_on(unsigned char leds);
void led_off(unsigned char leds);
void led_write(unsigned char data);

#endif // __LED_H__
