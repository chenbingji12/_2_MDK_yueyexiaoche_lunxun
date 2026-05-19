
#include "stm32f4xx_hal.h"
#include "han_shu.h"

CARSTATE CarState = STATE_XUNJI;  // 初始状态为循迹
volatile int yuansudu=60;  //全局变量，存储原始速度，以便在Han_shu线程中使用

/*循迹算法*/
void cmd_1(void){Xuanzhuan(-90);}
void cmd_2(void){Xuanzhuan(90);}
void cmd_3(void){Dianjisudu(yuansudu, yuansudu+5);}
void cmd_4(void){Dianjisudu(yuansudu+5, yuansudu);}
void cmd_5(void){Dianjisudu(yuansudu+15, yuansudu);}
void cmd_6(void){Dianjisudu(yuansudu, yuansudu+15);}
void cmd_7(void){Dianjisudu(yuansudu-15, yuansudu+20);}
void cmd_8(void){Dianjisudu(yuansudu+20, yuansudu-15);}
void cmd_9(void){Dianjisudu(yuansudu-25, yuansudu+30);}
void cmd_10(void){Dianjisudu(yuansudu+30, yuansudu-25);}
void cmd_11(void){Dianjisudu(yuansudu,yuansudu);}
void cmd_12(void){Dianjisudu(0,0);HAL_Delay(200);Xuanzhuan(90);Dianjisudu(0, 0);HAL_Delay(200);}
void cmd_13(void){Dianjisudu(0,0);HAL_Delay(200);Xuanzhuan(-90);Dianjisudu(0, 0);HAL_Delay(200);}

HUIDUFENBU cmd_table[] = {
  {h_11110000, cmd_1},{h_11100000, cmd_1},
  {h_00001111, cmd_2},{h_00000111, cmd_2},
  {h_11101111, cmd_3},
  {h_11110111, cmd_4},
  {h_11111011, cmd_5},{h_11110011, cmd_5},
  {h_11011111, cmd_6},{h_11001111, cmd_6},
  {h_10111111, cmd_7},{h_10011111, cmd_7},
  {h_11111101, cmd_8},{h_11111001, cmd_8},
  {h_01111111, cmd_9},{h_00111111, cmd_9},
  {h_11111100, cmd_10},{h_11111110, cmd_10}
};

uint16_t cmd_table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);

void xunji(uint32_t HUIDU)
{
 for(uint16_t i=0;i<cmd_table_size;i++)
 {
   if(HUIDU==cmd_table[i].huidufenbu)
   {
     cmd_table[i].zhixinghanshu();
     return;
   }
 }
 cmd_11();
}

void shizilukou(void)
{
    int sum=0;
    for(int j=0;j<8;j++)
    {
        sum=sum+huiduzhi(j);
    }
    if(sum>=7)
    {
        cmd_12();
    }
    
}

/*减速带算法*/
void jiansudai(void)
{
    MPU6050_Data data2;
    MPU6050_ReadFiltered(&data2);
    int16_t raw_ay2 = data2.raw_accel_y;
    if(raw_ay2>4000||raw_ay2<-4000)
    {
      HAL_Delay(10);
      MPU6050_ReadFiltered(&data2);    //再次读取数据确认倾斜
      raw_ay2 = data2.raw_accel_y;
      if(raw_ay2>4000||raw_ay2<-4000)
      {
        MPU6050_ReadFiltered(&data2);
        raw_ay2 = data2.raw_accel_y;
        if(raw_ay2>4000||raw_ay2<-4000)
        {
          Dianjisudu(0, 0);
          HAL_Delay(100);
          Zhizou();   //进入直走状态，等待外部干预
        }
      }
    }
}

/*下坡算法*/

void xiapo(void)
{
    MPU6050_Data data2;
    MPU6050_ReadFiltered(&data2);
    int16_t raw_ax2 = data2.raw_accel_x;
    if(CarState==STATE_XUNJI)
    {
    if(raw_ax2<-4000)
    {
        HAL_Delay(10);
        MPU6050_ReadFiltered(&data2);
        raw_ax2 = data2.raw_accel_x;
        if(raw_ax2<-4000)
        {
            HAL_Delay(10);
            MPU6050_ReadFiltered(&data2);
        raw_ax2 = data2.raw_accel_x;
            if(raw_ax2<-4000)
            {
                CarState=STATE_XIAPO;
                yuansudu=30;   //降低基础速度，提升小车在下坡时稳定性
            }
        }
    }
    }
    else if(CarState==STATE_XIAPO)
    {
        if(raw_ax2>-3000&&raw_ax2<3000)
        {
            HAL_Delay(10);
            MPU6050_ReadFiltered(&data2);
        raw_ax2 = data2.raw_accel_x;
            if(raw_ax2>-3000&&raw_ax2<3000)
            {
                HAL_Delay(10);
                MPU6050_ReadFiltered(&data2);
        raw_ax2 = data2.raw_accel_x;
                if(raw_ax2>-3000&&raw_ax2<3000)
                {
                    CarState=STATE_XUNJI;
                    yuansudu=60;   //恢复基础速度
                }
            }
        }
    }
}

