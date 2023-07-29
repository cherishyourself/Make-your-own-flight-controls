/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 11:28:52
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-09-25 09:55:55
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\GY25.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __GY25_H__
#define __GY25_H__
#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "mavlink_user.h"
extern float GY25_pitch,GY25_yaw,GY25_roll;
extern float GY25_Now[5],GY25_GYRO[10],GY25_ACC[10];
void GY25_Analyse(void);
#endif