/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:18:24
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 07:30:51
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\MYWORK\sbus.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SBUS_H
#define __SBUS_H
#include "main.h"
#include "mavlink_user.h"
#include "stdio.h"
#include "string.h"
#include "servo_engine.h"
#include "station.h"
#include "Flash.h"
extern User_Order Order;              //�ṹ�����ͱ���
extern User_Order *p_order;          //����ṹ��ָ��
typedef struct
{
	float leftv,rightv;//��������
    int16_t signal[25];
	int16_t CH1;//ͨ��1��ֵ
	int16_t CH2;//ͨ��2��ֵ
	int16_t CH3;//ͨ��3��ֵ
	int16_t CH4;//ͨ��4��ֵ
	int16_t CH5;//ͨ��5��ֵ
	int16_t CH6;//ͨ��6��ֵ
    int16_t CH7;//ͨ��7��ֵ
    int16_t CH8;//ͨ��8��ֵ
    int16_t CH9;//ͨ��9��ֵ
    int16_t CH10;//ͨ��10��ֵ
	int16_t CH11;//ͨ��11��ֵ
	int16_t CH12;//ͨ��12��ֵ
	int16_t CH13;//ͨ��13��ֵ
	int16_t CH14;//ͨ��14��ֵ
	int16_t CH15;//ͨ��15��ֵ
	int16_t CH16;//ͨ��16��ֵ
	uint8_t ConnectState;//ң���������������״̬ 0=δ���ӣ�1=��������
	int16_t CH[20];//�洢�Լ�����������ݣ���������
	int16_t CH1_A[5],CH2_A[5],CH3_A[5],CH4_A[5];//,0  2�����ͨ�������ֵ����Сֵ 1�洢����ֵ
	int16_t CH_AMax[5],CH_AMid[5],CH_AMin[5];
}SBUS_CH_Struct;
extern SBUS_CH_Struct SBUS_CH;
extern int CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX,Pitch_Min,Pitch_Max,Roll_Min,Roll_Max;
//SBUS�źŽ�����غ���
extern u8 update_sbus(u8 *buf);
void SBUS_Analyse(UART_HandleTypeDef *huart);//����SBUS
extern void Limit_Init(int CH1Min,int CH1Mid,int CH1MAX,int CH2Min,int CH2Mid,int CH2MAX,int CH3Min,int CH3Mid,int CH3MAX,int CH4Min,int CH4Mid,int CH4MAX,int Pitch_Min,int Pitch_Max,int Roll_Min,int Roll_Max);
#endif
