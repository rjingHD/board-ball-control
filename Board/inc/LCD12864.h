#ifndef _LCD12864_H_
#define _LCD12864_H_

#include "include.h"

#define uchar unsigned char
#define uint unsigned int


void LCD12864_Init();
void LCD12864_Write(unsigned char type,unsigned char dat);
void LCD12864_Delay_us(unsigned int us);
void LCD1284_Series_Write(unsigned char position, unsigned char *series);
void LCD12864_Digtal_Write(unsigned char position, signed long int num);
void set_position(unsigned char position);


#define LCD12864_RS_H   gpio_set(PTC5,1u)
#define LCD12864_RS_L   gpio_set(PTC5,0u)
#define LCD12864_RW_H   gpio_set(PTC6,1u)
#define LCD12864_RW_L   gpio_set(PTC6,0u)
#define LCD12864_E_H    gpio_set(PTC7,1u)
#define LCD12864_E_L    gpio_set(PTC7,0u)

extern void LCD12864_Init();
extern void LCD12864_Write(unsigned char type,unsigned char dat);
extern void LCD12864_Delay_us(unsigned int us);
extern void LCD12864_Series_Write(unsigned char position, unsigned char *series);
extern void LCD12864_Digtal_Write(unsigned char position, signed long int num);
extern void set_position(unsigned char position);


#endif