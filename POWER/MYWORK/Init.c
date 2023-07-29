/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-09 09:26:48
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\Init.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
���ļ����ڸ��ֳ�ʼ���������жϵĿ���,DMA�Ŀ���,PWM���Ŀ���
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
extern u8 aRxBuffer2[USART_REC_LEN];      //ʹ�õĴ��ڽ��ջ���
extern u8 USART1_RX[BUFFER_SIZE1] ;//GY25���ݽ���
void Init_User()
{
	//HAL_UART_Receive_IT(&huart3,(uint8_t*)&aRxBuffer3,1);//���������������ж�
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&aRxBuffer2,1);//�������ڶ������жϣ�����SBUS
	HAL_UART_Receive_IT(&huart1,(uint8_t*)&USART1_RX,1);//��������һ�����жϣ�����GY25
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //ʹ��IDLE�ж�
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	//DMA���պ������˾�һ��Ҫ�ӣ����ӽ��ղ�����һ�δ�������ʵ���ݣ��ǿյģ��Ҵ�ʱ���յ������ݳ���Ϊ�����������ݳ���
	HAL_UART_Receive_DMA(&huart1,USART1_RX,sizeof(USART1_RX));
	HAL_UART_Receive_DMA(&huart3,USART3_RX,sizeof(USART3_RX));
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//����PWM�������ƶ��
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//����PWM�������ƶ��
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);//����PWM�������ƶ��		
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//����PWM�������ƶ��
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);//����PWM�������ƶ��
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//����PWM�������ƶ��
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);//����PWM�������ƶ��		
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);//����PWM�������ƶ��



	/**********************************************************************
                               Order
                         ???????????????
**********************************************************************/
p_order=&Order;
p_order->KV=0.2;                                                  //??????
p_order->MODE=0;                                                 //MODE=0???? 1?????	
p_order->LEVELMODE=0;                                          //??????????????????��?
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
                           ????��??????
**********************************************************************/
SBUS_CH.CH1_A[2]=SBUS_CH.CH2_A[2]=SBUS_CH.CH4_A[2]=3000;
}

