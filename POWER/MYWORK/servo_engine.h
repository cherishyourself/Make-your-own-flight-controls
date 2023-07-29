/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 09:36:23
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\servo_engine.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SERVO_ENGINE_H
#define __SERVO_ENGINE_H
#include "mavlink_user.h"
#include "sbus.h"
#include "PID.h"
#include "fuzzypid.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "stdio.h"
#include "PID_Init.h"
extern int Pangle_PWM[5],PID_PWM[5],CPID_PWM[5];
extern void Servo_Control1(int CH);//right
extern void Servo_Control2(int CH);//left
extern void Servo_Control3(int CH);//roll
extern void Servo_Control4(int CH);//roll 
extern void Servo_Control5(int CH);//roll 
extern void Servo_Control6(int CH);//roll 
extern u8 update_sbus(u8 *buf);      
extern float pitch, roll, yaw;          //����
extern int CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX,Pitch_Min,Pitch_Max,Roll_Min,Roll_Max;
extern int CH1Min_L, CH1MAX_L, CH2Min_L, CH2MAX_L,  CH3Min_L, CH3MAX_L, CH4Min_L, CH4MAX_L;
extern void Control_Init(int CH1Min,int CH1Mid,int CH1MAX,int CH2Min,int CH2Mid,int CH2MAX,int CH3Min,int CH3Mid,int CH3MAX,int CH4Min,int CH4Mid,int CH4MAX,int Pitch_Min,int Pitch_Max,int Roll_Min,int Roll_Max);
#endif