#ifndef __PID_H
#define __PID_H
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "fuzzypid.h"
#include "mavlink_user.h"
typedef struct _PID
{
	float kp,ki,kd;
	float error,lastError;//���ϴ����
	float integral,maxIntegral;//���֡������޷�
	float output,maxOutput;//���������޷�
}PID;
void PID_SingleCalc(PID *pid,float reference,float feedback);
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
#endif
