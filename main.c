#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "board.h"
#include "lcd.h"
#include "led.h"
#include "sw.h"
#include "timer_interface.h"
#include "watch.h"

Watch _watch;

int g_cnt = 0;
void SysTickHandler(void) {
	g_cnt--;
}
void DelayMs(unsigned long ms) {
	g_cnt = ms;
	SysTickEnable();
	while (g_cnt > 0)
		;
	SysTickDisable();
}


void stop_timer(void){
	TimerDisable(TIMER1_BASE,TIMER_A);
}

void start_timer(void){
	TimerEnable(TIMER1_BASE,TIMER_A);
}

void sw_int_handler(void) {
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void timer0_int_handler(void) {
    char data = GPIOIntStatus(PSW_PORT,true);
	TimerDisable(TIMER0_BASE, TIMER_A);
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	GPIOIntClear(PSW_PORT, PSW_ALL);

    switch (data) {
        case PSW_1:
            on_lap_split_btn_push(&_watch);
            break;
        case PSW_2:
            on_start_stop_btn_push(&_watch);
            break;
    } //end switch

}

//g_timeのカウントUP
void timer1_int_handler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_DMA_TIMEOUT_A);
	on_timer_tick(&_watch);
}

//LED8の点滅
void timer2_int_handler(void) {
	unsigned char state;
	TimerIntClear(TIMER2_BASE, TIMER_DMA_TIMEOUT_A);
	state = led_get_state();
	led_set_state(state ^ 0x80);
}

void lcd_diff_disp(const char prev[17],const char next[17],int row){
    int i;
    for(i=0;i<17;i++){
        if(prev[i] != next[i]){
           lcd_locate(i,row);
           lcd_d_write(next[i]);
        }//end if
    }//end for
}

int main(void) {
    char buff_line1[17];
    char buff_line2[17];
    TimerInterface timer_interface;
	volatile unsigned loop;

	SysCtlClockSet(
			SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN
					| SYSCTL_XTAL_16MHZ);
	led_init();
	sw_init();
	sw_set_int_handler(sw_int_handler);

	/* Timer0A チャタリング対策。 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 5);
	TimerIntRegister(TIMER0_BASE, TIMER_A, timer0_int_handler);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//	TimerEnable(TIMER0_BASE,TIMER_A);

	/* Timer1A カウントアップ */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 1000);
	TimerIntRegister(TIMER1_BASE, TIMER_A, timer1_int_handler);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER1A);

	/* Timer2A led8点滅*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet());
	TimerIntRegister(TIMER2_BASE, TIMER_A, timer2_int_handler);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER2_BASE, TIMER_A);

	/* SysTick */
	SysTickPeriodSet(SysCtlClockGet() / 1000);
	SysTickIntRegister(SysTickHandler);

	lcd_configure();
	lcd_init();

	led_all_off();
	lcd_cls();

	timer_interface.start_timer = start_timer;
	timer_interface.stop_timer  = stop_timer;

	watch_init(&_watch,&timer_interface);

    while (1) {
        on_lcd_update(&_watch);
        if(_watch.state == LAP){
            lcd_brink();
        }else{
            lcd_stop_brink();
        }
        lcd_diff_disp(buff_line1,_watch.line1,0);
        lcd_diff_disp(buff_line2,_watch.line2,1);
        lcd_home();
        strncpy(buff_line1,_watch.line1,17);
        strncpy(buff_line2,_watch.line2,17);

        if (_watch.state == IDLE) {
            DelayMs(500);
        } else {
            DelayMs(50);
        }
    }
}
