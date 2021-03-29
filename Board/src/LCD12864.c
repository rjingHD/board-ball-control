#include "LCD12864.h"
#include "stdio.h"
#include "include.h"


#define uchar unsigned char
#define uint unsigned int

  //          LCD12864_Init();
//    LCD12864_Series_Write(0x80,"fun!");
/*函数名 ：LCD12864_Init()
 *输入：无
 *输出：无
 *描述：初始化LCD12864
 *备注：无
*/
void LCD12864_Init()
{
    gpio_init(PTC5,GPO,1);
    gpio_init(PTC6,GPO,1);
    gpio_init(PTC7,GPO,1);
    LCD12864_Write(0,0x30);
    LCD12864_Write(0,0x01);
    LCD12864_Write(0,0x06);      
    LCD12864_Write(0,0x0C);     
}
/*函数名 ：LCD12864_Write(unsigned char type,unsigned char dat)
 *输入：type = 0   写指令
        type = 1   写数据
 *输出：无
 *描述：向LCD12864写入指令/数据
 *备注：无
*/
void LCD12864_Write(unsigned char type,unsigned char dat)
{
    unsigned char cmd,dat1,dat2;
    unsigned char i = 0;
    LCD12864_RS_H;
    LCD12864_E_L;
    cmd = (0xF8 | (type << 1));   //分三次写入，分别为起始字节，高四位，低四位
 for(i=0;i<8;i++)
    {
        if(cmd & 0x80)  
        {
          LCD12864_RW_H;
        }        
        else
        {
            LCD12864_RW_L;
        }
        LCD12864_E_H;
        cmd <<= 1;
        DELAY_US(100);
        LCD12864_E_L;
    }
    dat1 = dat & 0xF0;
    for(i=0;i<8;i++)
    {
        if(dat1 & 0x80)  
        {
            LCD12864_RW_H;
        }
        else            
        {
          LCD12864_RW_L;
        }
         LCD12864_E_H;
        dat1 <<= 1;
        DELAY_US(100);
        LCD12864_E_L;
    }
    dat2 = (dat<<4) & 0xF0;
    for(i=0;i<8;i++)
    {
        if(dat2 & 0x80)  
        { 
            LCD12864_RW_H;
        }
        else  
        {
            LCD12864_RW_L;
        }
        LCD12864_E_H;
        dat2 <<= 1;
        DELAY_US(100);
        LCD12864_E_L;
    }
    LCD12864_RS_L;
    LCD12864_E_L;   
}
/*函数名 ：LCD12864_Delay_us(unsigned int us)
 *输入：us   延迟时间          us范围为(0,65535)
 *输出：无
 *描述：延迟一定时间
 *备注：不精确延迟
*/
void LCD12864_Delay_us(unsigned int us)
{
    unsigned int i;
    for(i=0;i<us;i++);
}
/*函数名 ：set_position(unsigned char position)
 *输入：position   光标起始位置     
 *输出：无
 *描述：设置光标位置
 *备注：无
*/
void set_position(unsigned char position)
{
    LCD12864_Write(0,position);
}


/*函数名 ：LCD1284_Series_Write(unsigned char position, unsigned char *series)
 *输入：position   光标起始位置
        *series    输入字符串      
 *输出：无
 *描述：写入字符串，也可以写入汉字
 *备注：无
*/
void LCD12864_Series_Write(unsigned char position, unsigned char *series)
{
    LCD12864_Write(0,position);                      //设定光标位置
    for(;*series != 0;series++)
       LCD12864_Write(1,*series);
}
void LCD12864_Digtal_Write(unsigned char position, signed long int num)
{
  
    unsigned char index = 0,strcnt=0;
    unsigned char temp[11],str[11];  
  
    if(num < 0)
    {
        str[strcnt++] = '-';
        num = -num;
    }
    do{
        temp[index++] = num % 10;
        num /= 10;
    }while(num != 0);
    for(;index > 0;)
      str[strcnt++] = temp[--index] + '0';
    str[strcnt++] = '\0';
    LCD12864_Series_Write(position,str);
}
