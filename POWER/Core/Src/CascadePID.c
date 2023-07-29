/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:22:51
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-09 08:33:15
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\Core\Src\CascadePID.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "CascadePID.h"
void CPID_Driver()
{
PID_SingleCalc(ppitch,Pangle_PWM[1],PID_PWM[1]);//角度环
PID_SingleCalc(proll,Pangle_PWM[2],PID_PWM[2]);

PID_SingleCalc(cppitch,p_order->RE_PID[1]*ppitch->output+CH1Mid,CPID_PWM[1]);//角速度环
PID_SingleCalc(cproll,p_order->RE_PID[2]*proll->output+CH2Mid,CPID_PWM[2]);
}