/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/*MPU6050��������˵��:
		STM32C8T6										MPU6050
			3.3V												VCC
			GND													GND
			PB6													SCL
			PB7													SDA
*/
/*��������˵��:
		STM32C8T6										TTLתUSB
			3.3V												3.3V
			GND													GND
			PA9													RXD
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/*ͷ�ļ�����*/
#include "stm32f1xx_it.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include <stdio.h>
#include "PID.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "dma.h"  
#include "mavlink_user.h"
#include "delay_user.h"
#include "fuzzypid.h"
#include "sbus.h"
#include "servo_engine.h"
#include "Init.h"
#include "GY25.h"
#include "station.h"
#include "Flash.h"
#include "PID_Init.h"
/* USER CODE END Include */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/************************************
**
              �������             					
						                     	 **
*************************************/
int CH1Min_L, CH1MAX_L, CH2Min_L, CH2MAX_L,  CH3Min_L, CH3MAX_L, CH4Min_L, CH4MAX_L;
int CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX;
int Pitch_Min,Pitch_Max,Roll_Min,Roll_Max;
float pitch, roll, yaw;
PWR_PVDTypeDef sConfigPVD;//PVD�ṹ�嶨��
/************************************
**
              �ⲿ����          
						                     	 **
*************************************/
extern void Init_User(void);//���ֳ�ʼ���������жϣ�PWMʹ��   
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/*
**************************************************************
***������:map                                                *
***��  ��:ӳ��                                               *
***��  ��:val I_Min I_Max O_Min O_Max                        *
**************************************************************
*/
float map(float val, float I_Min, float I_Max, float O_Min, float O_Max)
{
return(((val-I_Min)*((O_Max-O_Min)/(I_Max-I_Min)))+O_Min);
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();

  /* USER CODE BEGIN 2 */

	/*
**************************************************************
***������:Init_User                                           *
***��  ��:�����жϣ�PWMʹ��                                    *
***��  ��:��                                                  *
**************************************************************
*/
Init_User();
/**********************************************************************
                            PID��FUZZYPID����
**********************************************************************/
PID_AllInit();
/**********************************************************************
                                 Flash��ȡ����
**********************************************************************/
//USER_PVD_Config(sConfigPVD,PWR_PVD_MODE_IT_RISING);
//USER_Flash_R();
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{	 			
/* USER CODE END WHILE */
/* USER CODE BEGIN 3 */
//printf("p:%d",Flash_Buf_WGY[0]);
//printf("r:%d",Flash_Buf_WGY[1]);
//printf("Gx:%0.1f",GY25_GYRO[1]);
//printf("Gy:%0.1f",GY25_GYRO[2]);
//printf("Gz:%0.1f",GY25_GYRO[3]);
//printf("p:%.1f ",pitch);
//printf("r:%.1f ",roll);
//rintf("y:%.1f\r\n ",yaw);
//printf("%s",USART1_RX);
/*
**************************************************************************************************************
***������:RudderDisplay                                                                                 *
***��  ��:����QT������ʾ                                                                           *
**************************************************************************************************************
*/
RudderDisplay();
/*
**************************************************************************************************************
***������:SBUS_Limit                                                                                    *
***��  ��:���ڵ�����ͨ��������                                                                  *
**************************************************************************************************************
*/
Limit_Init(CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX,Pitch_Min,Pitch_Max,Roll_Min,Roll_Max);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
SBUS_Analyse(huart);//����SBUS����
// Command_Analyse(huart);
	
HAL_UART_Receive_IT(&huart2,(uint8_t *)&aRxBuffer2,1);   //�ٿ��������ж�
//HAL_UART_Receive_IT(&huart3,(uint8_t *)&aRxBuffer3,1);   
}

int fputc(int ch ,FILE *f)
{
HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,0xffff);
	return ch;
}
//�ض���C�⺯��getchar��scanf��USART1
int fgetc (FILE *f)
{
uint8_t ch=0;
HAL_UART_Receive(&huart3,&ch,1,0xffff);
return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
		printf("ERROR");
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
