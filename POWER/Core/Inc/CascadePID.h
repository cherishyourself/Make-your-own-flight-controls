/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:22:39
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 10:27:59
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\Core\Inc\CascadePID.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef CASCADEPID_H_
#define CASCADEPID_H_
#include "main.h"
#include "PID.h"
#include "PID_Init.h"
#include "GY25.h"
#include "sbus.h"
extern void CPID_Driver(void);
#endif