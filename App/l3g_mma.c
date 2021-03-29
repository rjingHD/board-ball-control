#include "common.h"
#include "include.h"
#include "l3g_mma.h"
#include "project.h"

int16 l3g_x;
int16 l3g_y;
int16 l3g_z;
int16 accx,accy,accz;
int32 a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0,a8=0,a9=0,a10=0;

void l3g_mma_init()
{
  
   i2c_init(I2C0,400*1000);
    
   i2c_write_reg(I2C0,SlaveAddress,CTRL_REG1, 0x0f);   //
   i2c_write_reg(I2C0,SlaveAddress,CTRL_REG2, 0x00);  //
   i2c_write_reg(I2C0,SlaveAddress,CTRL_REG3, 0x08); //
   i2c_write_reg(I2C0,SlaveAddress,CTRL_REG4, 0x30); //+-2000dps
   i2c_write_reg(I2C0,SlaveAddress,CTRL_REG5, 0x00);
   
   i2c_write_reg(I2C0, MMA845x_IIC_ADDRESS, 0x2A, 0x00); 
   i2c_write_reg(I2C0, MMA845x_IIC_ADDRESS, 0x0E, 0x00);
   i2c_write_reg(I2C0, MMA845x_IIC_ADDRESS, 0x2A, 0x01); 
}

int16 l3g_zero=0;

void read_l3g_mma()
{
  uint8 data_gyro[6];
  uint8 data_acc[6];
  a2++;
  data_gyro[0]=i2c_read_reg(I2C0, SlaveAddress, OUT_X_L);
  data_gyro[1]=i2c_read_reg(I2C0, SlaveAddress, OUT_X_H);
  l3g_x=((data_gyro[0])|(data_gyro[1]<<8))-l3g_zero;
  data_gyro[2]=i2c_read_reg(I2C0, SlaveAddress, OUT_Y_L);
  data_gyro[3]=i2c_read_reg(I2C0, SlaveAddress, OUT_Y_H);
  l3g_y=((data_gyro[2])|(data_gyro[3]<<8))-l3g_zero;
  data_gyro[4]=i2c_read_reg(I2C0, SlaveAddress, OUT_Z_L);
  data_gyro[5]=i2c_read_reg(I2C0, SlaveAddress, OUT_Z_H);
  l3g_z=((data_gyro[4])|(data_gyro[5]<<8))-l3g_zero;

  a3++;
  data_acc[0]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 1);  //x
  a6++;
  data_acc[1]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 2);
  a7++;  
  data_acc[2]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 3);  //y
  a8++;
  data_acc[3]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 4);
  a9++;
  data_acc[4]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 5);   //z
  a10++;
  data_acc[5]=i2c_read_reg(I2C0, MMA845x_IIC_ADDRESS, 6);
  a4++;
  accx=((data_acc[0]<<8)|(data_acc[1]));
  accy=((data_acc[2]<<8)|(data_acc[3]));
  accz=((data_acc[4]<<8)|(data_acc[5]));
  a5++;
  accx>>=4;
  accy>>=4;
  accz>>=4;
  
}

float ang_acc_x=0.0;
float ang_speed_x=0;
float angle_x=6.8;
float ang_gyro_x=0;

float ang_acc_y=0.0;
float ang_speed_y=0;
float angle_y=6.8;
float ang_gyro_y=0;

float ang_acctan_x=0;
float ang_acctan_y=0;

float Filter_A=0.9960;//9960
float dt=0.0049999;

void hubu_fliter()
{
  a1++;
  read_l3g_mma();

  ang_acc_x=((180.0/3.14159)*(float)atan2((accx),(accz)));  
  ang_speed_x= (l3g_y-20)*0.06765845;
  ang_gyro_x = ang_gyro_x+ang_speed_x*dt;
  ang_gyro_x = (Filter_A)*ang_gyro_x + (1-Filter_A)*(ang_acc_x);
  angle_x=ang_gyro_x;
  
  ang_acc_y=((180.0/3.14159)*(float)atan2((accy),(accz)));  
  ang_speed_y= -(l3g_x-20)*0.06765845;
  ang_gyro_y = ang_gyro_y+ang_speed_y*dt;
  ang_gyro_y = (Filter_A)*ang_gyro_y + (1-Filter_A)*(ang_acc_y);
  angle_y=ang_gyro_y;
}

