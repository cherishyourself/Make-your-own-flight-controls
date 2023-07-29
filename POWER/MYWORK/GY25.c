/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 11:30:48
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 07:27:55
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\GY25.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
本文件用于DMA解算和转换GY25和GY25Z
后续会添加GY901B
*/
#include "GY25.h"
#include "station.h"
extern  LINK_USER  LINK_USER1;
u8 USART1_RX[BUFFER_SIZE1] ;//GY25数据接收
float GY25_Now[5],GY25_GYRO[10],GY25_ACC[10];
void GY25_Analyse(void)
{
	
	if(p_order->MPU6050MODE==0)                                       //GY25
{
int i=0;
while(USART1_RX[i++]==0xaa&&USART1_RX[i+6]==0x55&&i<BUFFER_SIZE1)
{
	GY25_pitch   = ((USART1_RX[i+2] << 8) | USART1_RX[i+3])/100+LEVEL[1];
  GY25_yaw = ((USART1_RX[i+4] << 8) | USART1_RX[i+5])/100;
  GY25_roll= ((USART1_RX[i] << 8) |USART1_RX[i+1])/100+LEVEL[0];
	if(GY25_roll>180) GY25_roll-=655;
	GY25_roll=-GY25_roll;
	if(GY25_pitch>180) GY25_pitch-=655;
	if(GY25_yaw>180) GY25_yaw-=655;
}
}

if(p_order->MPU6050MODE==1)                                          //GY25Z
{
	int i=0;
while(USART1_RX[i++]==0x5A&&i<BUFFER_SIZE1)
{
	if(USART1_RX[i]==0x5A)
	{   int j;
		for(j=i;j<15;j++)
		{if(USART1_RX[j]==0x5A&&USART1_RX[j+1]==0x5A) break;}
  	GY25_GYRO[1]=((USART1_RX[i+3] << 8) | USART1_RX[i+4])/16.3835;
	GY25_GYRO[2]=((USART1_RX[i+5] << 8) | USART1_RX[i+6])/16.3835;
	GY25_GYRO[3]=((USART1_RX[i+7] << 8) | USART1_RX[i+8])/16.3835;
    if(GY25_GYRO[1]>2000) GY25_GYRO[1]=GY25_GYRO[1]-4000;
	if(GY25_GYRO[2]>2000) GY25_GYRO[2]=GY25_GYRO[2]-4000;
	if(GY25_GYRO[3]>2000) GY25_GYRO[3]=GY25_GYRO[3]-4000;
	GY25_roll =((USART1_RX[i+9] << 8) | USART1_RX[i+10])/100;
	GY25_pitch=((USART1_RX[i+11] << 8) | USART1_RX[i+12])/100;
	GY25_yaw=((USART1_RX[i+13] << 8) | USART1_RX[i+14])/100;
	if(GY25_roll>180) GY25_roll-=655;
	GY25_roll=-GY25_roll;
	if(GY25_pitch>180) GY25_pitch-=655;
	if(GY25_yaw>180) GY25_yaw-=655;
	}	
}
}
GY25_roll+=LEVEL[0];
GY25_pitch+=LEVEL[1];
if(p_order->LEVELMODE==1)
{
GY25_Now[0]=GY25_pitch;
GY25_Now[1]=GY25_roll;
p_order->LEVELMODE=0;
}
GY25_pitch-=GY25_Now[0];
GY25_roll-=GY25_Now[1];
}

