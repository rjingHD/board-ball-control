#include "include.h"


#ifndef PICTURE_H
#define PICTURE_H


#define W 320
#define H 240

#define H_min 15
#define H_max  225
#define W_min  60
#define W_max  240

#define	IMG_BLACK		0
#define	IMG_WHITE		255
#include "include.h"
extern uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
extern uint8 img[CAMERA_H][CAMERA_W];
extern float point_x,point_y;
extern uint16 N_object;
void picture_get() ;
void find_point();

#endif



