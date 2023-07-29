/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-11 11:01:09
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 13:56:23
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\station.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-11 11:01:09
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-03 22:28:50
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\station.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __COMMUNICATION_PROTOCOL_H
#define __COMMUNICATION_PROTOCOL_H
#include "main.h"
#include "mavlink_user.h"
#include "string.h"
#include "usart.h"
#include "stdio.h"
#include "GY25.h"
#include "sbus.h"
#include "PID_Init.h"
typedef struct LINK_
{
u8 LINK[10];
u8 Message[30];
int CH[20];
int Flag;
int PID_P_Flag;
int PID_R_Flag;
float PID[20];	
float Adjust[10][20];
u8 Station_PID;
}LINK_USER;
extern LINK_USER  LINK_USER1;
extern int LEVEL[3];
extern int Pitch_Min,Pitch_Max,Roll_Min,Roll_Max;
extern void PID_Reinit(void);
extern void Command_Analyse(void);
extern void Order_set(u8* message);
extern void RudderDisplay(void);//用于QT舵量显示  
#endif 
