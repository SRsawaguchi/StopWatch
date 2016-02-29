#ifndef __TIMER_INTERFACE_H__
#define __TIMER_INTERFACE_H__

typedef struct timer_interface{
    void (*start_timer)(void);
    void (*stop_timer)(void);
}TimerInterface;

#endif // __TIMER_INTERFACE_H__
