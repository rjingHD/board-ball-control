#include "include.h"
#include "picture.h"
#include "project.h"


#define W 320
#define H 240

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];
float point_x=0,point_y=0;

uint16 N_object=0;               //找到的物体数量
uint32 dd;                         //程序运行正常时为320（一幅图像320行）


int16 area=0;
int16 centre[3]={0};         //   [0]行  [1]列    [2]行数
                                            //图像处理相关变量
int16 Object[1][CAMERA_H][2]={0};  //定义三个数组，分别储存三个物体每一行的长度与中心点                                       //定义三个数组，分别储存三个物体每一行的长度与中心点
int16 lenght,middle,qq;                                                
uint16 N_object,flag;
                                             //   [0]行  [1]列    [2]行数
int16 centre1[2]={0};

int32  ax,ay,bx,by,cx,cy;
char bb;
int32 distance[3]={0};
uint8 temp1,temp2;
uint8 tower,head,tail;
uint32 fuck=0;
uint16 errortime=0;



void picture_get()      //图像获取函数
{
  camera_get_img();                          //摄像头获取图像
  img_extract(img, imgbuff, CAMERA_SIZE);          //解压为灰度图像，方便发送到上位机显
}

void find_point()
{
  uint16 error=15;
  

  N_object=0;
  area=0;		
	centre[0]=0;
	centre[1]=0;
	centre[2]=0;
	for(uint8 ii=0;ii<CAMERA_H-1;ii++)
	for(uint8 jj=0;jj<2;jj++)
	Object[0][ii][jj]=0;
  if(left>0&&right>160&&down>120&&up>0)      
  {for(uint16 ii=up+8;ii<down-8;ii++) 
    for(uint16 jj=left+8;jj<right-8;jj++)
    {
      if((!flag)&&(!img[ii][jj])) {flag=1;continue;}                                   //检测到0，1跳变点，开标志位					
      if(flag&&!img[ii][jj])  {lenght++;continue;}                    //计算全1段的长度                
	if(flag&&!img[ii][jj-1]&&(img[ii][jj]))                                       //检测到1,0跳变点
		{   
		flag=0;                                                       //关标志位，标识全1段的结束
		middle=jj-1-lenght/2;                                          //计算全1段的长度与中心点
		
		
		  if((Object[0][ii-1][0])&&(abs(middle-Object[0][ii-1][0])<=error)) //此中心点分别与三个数组上一行的中心点作比较；
		   {                                                                 //若中心点误差在允许范围内，将此段的中心点与长度存在该数组中
			Object[0][ii][0]=middle;
			Object[0][ii][1]=lenght;
			area=area+lenght;
			centre[2]++;
			centre[1]=(centre[1]+middle)/2;									
			middle=0;
			lenght=0;
                        
			break;
		    }
		
	          if(middle)                                                //若没有储存到，标明该段是新物体的第一段
		    {
                             if(N_object==0)
                             { Object[N_object][ii][0]=middle;
			       Object[N_object][ii][1]=lenght;
			       area=area+lenght;
			       centre[0]=ii;
			       centre[1]=middle;}
                             
                             if(N_object>0)
                             {
                               if(area<5||area>60)
                               {
                                 N_object=0;
                                 area=0;		
	                         centre[0]=0;
	                         centre[1]=0;
	                         centre[2]=0;
	                         for(uint8 ii=0;ii<CAMERA_H-1;ii++)
	                        for(uint8 jj=0;jj<2;jj++)
	                        Object[0][ii][jj]=0;
                                
                                Object[N_object][ii][0]=middle;
			       Object[N_object][ii][1]=lenght;
			       area=area+lenght;
			       centre[0]=ii;
			       centre[1]=middle;
                               }
                             }
                             
			       middle=0;
				 lenght=0;
				 N_object++;
		     }
                  
                  
                  
	       }	
    }
  }
    centre[0]=centre[0]+centre[2]/2;
    
    if(N_object==1)
    {
      point_x=centre[1];
      point_y=centre[0];
    }
  
}
