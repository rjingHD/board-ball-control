#include "include.h"
#ifndef PROJECT_H
#define PROJECT_H

#define max(a,b)((a)>(b)?(a):(b))
#define min(a,b)((a)>(b)?(b):(a))
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT0_IRQHandler();

#define midservox         17200
#define maxservox         18400 
#define minservox         15400

#define midservoy         13900
#define maxservoy         15500 
#define minservoy         12500

void x_yset_caculate();
void x_yset_caculate_jia();
void point_init();
extern float a[3],w[3],ang[3],T;
extern uint8 key_value;
extern float xset,yset;
extern int8 start_flag;
extern float time;
extern float speedx_now,speedy_now;
extern uint16 left,right,up,down;
extern float  lastxset,lastyset;
 
#endif