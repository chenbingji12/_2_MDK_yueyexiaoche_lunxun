
#include "stm32f4xx.h"                  // Device header

#ifndef han_shu_H
#define han_shu_H

// 8路灰度传感器分布值: 编码方式 = 每位权重×10^i, 使数值直观显示位模式
// 传感器排列: 位7(左侧) ~ 位0(右侧), 1=检测到黑线
#define h_00000000 0// 00000000: 无传感器检测到白线(全黑)
#define h_00001111 1111//00001111: 传感器0-3(右侧全部)检测到白线
#define h_11110000 11110000 // 11110000: 传感器4-7(左侧全部)检测到白线
#define h_11111000 11111000 // 11111000: 传感器3-7检测到白线
#define h_11111100 11111100 // 11111100: 传感器2-7检测到白线
#define h_11111110 11111110 // 11111110: 传感器1-7检测到白线
#define h_11111111 11111111 // 11111111: 全部传感器检测到白线(全白)
#define h_00011111 11111//00011111: 传感器0-4检测到白线
#define h_11100000 11100000 // 11100000: 传感器5-7(左侧)检测到白线
#define h_00000111 111      // 00000111: 传感器0-2(右侧)检测到白线
#define h_11101111 11101111 // 11101111: 除传感器4外检测到白线
#define h_11110111 11110111 // 11110111: 除传感器3外检测到白线
#define h_11111011 11111011 // 11111011: 除传感器2外检测到白线
#define h_11110011 11110011 // 11110011: 除传感器2-3外检测到白线
#define h_11011111 11011111 // 11011111: 除传感器5外检测到白线
#define h_11001111 11001111 // 11001111: 除传感器5-6外检测到白线
#define h_10111111 10111111 // 10111111: 除传感器6外检测到白线
#define h_10011111 10011111 // 10011111: 除传感器6-7外检测到白线
#define h_11111101 11111101 // 11111101: 除传感器1外检测到白线
#define h_11111001 11111001 // 11111001: 除传感器1-2外检测到白线
#define h_01111111 1111111  // 01111111: 除传感器7(最左侧)外检测到白线
#define h_00111111 111111   // 00111111: 除传感器6-7(左侧)外检测到白线

extern volatile int yuansudu;//电机基础转速(原始速度), 作为Dianjisudu()的速度基准值

/*小车状态机*/
typedef enum {
    STATE_XUNJI,  // 跟随白线
    STATE_BIZHANG,  // 避障
    STATE_JIANSUDAI,  // 减速带
    STATE_XIAPO,  // 下坡
    STATE_STOP  // 停止
} CARSTATE;
extern CARSTATE CarState;  // 初始状态为循迹

/* 循迹控制结构体, 用于存储电机速度控制相关参数*/
typedef struct
{
  uint32_t huidufenbu;
  void (*zhixinghanshu)(void);
} HUIDUFENBU;

void cmd_1(void);
void cmd_2(void);
void cmd_3(void);
void cmd_4(void);
void cmd_5(void);
void cmd_6(void);
void cmd_7(void);
void cmd_8(void);
void cmd_9(void);
void cmd_10(void);
void cmd_11(void);

void xunji(uint32_t HUIDU);
void shizilukou(void);

/*减速带函数*/
void jiansudai(void);

/*下坡函数*/
void xiapo(void);

/*电机控制函数*/
void RightDianji(int right_speed);
void LeftDianji(int left_speed);
void Dianjisudu(int left_speed,int right_speed);

/*灰度传感器函数*/
int huiduduqu(void);
int huiduzhi(int tongdao);
uint32_t zonghuiduzhi(void);

/*超声波*/
void TIM11_Delay_us(uint16_t us);
float chaoshenboceju(void);

/*陀螺仪*/
typedef struct {
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;  
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;     // 原始陀螺仪值
    float accel_x, accel_y, accel_z;  // 加速度 (g)
    float gyro_x, gyro_y, gyro_z;     // 角速度 (°/s)
} MPU6050_Data;

uint8_t MPU6050_ReadFiltered(MPU6050_Data* data);       // 读取数据
float Get_Z_RotationAngle(void);        // 获取Z轴旋转角度
uint8_t Tuoluoyi_Calibrate(uint16_t samples);       // 陀螺仪Z轴校准（向后兼容）
uint8_t MPU6050_Calibrate(uint16_t samples);        // MPU6050完整校准(陀螺仪3轴+加速度计3轴)
void Xuanzhuan(float target);       // 旋转到指定角度
void Zhizou(void);      // 直走，自动调整保持当前角度

#endif
