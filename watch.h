#ifndef __WATCH_H__
#define __WATCH_H__

#include "timer_interface.h"
#define TIMER_MAX (35999990)

typedef enum Mode{
  LAP_SPLIT,START_STOP,
}Mode;

typedef enum State{
  IDLE,STOP,LAP,RUN,NONE
}State;

typedef struct Watch{
  int   time;
  int   lap;
  int   split;
  int   start;
  char  line1[17];
  char  line2[17];
  int   is_mode_settled;
  Mode  mode;
  State state;
  TimerInterface *timer_interface;
}Watch;

typedef struct Time{
  int serial;
  int hour;
  int min;
  int sec;
  int ms;
}Time;

void watch_init(Watch *,TimerInterface *timer_interface);
void time_set(Time *,int);
void time_to_string(Time *,char *);
void on_start_stop_btn_push(Watch *);
void on_lap_split_btn_push(Watch *);
void on_lcd_update(Watch *);
void on_timer_tick(Watch *);
void on_state_change(Watch *);

#endif // __TIMER_INTERFACE_H__
