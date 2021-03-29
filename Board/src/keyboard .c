#include "common.h"
#include "include.h"
#include "keyboard.h"

uint8  mode_state = 0;
uint8  speed_mode = 1;
uint8  keynum = 0;  
uint8  display_flag = 0;
uint8  display_status = 0;
uint8  debug_flag = 0;
uint8  ring_status1 = 0;
uint8  ring_status2 = 0;
uint8  ring_status3 = 0;
uint8  ring_status = 0;

//{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}

extern int16 mod6_1[2];//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
extern int16 mod6_1time;
extern int16 mod6_2[2];//5 : {118,159}
extern int16 mod6_2time;
extern int16 mod6_3[2];
extern int16 mod6_3time;
extern int16 mod6_4[2];
extern int16 mod6_4time;

uint8 model6_areaselect=0;
uint8 model6area=0;
extern int8 start_flag;
extern int16  model;
extern float time; 
extern float point_x,point_y;
/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */

void PORTE_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint8  test = 0;    
    uint32 flag_c= PORTE_ISFR;
    
                                       //清中断标志位   
    n = 1;                                            
    if(flag_c & (1 << n))                                 //PTC1触发中断
    {
        DELAY_MS(50);
        test=gpio_get(PTE1);        
        if(test==0)
          keynum=1;
    }
    n = 2;                                             
    if(flag_c & (1 << n))                                 //PTC2触发中断
    {
        DELAY_MS(50);
        if(test==gpio_get(PTE2))
          keynum=2;
    }
    n = 3;                                             //场中断
    if(flag_c & (1 << n))                                 //PTC3触发中断
    {
        DELAY_MS(50);
        if(test==gpio_get(PTE3))
          keynum=3;
    }
    n = 4;                                             //场中断
    if(flag_c & (1 << n))                                 //PTC4触发中断
    {
        DELAY_MS(50);
        if(test==gpio_get(PTE4))
          keynum=4;
    }
    n = 5;                                             //场中断
    if(flag_c & (1 << n))                                 //PTC4触发中断
    {
        DELAY_MS(50);
        if(test==gpio_get(PTE5))
          keynum=5;
    }
    
    
    if (keynum>0)
    {
      display_flag=1;
      display_status=keynum;
    }
    if (keynum==1)
        {
         if(model<8)
         model++;
         else model=0;
         keynum=0;
        }
    
    else if (keynum==2)
        {
           if(model==6)
           {
           model6_areaselect++;
           if(model6_areaselect>4)
            model6_areaselect=0; 
           }
           

         keynum=0;
        }
    
    else if (keynum==3)
    {
    if(model6_areaselect!=0)
    {
      model6area++;
      if(model6area>9)
      {
      model6area=0;
      }
     //没写完 
    }

             keynum=0;

    }
    else if (keynum==4)
    {     
          start_flag=1;
          time=0;
          keynum=0;                        
    }
     else if (keynum==5)
    {     
      start_flag=0; 
      keynum=0;
                        
    }
    
    if(model6_areaselect==1)
    {
   //{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}
mod6_1[0]=110;
mod6_1[1]=149;//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
mod6_1time=0;
mod6_2[0]=111;
mod6_2[1]=172;//5 : {118,159}
mod6_2time=0;
mod6_3[0]=37;
mod6_3[1]=73;
mod6_3time=0;
mod6_4[0]=35;
mod6_4[1]=151;
mod6_4time=0;
    }
    
    if(model6_areaselect==2)
    {
   //{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}
//mod6_1[2]={110,149};//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
//mod6_1time=0;
//mod6_2[2]={35,151};//5 : {118,159}
//mod6_2time=0;
//mod6_3[2]={36,225};
//mod6_3time=0;
//mod6_4[2]={107,227};
mod6_4time=0;
mod6_1[0]=110;
mod6_1[1]=149;//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
mod6_1time=0;
mod6_2[0]=35;
mod6_2[1]=151;//5 : {118,159}
mod6_2time=0;
mod6_3[0]=36;
mod6_3[1]=225;
mod6_3time=0;
mod6_4[0]=107;
mod6_4[1]=227;
    }
    
    
        if(model6_areaselect==3)
    {
   //{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}
//mod6_1[2]={110,149};//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
//mod6_1time=0;
//mod6_2[2]={107,227};//5 : {118,159}
//mod6_2time=0;
//mod6_3[2]={183,227};
//mod6_3time=0;
//mod6_4[2]={188,151};

mod6_4time=0;
mod6_1[0]=110;
mod6_1[1]=149;//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
mod6_1time=0;
mod6_2[0]=107;
mod6_2[1]=227;//5 : {118,159}
mod6_2time=0;
mod6_3[0]=183;
mod6_3[1]=227;
mod6_3time=0;
mod6_4[0]=188;
mod6_4[1]=151;
    }
    
    
        if(model6_areaselect==4)
    {
   //{37,73},{35,151},{36,225},{111,72},{110,149},{107,227},{188,73},{188,151},{183,227}
//mod6_1[2]={110,149};//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
//mod6_1time=0;
//mod6_2[2]={188,151};//5 : {118,159}
//mod6_2time=0;
//mod6_3[2]={188,73};
//mod6_3time=0;
//mod6_4[2]={111,72};
//mod6_4time=0;
mod6_4time=0;
mod6_1[0]=110;
mod6_1[1]=149;//{153,160},{118,195},{83,159},{118,124}};25,45,58,56
mod6_1time=0;
mod6_2[0]=188;
mod6_2[1]=151;//5 : {118,159}
mod6_2time=0;
mod6_3[0]=188;
mod6_3[1]=73;
mod6_3time=0;
mod6_4[0]=111;
mod6_4[1]=72;
    }
    
    OLED_display();      
                     
   
    PORTE_ISFR  = ~0;
}

