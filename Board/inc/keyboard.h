#include "include.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

extern uint8  mode_state;
extern uint8  speed_mode;
extern uint8  keynum;  
extern uint8  display_flag;
extern uint8  display_status;
extern uint8  debug_flag;
extern uint8  ring_status1;
extern uint8  ring_status2;
extern uint8  ring_status3;
extern uint8  ring_status;
extern signed int  deltaKp ;
extern signed int  deltaTdin ;
extern signed int  deltaTdout ;
extern int speedset;
extern int deltax;
extern uint8 ringleft1;
extern uint8 ringleft2;
extern uint8 ringleft3;
extern uint8 ringright1;
extern uint8 ringright2;
extern uint8 ringright3;


extern void PORTC_IRQHandler();
extern void keyboardandswitch_init();
extern void OLED_display();
extern void key_set(); 
#endif

