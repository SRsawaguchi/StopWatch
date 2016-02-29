#include "watch.h"

#define ITOA(__N__) ((__N__) | 0x30)

void on_state_change_idle(Watch *);
void on_state_change_run(Watch *);
void on_state_change_lap(Watch *);
void on_state_change_stop(Watch *);

void on_lcd_update_idle(Watch *);
void on_lcd_update_run(Watch *);
void on_lcd_update_lap(Watch *);
void on_lcd_update_stop(Watch *);

void (*fv_on_state_change[4])(Watch *) = {
  on_state_change_idle,
  on_state_change_stop,
  on_state_change_lap,
  on_state_change_run,
};

void (*fv_on_lcd_update[4])(Watch *) = {
  on_lcd_update_idle,
  on_lcd_update_stop,
  on_lcd_update_lap,
  on_lcd_update_run,
};

State _lap_split_btn[2][4] = {
  { NONE, IDLE, RUN , LAP },
  { NONE, IDLE, RUN, NONE},
};

State _start_stop_btn[2][4] = {
  {RUN, NONE, STOP, STOP},
  {RUN, RUN, STOP, STOP},
};

void time_set(Time *t,int serial){
  t->serial = serial;
  t->ms     = serial % 1000;
  serial   /= 1000;
  t->sec    = serial % 60;
  serial   /= 60;
  t->min    = serial % 60;
  serial   /= 60;
  t->hour   = serial % 60;
}

void time_to_string(Time *t,char *out){
  //sprintf("%d:%02d'%02d\"%02d",t->hour,t->min,t->sec,t->ms);
  out[0]  = ITOA(t->hour);
  out[1]  = ':';
  out[2]  = ITOA(t->min/10);
  out[3]  = ITOA(t->min%10);
  out[4]  = '\'';
  out[5]  = ITOA(t->sec/10);
  out[6]  = ITOA(t->sec%10);
  out[7]  = '\"';
  out[8]  = ITOA(t->ms/100);
  out[9]  = ITOA(t->ms%100/10);
}

void format(char *line,char state[5],Time *t){
  int i;
  for(i=0;i<5;i++) line[i] = state[i];
  line[5] = ' ';
  time_to_string(t,line+6);
}

void watch_clear_state(Watch *w){
  int i;
  w->time  = 0;
  w->lap   = 0;
  w->split = 0;
  w->start = 0;
  w->mode  = LAP_SPLIT;
  w->state = IDLE;
  w->is_mode_settled = 0;

  for(i=0;i<sizeof(w->line1);i++){
    w->line1[i] = 0;
    w->line2[i] = 0;
  }
}

void watch_init(Watch *w,TimerInterface *timer_interface){
    w->timer_interface = timer_interface;
    watch_clear_state(w);
}

void on_timer_tick(Watch *w){
  w->time++;
  if(w->time > TIMER_MAX){
    w->time = 0;
  }
}

void on_start_stop_btn_push(Watch *w){
  State next;

  if( (w->is_mode_settled == 0) &&
      (w->state == RUN) ){
    w->mode = START_STOP;
    w->is_mode_settled = 1;
  }

  next = _start_stop_btn[w->mode][w->state];
  if(next == NONE) return;
  w->state = next;
  on_state_change(w);
}

void on_lap_split_btn_push(Watch *w){
  State next;
  if( (w->is_mode_settled == 0) &&
      (w->state == RUN) ){
    w->mode = LAP_SPLIT;
    w->is_mode_settled = 1;
  }

  next = _lap_split_btn[w->mode][w->state];
  if(next == NONE) return;
  w->state = next;
  on_state_change(w);
}

void on_lcd_update(Watch *w){
  (*fv_on_lcd_update[w->state])(w);
}

void on_state_change(Watch *w){
  (*fv_on_state_change[w->state])(w);
}

void on_state_change_idle(Watch *w){
  watch_clear_state(w);
  w->timer_interface->stop_timer();
}

void on_state_change_run(Watch *w){
  if(w->mode == START_STOP){
    w->start = w->time;
  }
  w->timer_interface->start_timer();
}
void on_state_change_lap(Watch *w){
  if(w->mode == START_STOP) return;

  w->split += w->time;
  w->lap    = w->time;
  w->time   = 0;
}

void on_state_change_stop(Watch *w){
  w->timer_interface->stop_timer();
  if(w->mode == START_STOP){
  }else{
      w->lap    = w->time;
      w->split += w->lap;
      w->time   = 0;
  }
}

void on_lcd_update_idle(Watch *w){
  Time now;
  time_set(&now,w->time);

  format(w->line1,"  LAP",&now);
  format(w->line2,"SPLIT",&now);
}

void on_lcd_update_run(Watch *w){
  if(w->mode == START_STOP){
    Time now;
    Time start;
    time_set(&now,w->time);
    time_set(&start,w->start);
    format(w->line1,"START",&start);
    format(w->line2," STOP",&now);
  }else{
    Time split;
    time_set(&split,w->split+w->time);
    format(w->line1,"  LAP",&split);
    format(w->line2,"SPLIT",&split);
  }
}

void on_lcd_update_lap(Watch *w){
  if(w->mode == START_STOP){
    return;
  }else{
    Time lap,split;
    time_set(&lap,w->lap);
    time_set(&split,w->split);
    format(w->line1,"  LAP",&lap);
    format(w->line2,"SPLIT",&split);
  }
}

void on_lcd_update_stop(Watch *w){
  if(w->mode == START_STOP){
    Time now;
    Time start;
    time_set(&now,w->time);
    time_set(&start,w->start);
    format(w->line1,"START",&start);
    format(w->line2," STOP",&now);
  }else{
    Time lap,split;
    time_set(&lap,w->lap);
    time_set(&split,w->split);
    format(w->line1,"  LAP",&lap);
    format(w->line2,"SPLIT",&split);
  }
}
