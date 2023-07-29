/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 14:51:05
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\mavlink_user.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
���ļ���Ҫ�������ù���ָ��
*/
#ifndef __MAVLINK_USER_H
#define __MAVLINK_USER_H
#include "main.h"

typedef struct order
{
double KV;//��������ֵR 
int16_t LEVELMODE;//Ϊһ���Ե�ǰֵ��Ϊˮƽ����У׼ 0Ϊ��ֵ
uint16_t MODE;//0Ϊ�ֶ� 1Ϊ����
int16_t RE_CH[5];//�ֶ�����ģʽ -1 �� 1 (1����2����3����4����)
int16_t RE_PID[5];//���ȷ���ģʽ -1 �� 1 (1 pitch 2roll )
int16_t RE_CON[5];
int16_t RE_CCH[5];
int16_t RE_CPID[5];
int16_t RE_CCON[5];
int16_t MPU6050MODE;//0ΪGY25 1ΪGY25Z 2Ϊjy901b
int16_t C_Adjust;
int16_t S_Adjust;
int16_t S_Adjust_Sure;	
}User_Order;
extern User_Order Order;
extern User_Order *p_order;//����ṹ��ָ��
#endif