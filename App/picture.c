#include "include.h"
#include "picture.h"
#include "project.h"


#define W 320
#define H 240

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];
float point_x=0,point_y=0;

uint16 N_object=0;               //�ҵ�����������
uint32 dd;                         //������������ʱΪ320��һ��ͼ��320�У�


int16 area=0;
int16 centre[3]={0};         //   [0]��  [1]��    [2]����
                                            //ͼ������ر���
int16 Object[1][CAMERA_H][2]={0};  //�����������飬�ֱ𴢴���������ÿһ�еĳ��������ĵ�                                       //�����������飬�ֱ𴢴���������ÿһ�еĳ��������ĵ�
int16 lenght,middle,qq;                                                
uint16 N_object,flag;
                                             //   [0]��  [1]��    [2]����
int16 centre1[2]={0};

int32  ax,ay,bx,by,cx,cy;
char bb;
int32 distance[3]={0};
uint8 temp1,temp2;
uint8 tower,head,tail;
uint32 fuck=0;
uint16 errortime=0;



void picture_get()      //ͼ���ȡ����
{
  camera_get_img();                          //����ͷ��ȡͼ��
  img_extract(img, imgbuff, CAMERA_SIZE);          //��ѹΪ�Ҷ�ͼ�񣬷��㷢�͵���λ����
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
      if((!flag)&&(!img[ii][jj])) {flag=1;continue;}                                   //��⵽0��1����㣬����־λ					
      if(flag&&!img[ii][jj])  {lenght++;continue;}                    //����ȫ1�εĳ���                
	if(flag&&!img[ii][jj-1]&&(img[ii][jj]))                                       //��⵽1,0�����
		{   
		flag=0;                                                       //�ر�־λ����ʶȫ1�εĽ���
		middle=jj-1-lenght/2;                                          //����ȫ1�εĳ��������ĵ�
		
		
		  if((Object[0][ii-1][0])&&(abs(middle-Object[0][ii-1][0])<=error)) //�����ĵ�ֱ�������������һ�е����ĵ����Ƚϣ�
		   {                                                                 //�����ĵ����������Χ�ڣ����˶ε����ĵ��볤�ȴ��ڸ�������
			Object[0][ii][0]=middle;
			Object[0][ii][1]=lenght;
			area=area+lenght;
			centre[2]++;
			centre[1]=(centre[1]+middle)/2;									
			middle=0;
			lenght=0;
                        
			break;
		    }
		
	          if(middle)                                                //��û�д��浽�������ö���������ĵ�һ��
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