int angle_avg_count=0;
float angle_x_avr=0;
float anglel_x[5]={0};
float angle_y_avr=0;
float anglel_y[5]={0};
void hubu_fliter_avr()
{
    anglel_x[angle_avg_count]=angle_x;
    angle_x_avr=(anglel_x[0]+anglel_x[1]+anglel_x[2]+anglel_x[3]+anglel_x[4])/5;
    anglel_y[angle_avg_count]=angle_y;
    angle_y_avr=(anglel_y[0]+anglel_y[1]+anglel_y[2]+anglel_y[3]+anglel_y[4])/5;
    angle_avg_count++;
    if(angle_avg_count==5)
      angle_avg_count=0;
}
/**************************************直立环控制*****************************************/
float Ang_zero1=0; //10.3           //机械零位时的角度
float Ang_zero2=0;
float Ang_De_x=0;                  //实际角度与零位角度之差
float Ang_De_y=0;
float Ang_P=1000.0;              //直立环控制参数
float Ang_D=50.0;               
float Stand_P_Value=0,Stand_D_Value=0;
float StandControlPwm_x=0,StandControlPwm_y=0;
float last_Ang_Car=15.0;
int stop_flag=0;


float Stand_I_Value_x=0;
float Stand_I_Value_y=0;
float Ang_I=0;

void stand_ctrl()
{ 
  
  //car_angle_get();  //需要读6050，iic0
  //Angle_caculate(); //卡尔曼滤波的，还没计算好
  hubu_fliter();
  hubu_fliter_avr();
  Ang_De_x= angle_x_avr - Ang_zero1;  //Ang_De为小车偏离平衡时的角度，当小车处于静止平衡位置时调整Ang_Gyro使 Ang_Deviation为零，保证此时电机不动，该值需自己调试出来
  Ang_P=100;Ang_D=20;
//  switch(abs((int)Ang_De_x)>=1)
//  {
//  case 0:
// // Ang_P=0;
//  Ang_D=0;;break;
//  default:break;
//  }

  Stand_P_Value=Ang_P*(Ang_De_x);
  Stand_I_Value_x +=Ang_De_x*Ang_I;
  Stand_D_Value=ang_speed_x*Ang_D;
  StandControlPwm_x=-(Stand_P_Value+Stand_I_Value_x+Stand_D_Value);
  
  Ang_De_y= angle_y_avr - Ang_zero2;  //Ang_De为小车偏离平衡时的角度，当小车处于静止平衡位置时调整Ang_Gyro使 Ang_Deviation为零，保证此时电机不动，该值需自己调试出来
  Ang_P=500;Ang_D=20;
//  switch(abs((int)Ang_De_y)>=1)
//  {
//  case 0:
//   // Ang_P=0;
//    Ang_D=0;break;
//  default:break;
//  }

  Stand_P_Value=Ang_P*(Ang_De_y);
  Stand_I_Value_y +=Ang_De_y*Ang_I;
  Stand_D_Value=ang_speed_y*Ang_D;
  StandControlPwm_y=(Stand_P_Value+Stand_I_Value_y+Stand_D_Value);
}

