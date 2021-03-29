#include "common.h"
#include "include.h"
#include "math.h"
#include "project.h"
#include "picture.h"
#include "pid.h"
uint16 left=0,right=0,up=0,down=0;

int16 point_position[13][2]={{37,73},{35,151},{36,225},{111,72},{110,149},
                             {107,227},{188,73},{188,151},{183,227},
                             {72,150},{110,110},{149,150},{109,188}};//25,45,58,56

float var[3]={0};
float anglast[3]={0};
float angxz=1;
unsigned char Re_buf[11],counter=0;
uint32 sign=0;

float speedx_now=0,speedy_now=0;
float lastx=0,lasty=0;

uint8 key_value=0;
int16  model=0;                     //ģʽ��������
uint16 clear_time=0;
uint16 lcdinittime=0;
int8 start_flag=0;           //��ʼ��־��������ʼ����
float time=0;                //�ӿ�ʼ������ʱ��ʼ���뵥λ

uint32 gg=0;

float xset=79,yset=60;    //�趨xy����
float lastxset=0,lastyset=0;
float stoperr=4;        //ͣ���������������ƫ��
int16 delaytime=8;

/******ģʽһ��Ҫ�ı���*****/
int16 mod1_2[2]={35,151};     //1Ϊx���꼴�У�0Ϊy���꼴�� ,ģʽһ��2��λ���꣬����2�ż���

/***********model2***********/
int16 mod2_5[2]={110,149};     //0y1x��ģʽ�������λ�����趨��һ��λ�����������λ

/******************model3*************************/
int16 mod3_4[2]={111,72};     //�ĺ�λ����
int16 mod3_4time=0;        //��4��λͣ��ʱ���ʱ
int16 mod3_5[2]={110,149};     //5 zuobiao

/****************model4*cong1��9*************************/
int16 mod4_45[2]={110,110};   //����֮�������
int16 mod4_45time=0;
int16 mod4_9[2]={183,227};    //9zuobiao
/**************model5*********1-2-6-9(stop)******************/
int16 mod5_2[2]={35,151};
int16 mod5_2time=0;
int16 mod5_6[2]={107,227};
int16 mod5_6time=0;
int16 mod5_9[2]={183,227};

/*********************model6*********************/
//{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}
int16 mod6_1[2]={110,110};//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
int16 mod6_1time=0;
int16 mod6_2[2]={149,150};//5 : {118,159}
int16 mod6_2time=0;
int16 mod6_3[2]={109,188};
int16 mod6_3time=0;
int16 mod6_4[2]={72,150};
int16 mod6_4time=0;
int16 mod6_4time2=0;
int16 mod6_cnt=0;   
/*******************model7*********����5���˶�*********/
int16 mod7_45[2]={110,110};//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
int16 mod7_45time=0;
int16 mod7_58[2]={149,150};//5 : {118,159}
int16 mod7_58time=0;
int16 mod7_56[2]={109,188};
int16 mod7_56time=0;
int16 mod7_25[2]={72,150};
int16 mod7_25time=0;
int16 mod7_45time2=0;
int16 mod7_cnt=0;               //ת��Ȧ����������
int16 mod7_9[2]={183,227};

int16 mod8_5[2]={110,149};
/******************model8*******************************/
int display_count;

