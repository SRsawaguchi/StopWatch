#ifndef __LCD_H__
#define __LCD_H__

extern void DelayMs(unsigned long ms);

void lcd_disp(char *s);
void lcd_newline(void);
void lcd_home(void);
void lcd_locate(unsigned char x, unsigned char y);
void lcd_cls(void);
void lcd_init(void);
void lcd_configure(void);
void lcd_i_write(unsigned char data);
void lcd_d_write(unsigned char data);
void lcd_clock(unsigned char data);
void lcd_brink(void);
void lcd_stop_brink(void);

#endif // __LCD_H__