//  uint32 left,right;
//void Motor_Ctrl(int32 Left_Motor,int32 Right_Motor)
//{   
//   
//
//   if(Left_Motor>=0)
//   { 
//     //left=Left_Motor+left_dd_front;      //加死区电压为了直立环调试方便
//     left=(Left_Motor+650);
//     ftm_pwm_duty(FTM0, FTM_CH0, 0);
//     ftm_pwm_duty(FTM0, FTM_CH1, left);
//   }
//   else if(Left_Motor<0)
//   {
//     //left=-(Left_Motor+left_dd_back);
//     left=-1*Left_Motor+650;
//     ftm_pwm_duty(FTM0, FTM_CH0, left);
//     ftm_pwm_duty(FTM0, FTM_CH1, 0);
//   }
//   
//     if(Right_Motor>=0)
//   { 
//     //right=Right_Motor+right_dd_front;
//     right=Right_Motor+650;
//     ftm_pwm_duty(FTM0, FTM_CH2, right);
//     ftm_pwm_duty(FTM0, FTM_CH3, 0);
//   }
//   else if(Right_Motor<0)
//   {
//     //right=-(Right_Motor+right_dd_back);
//     right=(-1)*Right_Motor+650;
//     ftm_pwm_duty(FTM0, FTM_CH2, 0);
//     ftm_pwm_duty(FTM0, FTM_CH3, right);
//   }
//   
//}