void  main(void)//CNST
{
  
    left=40,right=265,up=8,down=222;
    point_init();
     //FTM��ʼ��
    ftm_pwm_init(FTM0, FTM_CH0,(50),midservox);
    ftm_pwm_init(FTM0, FTM_CH1,(50),midservoy);
    keyboardandswitch_init();
    key_set();
    LCD12864_Init();
//    ftm_pwm_init(FTM0, FTM_CH0,(50),midservox);
//    ftm_pwm_init(FTM0, FTM_CH1,(50),midservoy);//�����ʼ��
 //while(1);   
   camera_init(imgbuff);    //����ͷ��ʼ��
   set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
   set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler                                   
   pit_init_ms(PIT0,50);                             //��ʼ��PIT0����ʱʱ��Ϊ�� 3333us=3.333ms
   set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
   
   NVIC_SetPriority(PIT0_IRQn,1);
   NVIC_SetPriority(PORTA_IRQn,0);                         //�趨�ж����ȼ���ȷ������ͷ�ɼ������ȼ����
   NVIC_SetPriority(DMA0_IRQn,2); 
   NVIC_SetPriority(PORTA_IRQn,3);  
    enable_irq (PIT0_IRQn);                                 //ʹ��PIT1�ж�
    while(1)
    {  
          picture_get();
#if 1        
          vcan_sendimg(imgbuff,OV7725_EAGLE_SIZE);
#endif
          find_point();
          display_count++;
            if(display_count>30)
            {
            display_count=0;
            LCD12864_Series_Write(0x98,"Time:");
            LCD12864_Series_Write(0x9b,"       "); 
            LCD12864_Digtal_Write(0x9b,(int)time);
            LCD12864_Series_Write(0x88,"x:");
            LCD12864_Series_Write(0x89,"    ");
            LCD12864_Digtal_Write(0x89,(int)point_x);
            LCD12864_Series_Write(0x8b,"y:");
            LCD12864_Series_Write(0x8c,"    ");
            LCD12864_Digtal_Write(0x8c,(int)point_y);
            
            
            
            }
    //      start_flag=1;

    }
}

void x_yset_caculate_jia()
{
xset = 151;
yset = 35;
}

/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

void PIT0_IRQHandler(void)
{   
  static uint8  speedflashflag=0;
  static float speedlim=2;         //�ٶ�����/////////////////////////////////
  static float lastspeedx1=0.0;
  static float lastspeedx2=0.0;
  static float lastspeedx3=0.0;
  static float lastspeedx4=0.0;
     
  static float lastspeedy1=0.0;
  static float lastspeedy2=0.0;
  static float lastspeedy3=0.0;
  static float lastspeedy4=0.0;
  
   if(start_flag==1)
     {
       if(speedflashflag==1)
       {
         lastspeedx1=0;lastspeedx2=0;lastspeedx3=0;lastspeedx4=0;
         lastspeedy1=0;lastspeedy2=0;lastspeedy3=0;lastspeedy4=0;
         speedflashflag=0;
       }
       gg++;
       time+=0.05;
       x_yset_caculate();//���ڲ�û������������ֵ/////////////////////////////////

       if(time>100) time=100;
       
       if(gg==2)
       {
         gg=0;
         
          xwantspeed=turnx_controlspeed(xset);
          ywantspeed=turny_controlspeed(yset);
          
          if(xwantspeed>speedlim) xwantspeed=speedlim;
          else if(xwantspeed<-speedlim) xwantspeed=-speedlim;
          if(ywantspeed>speedlim) ywantspeed=speedlim;
          else if(ywantspeed<-speedlim) ywantspeed=-speedlim;
         
         
       }
        if(time>0.1)
        {
       speedx_now=((point_x-lastx)+lastspeedx2+lastspeedx3+lastspeedx4+lastspeedx1)/5;
       speedy_now=((point_y-lasty)+lastspeedy2+lastspeedy3+lastspeedy4+lastspeedy1)/5;
       lastspeedx1=speedx_now;lastspeedx2=lastspeedx1;lastspeedx3=lastspeedx2;lastspeedx4=lastspeedx3;
       lastspeedy1=speedy_now;lastspeedy2=lastspeedy1;lastspeedy3=lastspeedy2;lastspeedy4=lastspeedy3;
       
       turn_control();
        }
       lastx=point_x;lasty=point_y;
       
       lastxset=xset,lastyset=yset;
     }
   
   
     else if(start_flag==0)
      {
        speedflashflag=1;
        time=0;
        ftm_pwm_duty(FTM0, FTM_CH0,midservox);
        ftm_pwm_duty(FTM0, FTM_CH1,midservoy);
      }
     
  PIT_Flag_Clear(PIT0);  
}

