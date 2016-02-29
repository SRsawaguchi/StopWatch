#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"

#include "board.h"
#include "sw.h"

void sw_default_handler(void){
	while(1);
}
void (*_sw1_handler)() = sw_default_handler;
void (*_sw2_handler)() = sw_default_handler;
void SwIntHandler(void) {
	volatile long loop;
	char btnStatus = GPIOIntStatus(GPIO_PORTB_BASE, true);

	switch (btnStatus) {
	case PSW_1:
		_sw1_handler();
		break;
	case PSW_2:
		_sw2_handler();
		break;
	default:
		break;
	} //end switch

	GPIOIntClear(GPIO_PORTB_BASE,PSW_ALL);
}


void (*_int_handler)() = SwIntHandler;

void sw_set_sw1_handler(void (*handler)(void)){
	_sw1_handler = handler;
}

void sw_set_sw2_handler(void (*handler)(void)){
	_sw2_handler = handler;
}
void sw_set_int_handler(void (*handler)(void)){
	GPIOIntRegister(PSW_PORT, handler);
}

void sw_init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,PSW_ALL);
	GPIOIntTypeSet(GPIO_PORTB_BASE, PSW_ALL, GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTB_BASE, SwIntHandler);
	GPIOIntEnable(GPIO_PORTB_BASE, PSW_ALL);
	IntEnable(INT_GPIOB);
}
