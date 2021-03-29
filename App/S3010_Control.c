#include "include.h"
#include "project.h"
#include "picture.h"
#include "Speed_Control.h"
#include "S3010_Control.h"
#define QEP_time 5000
//舵机相关的宏定义
#define S3010_FTM   FTM0
#define S3010_CH    FTM_CH5                             //舵机初始化PTA12 
#define S3010_HZ    (100)

#define ServeKp_numL 80
#define ServeKp_numR 80
#define row_num      9

//#define S_D5_centervalue    3920 //3827
#define Control_left_limit  4525//4350
#define Control_right_limit 3390 //3350 


/*****************舵机相关***********/
float row_percent[row_num] = {0.454,0.32,0.151,0.05,0.005,0.005,0.005,0.005,0.005};
int deltax = 0;

int deltax_error;
int pre_deltax = 0; //远处路况检测
int S3010_row = 0;
int Control = S_D5_centervalue;
int Control_error =0;
int radius = 0;    //弯道半径
float curvature = 0; //曲率


/****************************
速度设定
*****************************/
int mode1= 0;
int mode2= 0;
int mode3= 0;
int mode4= 0;
/*****************************
环形弯转向设定
*****************************/
int ring_direction1 = 0;
int ring_direction2 = 0;






void Key_Init(void)
{
#define KEY1 gpio_get(PTE27)
#define KEY2 gpio_get(PTE26)
#define KEY3 gpio_get(PTE25)
#define KEY4 gpio_get(PTE24)

    if(KEY1==0&&KEY2==0)
    {
       DELAY_MS(10);
       if(KEY1==0&&KEY2==0)
       {
          mode1 = 1;
       }
    }
    if(KEY1==0&&KEY2==1)
    {
       DELAY_MS(10);
       if(KEY1==0&&KEY2==1)
       {
          mode2 = 1;
       }
    }
    if(KEY1==1&&KEY2==0)
    {
       DELAY_MS(10);
       if(KEY1==1&&KEY2==0)
       {
          mode3 = 1;
       }
    }
    if(KEY1==1 && KEY2==1)
    {
       DELAY_MS(10);
       if(KEY1==1&&KEY2==1)
       {
         mode4 = 1;
       }
    }
    if(KEY3==0)
    {
       DELAY_MS(10);
       if(KEY3==0)
        ring_direction1 = 0;
    }
    if(KEY3==1)
    {
       DELAY_MS(10);
       if(KEY3==1)
         ring_direction1 = 1;
    }
    if(KEY4== 0)
    {
       DELAY_MS(10);
       if(KEY4==0)
         ring_direction2 = 0;
    }
    if(KEY4== 1)
    {
       DELAY_MS(10);
       if(KEY4==1)
        ring_direction2 = 1; 
    }
}

             
void S3010_control()
{
   // float ServeKpL[ServeKp_numL] = {0,1.91,2.68,2.54,5.49,5.52,5.64,5.85,6.15,7.53,9,9.56,9.20,11.93,12.75,12.66,11.65,11.73,11.90,11.16,11.50,11.93,15.43,15,14.63,14.31,14.04,13.81,13.61,13.45,13.32,13.21,13.11,13.03,12.95,12.87,12.79,12.70,12.60,12.49,12.38,12.26,12.13,12.01,11.88,11.76,11.63,11.51,11.39,11.27,11.16,11.05,10.94,10.83,10.73,10.63,10.54,10.44,10.35,10.27,10.19,10.11,10.04,9.980,9.920,9.870,9.830,9.790,9.760,8.740,8.720,8.720,8.720,8.730,8.750,8.780,8.820,8.870,8.930};
   //float ServeKpR[ServeKp_numR] = {0,1.91,2.68,2.54,5.49,5.52,5.64,5.85,6.15,7.53,9,9.56,9.20,11.93,12.75,12.66,11.65,11.73,11.90,11.16,11.50,11.93,15.43,15,14.63,14.31,14.04,13.81,13.61,13.45,13.32,13.21,13.11,13.03,12.95,12.87,12.79,12.70,12.60,12.49,12.38,12.26,12.13,12.01,11.88,11.76,11.63,11.51,11.39,11.27,11.16,11.05,10.94,10.83,10.73,10.63,10.54,10.44,10.35,10.27,10.19,10.11,10.04,9.980,9.920,9.870,9.830,9.790,9.760,8.740,8.720,8.720,8.720,8.730,8.750,8.780,8.820,8.870,8.930};
      
  
   float ServeKpL[ServeKp_numL] = {0.00 ,0.41 ,0.82 ,1.23 ,1.64 ,2.05 ,2.46 ,2.87 ,3.28 ,3.69 ,4.10 ,4.51 ,4.92 ,5.33 ,5.74 ,6.15 ,6.56 ,6.97 ,7.38 ,7.79 ,8.20, 8.44 ,10.21 ,10.01 ,9.85 ,9.71 ,9.59 ,9.50 ,9.42 ,9.36 ,9.31 ,9.28 ,9.25 ,9.23 ,9.21 ,9.19 ,9.25 ,9.30 ,9.35 ,9.40 ,9.44 ,9.43 ,10.42 ,10.41 ,10.39 ,10.38 ,10.37 ,10.36 ,10.35 ,10.34 ,11.22 ,11.11 ,11.00 ,10.89 ,10.78 ,10.68 ,10.59 ,10.49 ,10.40 ,10.31 ,10.23 ,10.15 ,10.08 ,9.26 ,8.45 ,8.40 ,8.35 ,8.31 ,8.28 ,7.75 ,7.23 ,7.22 ,7.22 ,7.23 ,7.24 ,7.27 ,9.46 ,9.54 ,9.62 ,9.69 };
//   float ServeKpR[ServeKp_numR] = {0,1.91,2.68,2.54,2.49,3.52,4.64,5.85,6.15,7.53,7,7.56,7.2,9.93,10.75,10.66,9.65,9.73,9.9,9.16,9.5,9.93,13.43,13,12.63,12.31,12.04,11.81,11.61,11.45,11.32,11.21,11.11,11.03,10.95,10.87,10.79,10.7,10.6,10.49,10.38,10.26,12.13,12.01,11.88,11.76,11.63,11.51,11.39,11.27,11.16,11.05,10.94,10.83,10.73,10.63,10.54,10.44,10.35,10.27,10.19,10.11,10.04,8.48,8.42,8.37,8.33,8.29,8.26,7.24,7.22,7.22,7.22,7.23,7.25,7.28,7.32,7.37,7.43,7.47};
//   float ServeKpR[ServeKp_numR] = {0.13,2.04,2.81,2.67,2.62,3.65,4.77,5.98,6.28,7.66,7.13,7.69,7.33,10.15,10.97,10.88,9.87,9.95,10.12,9.38,9.72,10.15,13.57,13.14,12.77,12.45,12.18,11.95,11.75,11.59,11.46,11.35,11.25,11.17,11.09,11.01,10.93,10.84,10.74,10.63,10.52,10.4,12.27,12.15,12.02,11.9,11.77,11.65,11.53,11.41,11.3,11.19,11.08,10.97,10.87,10.77,10.68,10.58,10.49,10.41,10.33,10.25,10.18,8.64,8.58,8.53,8.49,8.45,8.42,7.4,7.38,7.38,7.38,7.39,7.41,7.44,7.48,7.53,7.59,7.63};
                          //         0    1      2     3     4     5     6     7     8     9    10     11    12    13   14     15    16    17    18   19     20   21     22     23    24     25    26    27    28    29    30   31    32     33    34    35   36    37    38    39    40    41     42     43     44     45     46     47     48     49     50     51     52     53     54    55      56     57     58     59     60     61     62     63    64   65    66     67    68    69   70    71    72    73    74    75     76   77    78    79  
   float ServeKpR[ServeKp_numR] = {0.00 ,0.41 ,0.82 ,1.23 ,1.64 ,2.05 ,2.46 ,2.87 ,3.28 ,3.69 ,4.10 ,4.51 ,4.92 ,5.33 ,5.74 ,6.15 ,6.56 ,6.97 ,7.38 ,7.79 ,8.20, 8.44 ,10.21 ,10.01 ,9.85 ,9.71 ,9.59 ,9.50 ,9.42 ,9.36 ,9.31 ,9.28 ,9.25 ,9.23 ,9.21 ,9.19 ,9.25 ,9.30 ,9.35 ,9.40 ,9.44 ,9.43 ,10.42 ,10.41 ,10.39 ,10.38 ,10.37 ,10.36 ,10.35 ,10.34 ,11.22 ,11.11 ,11.00 ,10.89 ,10.78 ,10.68 ,10.59 ,10.49 ,10.40 ,10.31 ,10.23 ,10.15 ,10.08 ,9.26 ,8.45 ,8.40 ,8.35 ,8.31 ,8.28 ,7.75 ,7.23 ,7.22 ,7.22 ,7.23 ,7.24 ,7.27 ,9.46 ,9.54 ,9.62 ,9.69 };

    
   float Serve_Kd_in = 50;
   float Serve_Kd_out = 45;
    if(abs(deltax)<=20)
    {  
         ServeKpR[abs(deltax)]+=0.2;
            ServeKpL[abs(deltax)]+=0.5;  
    }
    if(mode1 == 1)
    {  
        Serve_Kd_in = 55;
        Serve_Kd_out = 46;
        if(abs(deltax)<35)
        {
            Serve_Kd_in  =  45;
            Serve_Kd_out =  37;
        }
        if(abs(deltax)>10 && abs(deltax)<60) 
        {
            ServeKpR[abs(deltax)]+=0.2;
            ServeKpL[abs(deltax)]+=0.5;
        }
        if(abs(deltax)>60)
        {
          //  ServeKpR[abs(deltax)]+=1.5;
           // ServeKpL[abs(deltax)]+=2.2;
        }
        if(ring_flag)
        {
           Serve_Kd_in  =  30;
           Serve_Kd_out =  30;
        }
        if(obstacle_flag)
        {
          ServeKpR[abs(deltax)] = 8;
          ServeKpR[abs(deltax)] = 8;
          Serve_Kd_in  =  18;
          Serve_Kd_out =  20;
        }
     }
     if(mode2 == 1)
     { 
        Serve_Kd_in = 55;
        Serve_Kd_out = 46;
        if(abs(deltax)<35)
        {
            Serve_Kd_in  =  45;
            Serve_Kd_out =  37;
        }
        if(abs(deltax)>10 && abs(deltax)<60) 
        {
            ServeKpR[abs(deltax)]+=0.2;
            ServeKpL[abs(deltax)]+=0.5;
        }
        if(abs(deltax)>60)
        {
          //  ServeKpR[abs(deltax)]+=1.5;
           // ServeKpL[abs(deltax)]+=2.2;
        }
        if(ring_flag)
        {
           Serve_Kd_in  =  30;
           Serve_Kd_out =  30;
        }
        if(obstacle_flag)
        {
          ServeKpR[abs(deltax)] = 8;
          ServeKpR[abs(deltax)] = 8;
          Serve_Kd_in  =  18;
          Serve_Kd_out =  20;
        }
     }
     if(mode3 == 1)
     { 
         Serve_Kd_in = 55;
        Serve_Kd_out = 46;
        if(abs(deltax)<35)
        {
            Serve_Kd_in  =  45;
            Serve_Kd_out =  37;
        }
        if(abs(deltax)>10 && abs(deltax)<35) 
        {
            ServeKpR[abs(deltax)]+=0.3;
            ServeKpL[abs(deltax)]+=0.5;
        }
                if(abs(deltax)>34 && abs(deltax)<60) 
        {
            ServeKpR[abs(deltax)]+=0.5;
            ServeKpL[abs(deltax)]+=0.7;
        }
        if(abs(deltax)>60)
        {
          //  ServeKpR[abs(deltax)]+=1.5;
           // ServeKpL[abs(deltax)]+=2.2;
        }
        if(ring_flag)
        {
           Serve_Kd_in  =  30;
           Serve_Kd_out =  30;
        }
        if(obstacle_flag)
        {
          ServeKpR[abs(deltax)] = 8;
          ServeKpR[abs(deltax)] = 8;
          Serve_Kd_in  =  18;
          Serve_Kd_out =  20;
        }
     }
     if(mode4==1)
     {
        Serve_Kd_in = 50;
        Serve_Kd_out = 45;
        if(abs(deltax)<35)
        {
            Serve_Kd_in  =  40;
            Serve_Kd_out =  35;
        }
        if(abs(deltax)>10 && abs(deltax)<60) 
        {
            ServeKpR[abs(deltax)]+=0.3;
            ServeKpL[abs(deltax)]+=0.3;
        }
        if(abs(deltax)>60)
        {
            ServeKpR[abs(deltax)]+=2.2;
            ServeKpL[abs(deltax)]+=1.5;
        }
        if(ring_flag)
        {
           Serve_Kd_in  =  30;
           Serve_Kd_out =  30;
        }
        if(obstacle_flag)
        {
          ServeKpR[abs(deltax)] = 8;
          ServeKpR[abs(deltax)] = 8;
          Serve_Kd_in  =  20;
          Serve_Kd_out =  20;
        }
     }
    int i = 0;
    int j = 0;
    int deltax_last = 0; //上一次的偏差
    
    float center_error= 0; //浮点型中线加权量
    deltax_last  = deltax;
    if(effctline>=0)
    {
        if(effctline>H-10)
        {
            for(i=effctline;i>=0;i--)
               centerline[i] = centerline[H-1];
        }
        else if(effctline<=35&&abs(centerline[effctline]-W/2)<10&&ring_flag==0)
        {
            for(i=effctline;i>=0;i--)
               centerline[i] = W/2;
        }
        else if(centerline[H-1]<centerline[effctline])
        {
            for(i=effctline;i>=0;i--)
               centerline[i] = W-1;
        }
        else
        {
           if(centerline[H-1]>centerline[effctline])
           {
              for(i=effctline;i>=0;i--)
               centerline[i] = 1;
           }
        }
    }
    for(i= 15;i< 67; i++)
    {
        if(i==23 || i==25 || i==27 || i==35 || i==37 || i==39 || i==60 || i==63 || i==66 )
        {
            center_error += (W/2-centerline[i])*row_percent[j];
            j++;
        }

    }
    deltax = (int)(center_error);
    //路径优化
    if((zero_point_get(1,H-2,centerline)==3||zero_point_get(1,H-2,centerline)==4)&&effctline<=5&&abs(W/2-centerline[5])<=30 \
       &&dispersion_degree(15,2)>=4 && dispersion_degree(15,2)<=20&&!ring_flag&&!obstacle_flag&&!stopcar())
    {
        //printf("s弯\n");
        deltax= (int)(W/2-(get_left_max(effctline+5)+get_right_min(effctline+5))/2);
    }
    Control_error=deltax-deltax_last;
    if (deltax>= W/2) deltax = W/2;
    if (deltax<= -W/2) deltax = -W/2;
    if(deltax> 0)
    {
        if(deltax*(deltax-deltax_last)>0) 
        { 
            Control = (int)(ServeKpR[abs(deltax)]*deltax + Serve_Kd_in*(deltax-deltax_last))+S_D5_centervalue;
        }
        else if(deltax*(deltax-deltax_last)<= 0) 
        {
            Control = (int)(ServeKpR[abs(deltax)]*deltax + Serve_Kd_out*(deltax-deltax_last))+S_D5_centervalue;
        }
    }
    else if(deltax<= 0)
    {
        if(deltax*(deltax-deltax_last)>0)
        {
            Control = (int)(ServeKpL[abs(deltax)]*deltax + Serve_Kd_in*(deltax-deltax_last))+S_D5_centervalue;
        }
        else if(deltax*(deltax-deltax_last)<= 0)
        {
            Control = (int)(ServeKpL[abs(deltax)]*deltax + Serve_Kd_in*(deltax-deltax_last))+S_D5_centervalue;
        }   
    }
    if(deltax>= ServeKp_numR-1) Control = Control_left_limit;
    if(deltax<= -ServeKp_numR+1) Control = Control_right_limit;
    if(Control>Control_left_limit)  Control=Control_left_limit;
    if(Control<Control_right_limit)   Control=Control_right_limit;
    if(stop==0&&stopcar() && stop_line>=50) 
    {
      Control = S_D5_centervalue;
      DELAY_MS(100);
    }
    if(stop&&stopcar()&& stop_line>=50)
    {
       Control = S_D5_centervalue;
       DELAY_MS(100);
    }
    ftm_pwm_duty(S3010_FTM, S3010_CH,Control);
}