void x_yset_caculate()
{
  if(model==1&&start_flag==1)
  {
    xset = mod1_2[1];
    yset = mod1_2[0];
  }
  
  else if(model==2&&start_flag==1)
  {
    xset=mod2_5[1];
    yset=mod2_5[0];
  }
  else if(model==3&&start_flag==1)
  {
    if(mod3_4time<40)
    {
      xset=mod3_4[1];
      yset=mod3_4[0];
      if(fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr&&start_flag==1)
      {
        mod3_4time++;
      }
      else mod3_4time=0;
    }
    else if(mod3_4time>=40)
    {
      xset=mod3_5[1];
      yset=mod3_5[0];
    }
  }
  else if(model==4&&start_flag==1)
  {
    if(mod4_45time<=delaytime&&start_flag==1)
    {
      xset = mod4_45[1];
      yset = mod4_45[0];
      if((fabs(xset-point_x)<=stoperr)&&(fabs(yset-point_y)<=stoperr)&&start_flag==1)
      {
        mod4_45time++;
      }
      else mod3_4time=0;
    }
    else if(mod4_45time>delaytime&&start_flag==1)
    {
      xset = mod4_9[1];
      yset = mod4_9[0];
    }
  }
  else if(model==5&&start_flag==1)
  {
    if(mod5_2time<=delaytime&&mod5_6time<=delaytime&&start_flag==1)
    { 
      xset = mod5_2[1];
      yset = mod5_2[0];
      if((fabs(xset-point_x)<=stoperr)&&(fabs(yset-point_y)<=stoperr)&&start_flag==1)
      {
        mod5_2time++;
      }
      else mod5_2time=0;
    }
    else if(mod5_2time>delaytime&&mod5_6time<=delaytime&&start_flag==1)
    {
      xset = mod5_6[1];
      yset = mod5_6[0];
      if((fabs(xset-point_x)<=stoperr)&&(fabs(yset-point_y)<=stoperr)&&start_flag==1)
      {
        mod5_6time++;
      }
      else mod5_6time=0;
    }
    else if(mod5_2time>delaytime&&mod5_6time>delaytime&&start_flag==1)
    {
      xset = mod5_9[1];
      yset = mod5_9[0];
    } 
  }
  
    if(model==8&&start_flag==1)
  {
    xset = mod8_5[1];
    yset = mod8_5[0];
  }
  else if(model==6&&start_flag==1)
  {
//    
//   
//    if(mod6_pointnum<mod6_pointcnt)
//    {
//      xset = mod6_setpoint[mod6_pointnum][1];
//      yset = mod6_setpoint[mod6_pointnum][0];
//      
//      if((fabs(xset-point_x)<=stoperr)&&(fabs(yset-point_y)<=stoperr)&&start_flag==1)
//      {
//        mod6_delaytime++;
//      }
//      else mod6_delaytime=0;
//      
//      if(mod6_delaytime>delaytime)
//      {
//        mod6_pointnum++;
//        mod6_delaytime=0;
//      }
//     
//    }
//   
        if(mod6_cnt<3)
    {
      if(mod6_1time<=delaytime&&mod6_2time<=delaytime&&mod6_3time<=delaytime&&mod6_4time<=delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod6_1[1];
         yset = mod6_1[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod6_1time++;
         }
        else mod6_1time=0;
      }
      else if(mod6_1time>delaytime&&mod6_2time<=delaytime&&mod6_3time<=delaytime
              &&mod6_4time<=delaytime&&mod6_4time2<=delaytime)
      {
         xset = mod6_2[1];
         yset = mod6_2[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod6_2time++;
         }
        else mod6_2time=0;
      }
      else if(mod6_2time>delaytime&&mod6_1time>delaytime&&mod6_3time<=delaytime
              &&mod6_4time<=delaytime&&mod6_4time2<=delaytime)
      {
         xset = mod6_3[1];
         yset = mod6_3[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod6_3time++;
         }
        else mod6_3time=0;
      }
       else if(mod6_1time>delaytime&&mod6_2time>delaytime&&mod6_3time>delaytime
              &&mod6_4time<=delaytime&&mod6_4time2<=delaytime)
      {
         xset = mod6_4[1];
         yset = mod6_4[0];
        if(fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr)
        {
        mod6_4time++;
         }
        else mod6_4time=0;
      }

      else if(mod6_2time>delaytime&&mod6_1time>delaytime&&mod6_3time>delaytime
              &&mod6_4time>delaytime&&mod6_4time2>delaytime)
      {
        mod6_1time=0;
        mod6_2time=0;
        mod6_3time=0;
        mod6_4time=0;
        mod6_4time2=0;
        mod6_cnt+=1;
      }
    }
    
  }
  else if(model==7&&start_flag==1)
  {
    if(mod7_cnt<3)
    {
      if(mod7_45time<=delaytime&&mod7_58time<=delaytime&&mod7_56time<=delaytime&&mod7_25time<=delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod7_45[1];
         yset = mod7_45[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod7_45time++;
         }
        else mod7_45time=0;
      }
      else if(mod7_45time>delaytime&&mod7_58time<=delaytime&&mod7_56time<=delaytime
              &&mod7_25time<=delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod7_58[1];
         yset = mod7_58[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod7_58time++;
         }
        else mod7_58time=0;
      }
      else if(mod7_45time>delaytime&&mod7_58time>delaytime&&mod7_56time<=delaytime
              &&mod7_25time<=delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod7_56[1];
         yset = mod7_56[0];
        if((fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr))
        {
        mod7_56time++;
         }
        else mod7_56time=0;
      }
       else if(mod7_45time>delaytime&&mod7_58time>delaytime&&mod7_56time>delaytime
              &&mod7_25time<=delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod7_25[1];
         yset = mod7_25[0];
        if(fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr)
        {
        mod7_25time++;
         }
        else mod7_25time=0;
      }
       else if(mod7_45time>delaytime&&mod7_58time>delaytime&&mod7_56time>delaytime
              &&mod7_25time>delaytime&&mod7_45time2<=delaytime)
      {
         xset = mod7_45[1];
         yset = mod7_45[0];
        if(fabs(xset-point_x)<=stoperr&&fabs(yset-point_y)<=stoperr)
        {
        mod7_45time2++;
         }
        else mod7_45time2=0;
      }
      else if(mod7_45time>delaytime&&mod7_58time>delaytime&&mod7_56time>delaytime
              &&mod7_25time>delaytime&&mod7_45time2>delaytime)
      {
        mod7_45time=0;
        mod7_58time=0;
        mod7_56time=0;
        mod7_25time=0;
        mod7_45time2=0;
        mod7_cnt+=1;
      }
    }
    else if(mod7_cnt>=3)
    {
      xset = mod7_9[1];
      yset = mod7_9[0];
    }
    
  }
}


