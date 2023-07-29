/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-09 09:26:48
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\Init.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
本文件用于各种初始化：包括中断的开启,DMA的开启,PWM波的开启
*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include "mavlink_user.h"
#include "delay_user.h"
#include "fuzzypid.h"
#include "PID.h"
#include "sbus.h"
#include "servo_engine.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "GY25.h"
#include "station.h"
extern u8 aRxBuffer2[USART_REC_LEN];      //使用的串口接收缓冲
extern u8 USART1_RX[BUFFER_SIZE1] ;//GY25数据接收
void Init_User()
{
	//HAL_UART_Receive_IT(&huart3,(uint8_t*)&aRxBuffer3,1);//开启串口三接收中断
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&aRxBuffer2,1);//开启串口二接收中断，接收SBUS
	HAL_UART_Receive_IT(&huart1,(uint8_t*)&USART1_RX,1);//开启串口一接收中断，接收GY25
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //使能IDLE中断
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	//DMA接收函数，此句一定要加，不加接收不到第一次传进来的实数据，是空的，且此时接收到的数据长度为缓存器的数据长度
	HAL_UART_Receive_DMA(&huart1,USART1_RX,sizeof(USART1_RX));
	HAL_UART_Receive_DMA(&huart3,USART3_RX,sizeof(USART3_RX));
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//启动PWM波来控制舵机
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//启动PWM波来控制舵机
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);//启动PWM波来控制舵机		
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//启动PWM波来控制舵机
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);//启动PWM波来控制舵机
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//启动PWM波来控制舵机
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);//启动PWM波来控制舵机		
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);//启动PWM波来控制舵机



	/**********************************************************************
                               Order
                         ???????????????
**********************************************************************/
p_order=&Order;
p_order->KV=0.2;                                                  //??????
p_order->MODE=0;                                                 //MODE=0???? 1?????	
p_order->LEVELMODE=0;                                          //??????????????????У?
p_order->RE_CH[1]=1;	                             //????????? -1 ?? 1
p_order->RE_CH[2]=1;
p_order->RE_CH[3]=1;
p_order->RE_CH[4]=1;
p_order->RE_PID[1]=-1;	                         //?????????? -1 ?? 1
p_order->RE_PID[2]=-1;
p_order->RE_CON[1]=-1;
p_order->RE_CON[2]=-1;

p_order->RE_CCH[1]=1;	                             //????????? -1 ?? 1
p_order->RE_CCH[2]=1;
p_order->RE_CCH[3]=1;
p_order->RE_CCH[4]=1;
p_order->RE_CPID[1]=1;	                         //?????????? -1 ?? 1
p_order->RE_CPID[2]=-1;
p_order->RE_CCON[1]=-1;
p_order->RE_CCON[2]=-1;
p_order->MPU6050MODE=1; 
LINK_USER1.Flag=1;                    
/**********************************************************************
                       ?????? ????? ????? ????
**********************************************************************/
CH1Min=1000; CH1Mid=1550; CH1MAX=2000;
CH2Min=1050; CH2Mid=1500; CH2MAX=1950;
CH3Min=1000; CH3Mid=1500; CH3MAX=2000;
CH4Min=1200; CH4Mid=1570; CH4MAX=2200;
Pitch_Min=-45;Pitch_Max=45;
Roll_Min=-45;Roll_Max=45;

/**********************************************************************
                           ????У??????
**********************************************************************/
SBUS_CH.CH1_A[2]=SBUS_CH.CH2_A[2]=SBUS_CH.CH4_A[2]=3000;
}

