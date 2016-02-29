#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "board.h"
#include "led.h"

void led_init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,LED_1_4);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,LED_5_8);
}

void led_all_off(void){
	led_write(0xff);
}

void led_all_on(void){
	led_write(0x00);
}

unsigned char led_get_state(void){
	unsigned char left  = GPIOPinRead(GPIO_PORTD_BASE,LED_1_4)&0x0f;
	unsigned char right = GPIOPinRead(GPIO_PORTC_BASE,LED_5_8)&0xf0;
	return ~(left | right);
}

void led_set_state(unsigned char state){
	led_write(~state);
}

void led_on(unsigned char leds){
	unsigned char state = led_get_state();
	led_write(~(state|leds));
}

void led_off(unsigned char leds){
	unsigned char state = led_get_state();
	led_write(~(state^leds));
}

void led_write(unsigned char data){
	GPIOPinWrite(GPIO_PORTD_BASE,LED_1_4,data&LED_1_4);
	GPIOPinWrite(GPIO_PORTC_BASE,LED_5_8,data&LED_5_8);
}