void point_init()
{
  mod1_2[0]=point_position[1][0];
  mod1_2[1]=point_position[1][1];
  
  mod2_5[0]=point_position[4][0];
  mod2_5[1]=point_position[4][1];
  
   mod3_4[0]=point_position[3][0];
   mod3_4[1]=point_position[3][1];
   mod3_5[0]=point_position[4][0];
   mod3_5[1]=point_position[4][1];
   
   
   mod4_45[0]=(point_position[3][0]+point_position[4][0])/2;
   mod4_45[1]=(point_position[3][1]+point_position[4][1])/2;
   mod4_9[0]=point_position[8][0] ;
   mod4_9[1]=point_position[8][1] ;
   
    mod5_2[0]=point_position[1][0] ;
    mod5_2[1]=point_position[1][1];
    mod5_6[0]=point_position[5][0] ;
    mod5_6[1]=point_position[5][1] ;
    mod5_9[0]=point_position[8][0] ;
    mod5_9[1]=point_position[8][1] ;
    
    
    mod7_45[0]=(point_position[3][0]+point_position[4][0] )/2;
    mod7_45[1]=(point_position[3][1]+point_position[4][1] )/2;
    mod7_58[0]=(point_position[4][0]+point_position[7][0] )/2;
    mod7_58[1]=(point_position[4][1]+point_position[7][1] )/2;
    mod7_56[0]=(point_position[4][0]+point_position[5][0] )/2;
    mod7_56[1]=(point_position[4][1]+point_position[5][1] )/2;
    mod7_25[0]=(point_position[1][0]+point_position[4][0] )/2;
    mod7_25[1]=(point_position[1][1]+point_position[4][1] )/2;
    mod7_9[0]=point_position[8][0] ;
    mod7_9[1]=point_position[8][1] ;
    
    mod8_5[0]=point_position[4][0];
    mod8_5[1]=point_position[4][1];
}