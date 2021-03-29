#ifndef __pid_H__
#define __pid_H__


extern float PID_Control_Wheel(float GivenValue,float MeasureValue);//ÔöÁ¿Ê½pid

void motor_output();

void stand_ctrl();
void turn_control();
void angerr_caculate();
extern float angxerr,angyerr;
float turnx_controlspeed(float pointxset);
float turny_controlspeed(float pointyset);
extern float   xwantspeed,ywantspeed;
#endif