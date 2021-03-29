#include "common.h"
#include "include.h"
#include "pid.h"
//#include "all_init.h"
//#include "main.h"
#include "project.h"
#include "picture.h"
//#include "l3g_mma.h"

float angxerr=0.0,angyerr=0.0;

float xp[3]={10,20,60};
float yp[3]={12,24,72};

float turnpwmx=0.0;
float turnpwmy=0.0;
float turnx_controlspeed(float pointxset)
{
 
   // static float  DerivativeGain=0.024;        //����΢��ϵ��Td //hxd
   // static float  ControlPeriod=0.02;        //��������T
    static float  FrontErrorK_1=0.0;          //ǰһ�����ֵe(k-1)
    
    float Kp ,Kd,CurrentError,AdjustValue;     
    CurrentError=pointxset-point_x;
    //if(fabs(CurrentError)<15&&fabs(CurrentError)>3)
    Kp=0.1;  //0.1
    //else if(fabs(CurrentError)<=3)
     // Kp=1.0;
    //else if(fabs(CurrentError)>=15&&fabs(CurrentError)<35) Kp=1;
    //else Kp=0.5;
    Kd=0.2;           //����΢��ϵ��Td/T
    if(time<=0.3) Kd=0;
       
   
       AdjustValue=CurrentError*Kp+(CurrentError-FrontErrorK_1)*Kd;        //����λ��PID�㷨�������ֵ
       
       FrontErrorK_1=CurrentError;                          //����ǰ����ǰһ��

    return  AdjustValue;
}


float turny_controlspeed(float pointyset)
{
   // static float  DerivativeGain=0.024;        //����΢��ϵ��Td //hxd
   // static float  ControlPeriod=0.02;        //��������T
    static float  FrontErrorK_1=0.0;          //ǰһ�����ֵe(k-1)
    
    float Kp ,Kd,CurrentError,AdjustValue;     
    CurrentError=pointyset-point_y;
  //if(fabs(CurrentError)<15&&fabs(CurrentError)>5)
    Kp=0.1;  //0.1
    //else if(fabs(CurrentError)<=5)
      //Kp=1.2;
    //else if(fabs(CurrentError)>=15&&fabs(CurrentError)<35) Kp=1;
    //else Kp=0.5;
    Kd=0.2;           //����΢��ϵ��Td/T
    if(time<=0.3) Kd=0;
       
                                      
       AdjustValue=CurrentError*Kp+(CurrentError-FrontErrorK_1)*Kd;        //����λ��PID�㷨�������ֵ
       
       FrontErrorK_1=CurrentError;                          //����ǰ����ǰһ��

    return  AdjustValue;
}

float xwantspeed=0.0;
float ywantspeed=0.0;

float turnx_controlangle()
{
    static float  ProportionalGain=300;          //��������ϵ��Kp///////////////////// shang 
   // static float  DerivativeGain=0.024;        //����΢��ϵ��Td //hxd
   // static float  ControlPeriod=0.02;        //��������T
    static float  FrontErrorK_1=0.0;          //ǰһ�����ֵe(k-1)
    static float  FrontErrorK_2=0.0;          //ǰǰ�����ֵe(k-2)
    static float  AddAdjust_Value=0.0;
    
    float  Ka,Kb,Kc,Ki,Kd,CurrentError,AdjustValue;
  //  xwantspeed=0;
    CurrentError=xwantspeed-speedx_now;
    if(xwantspeed<1)
    Ki=5.5;/////////////////////////////////////////////////////////////////////////////shang
    else Ki=0;
    Kd=0;           //����΢��ϵ��Td/T
    
    

    
    
    
    Ka =ProportionalGain+Kd+Ki;                //����PID��ǰ���ϵ��
    Kb =-ProportionalGain-2*Kd;                //����PIDǰһ�����ϵ��
    Kc =Kd;                                    //����PIDǰǰ�����ϵ��
    if((xset-lastxset)!=0)
    { AdjustValue=0;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=0;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=0;                          //����ǰ����ǰһ��
    }
    
    
       else if(start_flag==0)
    {
      AdjustValue=0;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=0;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=0;                          //����ǰ����ǰһ��
    }
    
    
    else 
    { AdjustValue=AddAdjust_Value+CurrentError*Ka+FrontErrorK_1*Kb+FrontErrorK_2*Kc;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=FrontErrorK_1;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=CurrentError;
    }
       //����ǰ����ǰһ��
    AddAdjust_Value=AdjustValue;
    return  AdjustValue;
}

float turny_controlangle()
{
    static float  ProportionalGain=300;          //��������ϵ��Kp //////////////////////xia 
  // static float  DerivativeGain=0.024;        //����΢��ϵ��Td //hxd
   // static float  ControlPeriod=0.02;        //��������T
    static float  FrontErrorK_1=0.0;          //ǰһ�����ֵe(k-1)
    static float  FrontErrorK_2=0.0;          //ǰǰ�����ֵe(k-2)
    static float  AddAdjust_Value=0.0;
    
    float  Ka,Kb,Kc,Ki,Kd,CurrentError,AdjustValue;
  //  ywantspeed=0;
    CurrentError=ywantspeed-speedy_now;
     if(ywantspeed<1)
    Ki=5.5;/////////////////////////////////////////////////////////////////////////////xia
    else Ki=0;
    Kd=0;           //����΢��ϵ��Td/T
    

    Ka =ProportionalGain+Kd+Ki;                //����PID��ǰ���ϵ��
    Kb =-ProportionalGain-2*Kd;                //����PIDǰһ�����ϵ��
    Kc =Kd;                                    //����PIDǰǰ�����ϵ��
      if((yset-lastyset)!=0)
    { AdjustValue=0;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=0;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=0;                          //����ǰ����ǰһ��
    }
    
    else if(start_flag==0)
    {
      AdjustValue=0;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=0;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=0;                          //����ǰ����ǰһ��
    }
    
    else 
    { AdjustValue=AddAdjust_Value+CurrentError*Ka+FrontErrorK_1*Kb+FrontErrorK_2*Kc;        //��������ʽPID�㷨�������ֵ
       FrontErrorK_2=FrontErrorK_1;              //��ǰһ�ε�����ǰǰ��
       FrontErrorK_1=CurrentError;
    }
    AddAdjust_Value=AdjustValue;
    return  AdjustValue;
}

void turn_control()
{
  static float outx=0,outy=0;

 
  turnpwmx=turnx_controlangle();
  turnpwmy=turny_controlangle();
    
  outx=turnpwmx+midservox;
  outy=-turnpwmy+midservoy;
  
  if(outx>maxservox) outx=maxservox;
  else if(outx<minservox) outx=minservox;
  
  if(outy>maxservoy) outy=maxservoy;
  else if(outy<minservoy) outy=minservoy;
  
  ftm_pwm_duty(FTM0, FTM_CH0,(uint32)outx);
  ftm_pwm_duty(FTM0, FTM_CH1,(uint32)outy);
}