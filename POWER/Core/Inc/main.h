/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 10:50:14
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 08:18:27
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\Core\Inc\main.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//mapӳ�亯��
extern float map(float val, float I_Min, float I_Max, float O_Min, float O_Max);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/*************************************
***              
               �궨�� 
                                   ***
*************************************/
#define FixedWing 0
#define FlyingWing 1
#define PLANE FlyingWing   //Ϊ0��С���  1��̫����

#define  pitch GY25_pitch
#define  roll GY25_roll
#define  yaw  GY25_yaw

#define Flash_WGY sizeof(Flash_Buf_WGY)
#define Flash_WCH sizeof(Flash_Buf_WCH)
#define Flash_RGY sizeof(Flash_Buf_RGY)
#define Flash_RCH sizeof(Flash_Buf_RCH)	
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef uint8_t u8;
typedef uint16_t u16; 
typedef uint32_t u32;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern int CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX,Pitch_Min,Pitch_Max,Roll_Min,Roll_Max;
extern float P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO;//PID
extern float P_FPID_K,P_FPID_I,P_FPID_D,P_FPID_MAXI,P_FPID_MINI,P_FPID_MAXO,R_FPID_K,R_FPID_I,R_FPID_D,R_FPID_MAXI,R_FPID_MINI,R_FPID_MAXO;//FPID
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
