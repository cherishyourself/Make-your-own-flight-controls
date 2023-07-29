/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 08:40:15
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\Flash.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "main.h"
#include "main.h"
#include "GY25.h"
#include "SBUS.h"
#include "stm32f1xx_it.h"
#include <stdio.h>
//=========================�������ͺ궨��
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define __IO    volatile 
typedef __IO uint16_t vu16;
 
//=========================�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	64 	 	//��ѡSTM32��FLASH������С(��λΪK)
    #if     STM32_FLASH_SIZE < 256      //����������С
    #define STM_SECTOR_SIZE     1024    //1K�ֽ� ����һҳ
    #else 
    #define STM_SECTOR_SIZE	    2048    //2K�ֽ�
    #endif	
#define STM32_FLASH_BASE    0x08000000 		//STM32 FLASH����ʼ��ַ
#define FLASH_SAVE_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*63	//дFlash�ĵ�ַ������ӵ����ڶ�ҳ��ʼ
#define STM32_FLASH_WREN 	1              	//ʹ��FLASHд��(0����ʹ��;1��ʹ��)
#define FLASH_WAITETIME  	50000          	//FLASH�ȴ���ʱʱ��
 
 
 
 
u8 STMFLASH_GetStatus(void);				  //���״̬
u8 STMFLASH_WaitDone(u16 time);				  //�ȴ���������
u8 STMFLASH_ErasePage(u32 paddr);			  //����ҳ
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat);//д�����
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void Flash_PageErase(uint32_t PageAddress);     //��������

void USER_Flash_W(void);//Flashд����
void USER_Flash_R(void);//Flahs������
extern PWR_PVDTypeDef sConfigPVD;//PVD�ṹ���ⲿ����
extern void USER_PVD_Config(PWR_PVDTypeDef PVD_,int PWR_PVD_MODE_IT);

extern int Flash_Buf_WCH[20];//Flash�ϵ�洢д��ͨ������
extern  int Flash_Buf_WGY[4];//Flash�ϵ�洢д��GY25����
extern  int Flash_Buf_RCH[20];//Flash�ϵ�洢��ȡͨ������
extern  int Flash_Buf_RGY[4];//Flash�ϵ�洢��ȡGY25����
#endif
