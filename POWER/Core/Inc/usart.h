/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 10:50:15
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-06 08:51:10
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\Core\Inc\usart.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
extern void DMA_printf(const char *format, ...);
//���ڶ�SBUS����
#define USART_REC_LEN 25                    //���ջ�������ֽ���
extern u8 aRxBuffer2[USART_REC_LEN];      //使用的串口接收缓冲
extern  volatile uint8_t rx_len ;  //����һ֡���ݵĳ���
extern volatile uint8_t recv_end_flag; //һ֡���ݽ�����ɱ�־
extern uint8_t rx_buffer[100];  //�������ݻ�������

//����һ����GY25
#define BUFFER_SIZE1  1000//��������ֽڳ���
extern u8 USART1_RX[BUFFER_SIZE1];//GY25���ݽ���

//����������ͨѶ
#define BUFFER_SIZE3  1000//��������ֽڳ���
extern uint8_t RX_Flag3;//һ֡������ɱ�עλ
extern u8 USART3_RX[BUFFER_SIZE3];//GY25���ݽ���
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
