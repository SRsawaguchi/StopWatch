#ifndef __BOARD_H__
#define __BOARD_H__

/* Push switches */
#define PSW_PORT (GPIO_PORTB_BASE)
#define PSW_ALL  (GPIO_PIN_2|GPIO_PIN_3)
#define PSW_1    (GPIO_PIN_2)
#define PSW_2    (GPIO_PIN_3)

/* LEDs */
#define LED_ALL (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7)
#define LED_1_4 (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3)
#define LED_5_8 (GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7)
#define LED_1   (GPIO_PIN_0)
#define LED_2   (GPIO_PIN_1)
#define LED_3   (GPIO_PIN_2)
#define LED_4   (GPIO_PIN_3)
#define LED_5   (GPIO_PIN_4)
#define LED_6   (GPIO_PIN_5)
#define LED_7   (GPIO_PIN_6)
#define LED_8   (GPIO_PIN_7)

/* LCD */
#define LCD_PERIPH_RS (SYSCTL_PERIPH_GPIOE)
#define LCD_PERIPH_E  (SYSCTL_PERIPH_GPIOE)
#define LCD_PERIPH_D4 (SYSCTL_PERIPH_GPIOA)
#define LCD_PORT_RS   (GPIO_PORTE_BASE)
#define LCD_PORT_E    (GPIO_PORTE_BASE)
#define LCD_PORT_D4   (GPIO_PORTA_BASE)
#define LCD_RS        (GPIO_PIN_1)
#define LCD_E         (GPIO_PIN_0)
#define LCD_D4        (GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5)

#endif // __BOARD_H__
