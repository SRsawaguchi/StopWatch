#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

#include "board.h"
#include "lcd.h"

void lcd_disp(char *s)
{
	while(*s){
		lcd_d_write(*s++);
	}
	DelayMs(1);
}

void lcd_newline(void)
{
	lcd_locate(0, 1);
	DelayMs(2);
}

void lcd_home(void)
{
	lcd_locate(0, 0);
	DelayMs(2);
}

void lcd_locate(unsigned char x, unsigned char y)
{
	unsigned char pos = y*0x40 + x;

	pos |= 0x80;
	lcd_i_write(pos);
}

void lcd_cls(void)
{
	lcd_i_write(0x01);
	DelayMs(2);
}

void lcd_init(void)
{
	lcd_i_write(0x28);
	lcd_i_write(0x08);
	lcd_cls();
	lcd_i_write(0x0c);
	lcd_i_write(0x06);
}

void lcd_configure(void)
{
	SysCtlPeripheralEnable(LCD_PERIPH_D4);
	SysCtlPeripheralEnable(LCD_PERIPH_E);
	SysCtlPeripheralEnable(LCD_PERIPH_RS);
	GPIOPinTypeGPIOOutput(LCD_PORT_D4, LCD_D4);
	GPIOPinTypeGPIOOutput(LCD_PORT_RS, LCD_RS);
	GPIOPinTypeGPIOOutput(LCD_PORT_E,LCD_E);

	DelayMs(30);
	GPIOPinWrite(LCD_PORT_RS, LCD_RS, 0x00);

	DelayMs(15);
	lcd_clock(3);
	DelayMs(5);
	lcd_clock(3);

	lcd_clock(3);
	lcd_clock(2);
}

void lcd_i_write(unsigned char data)
{
	GPIOPinWrite(LCD_PORT_RS, LCD_RS, 0x00);
	DelayMs(1);

	lcd_clock((data&0xf0)>>4);
	lcd_clock(data&0x0f);
}

void lcd_d_write(unsigned char data)
{
	GPIOPinWrite(LCD_PORT_RS, LCD_RS, LCD_RS);
	DelayMs(1);

	lcd_clock((data&0xf0)>>4);
	lcd_clock(data&0x0f);
}

void lcd_clock(unsigned char data)
{
	GPIOPinWrite(LCD_PORT_E, LCD_E, LCD_E);
	GPIOPinWrite(LCD_PORT_D4, LCD_D4, (data&0x0f)<<2);
	DelayMs(1);
	GPIOPinWrite(LCD_PORT_E, LCD_E, 0x00);
	DelayMs(1);
}

void lcd_brink(void){
    lcd_i_write(0x0d);
}

void lcd_stop_brink(void){
    lcd_i_write(0x0c);
}