void OLED_display()
{
if(display_flag!=0)
              {
                LCD12864_Write(0,0x01);
                LCD12864_Series_Write(0x80,"mode:");
                LCD12864_Digtal_Write(0x84,model);
                LCD12864_Series_Write(0x90,"area:");
                LCD12864_Digtal_Write(0x93,model6_areaselect);
                LCD12864_Series_Write(0x94,"num:");
                LCD12864_Digtal_Write(0x96,model6area);
                
                
//                  OLED_CLS();
//                  OLED_P6x8Str(0,0,"mode");
//                  Dis_Num(1,0,mode_state,0);  
//                  OLED_P6x8Str(0,2,"spdmod");
//                  Dis_Num(3,0,speed_mode,0);
//                  OLED_P6x8Str(0,4,"spdset");
//                  Dis_Num(5,0,speedset,0);
//                  OLED_P6x8Str(0,6,"R1 R2 R3");
//                  Dis_Num(7,0,ring_status1,0);
//                  Dis_Num(7,2,ring_status2,0);
//                  Dis_Num(7,4,ring_status3,0);
////                  if(PID_status==0)                  
////                  OLED_P6x8Str(56,7,"S");
////                  if(PID_status==1)
////                  OLED_P6x8Str(56,7,"M"); 
//                  
//                  OLED_P6x8Str(64,2,"Kp");
//                  Dis_Num(3,8,deltaKp,0);
//             //     OLED_ShowData_Float(deltaKp,3,3);
//                  OLED_P6x8Str(64,4,"Tdin");
//                  Dis_Num(5,8,deltaTdin,0);
//                  OLED_P6x8Str(64,6,"Tdout");
//                  Dis_Num(7,8,deltaTdout,0);
//                  if(mode_state==1)
//              {
//                    OLED_P6x8Str(64,0,"spemodset");
//               } 
//            else if(mode_state==2)
//              {                 
//                    OLED_P6x8Str(64,0,"speset"); 
//               }
//            else if(mode_state==3)
//             {
//                    OLED_P6x8Str(64,0,"ringset");
//              }
//           else if(mode_state==4)
//             {
//                    OLED_P6x8Str(64,0,"Kpset");
//              }
//           else if(mode_state==5)
//              {
//                    OLED_P6x8Str(64,0,"Tdinset");
//               }
//           else if(mode_state==6)
//              {
//                    OLED_P6x8Str(64,0,"Tdoutset");
//              }
//                  
                  display_flag=0;                    
              } 
}

void key_set()
{
  
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);
    
    
//    while(!gpio_get(PTE27))
//    {
//    if(debug_flag==0)
    enable_irq(PORTE_IRQn);
    
//    debug_flag = 1;    
//    OLED_display();//摁键读取后得到的结果用液晶显示  
//    }
//    
//    if(debug_flag==1)
//    disable_irq(PORTE_IRQn);      
    
}

void keyboardandswitch_init()
{
gpio_init(PTE1, GPI, 0);
port_init(PTE1, IRQ_FALLING | PF | ALT1 | PULLUP );
gpio_init(PTE2, GPI, 0);
port_init(PTE2, IRQ_FALLING | PF | ALT1 | PULLUP );
gpio_init(PTE3, GPI, 0);
port_init(PTE3, IRQ_FALLING | PF | ALT1 | PULLUP );
gpio_init(PTE4, GPI, 0);
port_init(PTE4, IRQ_FALLING | PF | ALT1 | PULLUP );
gpio_init(PTE5, GPI, 0);
port_init(PTE5, IRQ_FALLING | PF | ALT1 | PULLUP );
//gpio_init(PTE27, GPI, 0);
//port_init(PTE27, PF | ALT1 | PULLUP );
//gpio_init(PTE26, GPI, 0);
//port_init(PTE26, PF | ALT1 | PULLUP );
//gpio_init(PTE25, GPI, 0);
//port_init(PTE25, PF | ALT1 | PULLUP );
//gpio_init(PTE24, GPI, 0);
//port_init(PTE24, PF | ALT1 | PULLUP );
//

}