//float angxerr=0.0,angyerr=0.0;
//
//float xp[3]={10,20,60};
//float yp[3]={12,24,72};
//
//float turnpwmx=0.0;
//float turnpwmy=0.0;
//
//float turnx_controlspeed(float pointxset)
//{
// 
//   // static float  DerivativeGain=0.024;        //给定微分系数Td //hxd
//   // static float  ControlPeriod=0.02;        //控制周期T
//    static float  FrontErrorK_1=0.0;          //前一次误差值e(k-1)
//    
//    float Kp,Kd,CurrentError,AdjustValue;     
//    CurrentError=pointxset-ball_center_w;
//    //if(fabs(CurrentError)<15&&fabs(CurrentError)>3)
//    Kp=0.1;
//    //else if(fabs(CurrentError)<=3)
//     // Kp=1.0;
//    //else if(fabs(CurrentError)>=15&&fabs(CurrentError)<35) Kp=1;
//    //else Kp=0.5;
//    Kd=0.2;           //计算微分系数Td/T
//    if(time<=0.3) Kd=0;
//       
//   
//       AdjustValue=CurrentError*Kp+(CurrentError-FrontErrorK_1)*Kd;        //按照位置PID算法计算输出值
//       
//       FrontErrorK_1=CurrentError;                          //将当前误差赋给前一次
//
//    return  AdjustValue;
//}
//
//
//float turny_controlspeed(float pointyset)
//{
//   // static float  DerivativeGain=0.024;        //给定微分系数Td //hxd
//   // static float  ControlPeriod=0.02;        //控制周期T
//    static float  FrontErrorK_1=0.0;          //前一次误差值e(k-1)
//    
//    float Kp ,Kd,CurrentError,AdjustValue;     
//    CurrentError=pointyset-ball_center_h;
//  //if(fabs(CurrentError)<15&&fabs(CurrentError)>5)
//    Kp=0.1;
//    //else if(fabs(CurrentError)<=5)
//      //Kp=1.2;
//    //else if(fabs(CurrentError)>=15&&fabs(CurrentError)<35) Kp=1;
//    //else Kp=0.5;
//    Kd=0.2;           //计算微分系数Td/T
//    if(time<=0.3) Kd=0;
//       
//                                      
//       AdjustValue=CurrentError*Kp+(CurrentError-FrontErrorK_1)*Kd;        //按照位置PID算法计算输出值
//       
//       FrontErrorK_1=CurrentError;                          //将当前误差赋给前一次
//
//    return  AdjustValue;
//}
//
//float speedset_x=0.0;
//float speedset_y=0.0;
//
//float turnx_controlangle()
//{
//    static float  ProportionalGain=400;          //给定比例系数Kp  
//   // static float  DerivativeGain=0.024;        //给定微分系数Td //hxd
//   // static float  ControlPeriod=0.02;        //控制周期T
//    static float  FrontErrorK_1=0.0;          //前一次误差值e(k-1)
//    static float  FrontErrorK_2=0.0;          //前前次误差值e(k-2)
//    static float  AddAdjust_Value=0.0;
//    
//    float  Ka,Kb,Kc,Ki,Kd,CurrentError,AdjustValue;     
//    CurrentError=speedset_x-speed_now_x;
//    if(speedset_x<1)
//    Ki=3;
//    else Ki=0;
//    Kd=0;           //计算微分系数Td/T
//        
//    Ka =ProportionalGain+Kd+Ki;                //增量PID当前误差系数
//    Kb =-ProportionalGain-2*Kd;                //增量PID前一次误差系数
//    Kc =Kd;                                    //增量PID前前次误差系数
//    if((xset-lastxset)!=0)
//    { AdjustValue=0;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=0;              //将前一次的误差赋给前前次
//       FrontErrorK_1=0;                          //将当前误差赋给前一次
//    }
//    
//    
//       else if(start_flag==0)
//    {
//      AdjustValue=0;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=0;              //将前一次的误差赋给前前次
//       FrontErrorK_1=0;                          //将当前误差赋给前一次
//    }
//    
//    
//    else 
//    { AdjustValue=AddAdjust_Value+CurrentError*Ka+FrontErrorK_1*Kb+FrontErrorK_2*Kc;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=FrontErrorK_1;              //将前一次的误差赋给前前次
//       FrontErrorK_1=CurrentError;
//    }
//       //将当前误差赋给前一次
//    AddAdjust_Value=AdjustValue;
//    return  AdjustValue;
//}
//
//float turny_controlangle()
//{
//    static float  ProportionalGain=400;          //给定比例系数Kp  
//  // static float  DerivativeGain=0.024;        //给定微分系数Td //hxd
//   // static float  ControlPeriod=0.02;        //控制周期T
//    static float  FrontErrorK_1=0.0;          //前一次误差值e(k-1)
//    static float  FrontErrorK_2=0.0;          //前前次误差值e(k-2)
//    static float  AddAdjust_Value=0.0;
//    
//    float  Ka,Kb,Kc,Ki,Kd,CurrentError,AdjustValue;     
//    CurrentError=speedset_y-speed_now_y;
//     if(speedset_y<1)
//    Ki=3;
//    else Ki=0;
//    Kd=0;           //计算微分系数Td/T
//    
//
//    Ka =ProportionalGain+Kd+Ki;                //增量PID当前误差系数
//    Kb =-ProportionalGain-2*Kd;                //增量PID前一次误差系数
//    Kc =Kd;                                    //增量PID前前次误差系数
//      if((yset-lastyset)!=0)
//    { AdjustValue=0;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=0;              //将前一次的误差赋给前前次
//       FrontErrorK_1=0;                          //将当前误差赋给前一次
//    }
//    
//    else if(start_flag==0)
//    {
//      AdjustValue=0;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=0;              //将前一次的误差赋给前前次
//       FrontErrorK_1=0;                          //将当前误差赋给前一次
//    }
//    
//    else 
//    { AdjustValue=AddAdjust_Value+CurrentError*Ka+FrontErrorK_1*Kb+FrontErrorK_2*Kc;        //按照增量式PID算法计算输出值
//       FrontErrorK_2=FrontErrorK_1;              //将前一次的误差赋给前前次
//       FrontErrorK_1=CurrentError;
//    }
//    AddAdjust_Value=AdjustValue;
//    return  AdjustValue;
//}
//
//void turn_control()
//{
//  static float outx=0,outy=0;
//  
//  turnpwmx = turnx_controlangle();
//  turnpwmy = turny_controlangle();
//    
//  outx=turnpwmx+midservox;
//  outy=turnpwmy-midservoy;
//  
//  if(outx>maxservox) outx=maxservox;
//  else if(outx<minservox) outx=minservox;  
//  if(outy>maxservoy) outy=maxservoy;
//  else if(outy<minservoy) outy=minservoy;
//  
//  ftm_pwm_duty(FTM0, FTM_CH0,(uint32)outx);
//  ftm_pwm_duty(FTM0, FTM_CH1,(uint32)outy);
//}