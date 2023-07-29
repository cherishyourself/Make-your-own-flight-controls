/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-08-06 21:32:25
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-07 11:15:12
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\POWER\Core\Inc\PID_Init.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef PID_INIT_
#define PID_INIT_
#include "main.h"
#include "PID.h"
#include "fuzzypid.h"
#include "CascadePID.h"
//********以下选择PID模式
#define SPID 0
#define FuzzyPID 1
#define CPID 2
#define PIDMODE CPID //单级SPID 模糊FuzzyPID 串级CascadePID
//*********

extern PID *proll,*ppitch,*pyaw,*cproll,*cppitch,*cpyaw ;
extern PID pproll,pppitch,ppyaw,cpproll,cpppitch,cppyaw; 
extern float CPID_MAXI[3],CPID_MAXO[3],CP_PID[4],CR_PID[4];;
extern float P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO;
extern float P_FPID_K,P_FPID_I,P_FPID_D,P_FPID_MAXI,P_FPID_MINI,P_FPID_MAXO,R_FPID_K,R_FPID_I,R_FPID_D,R_FPID_MAXI,R_FPID_MINI,R_FPID_MAXO;
extern void PID_Reinit(void);//PID���ض��� ����ͨѶ
extern void PID_AllInit(void);//PID������ʼ��
#endif