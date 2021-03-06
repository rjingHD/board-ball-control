#ifndef _l3g_mma_H_
#define _l3g_mma_H_



//#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38
//****************************************
#define	SlaveAddress   0x69	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

#define MMA845x_IIC_ADDRESS 0x1c

extern float angle,ang_speed,ang_acc;
extern void hubu_fliter();
void l3g_mma_init();
extern void stand_ctrl();

//extern float StandControlPwm_x;
//extern float StandControlPwm_y;
//extern int ball_center_w ;
//extern int ball_center_h ;
//extern void Motor_Ctrl(int32 Left_Motor,int32 Right_Motor);
//void stand_ctrl();
//void turn_control();
//void angerr_caculate();
//extern float angxerr,angyerr;
//float turnx_controlspeed(float pointxset);
//float turny_controlspeed(float pointyset);
//extern float   speedset_x,speedset_y;


#endif