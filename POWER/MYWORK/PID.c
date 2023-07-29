/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 10:50:48
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-09 09:30:47
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\word\POWER\MYWORK\PID.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "PID.h"
#define err 7
#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))
//����pid����
void PID_SingleCalc(PID *pid,float reference,float feedback)
{
	//��������
	pid->lastError=pid->error;
	pid->error=reference-feedback;
	//���ջ�����
	if(fabs((double)pid->error)>fabs((double)err)) pid->error=pid->error;
	else if(fabs((double)pid->error)<=fabs((double)err)) pid->error=0;
	//����΢��
	pid->output=(pid->error-pid->lastError)*pid->kd;
	//�������
	pid->output+=pid->error*pid->kp;
	//�������
	pid->integral+=pid->error*pid->ki;
	//�����޷�
	LIMIT(pid->integral,-pid->maxIntegral,pid->maxIntegral);
	pid->output+=pid->integral;
	//����޷�
	LIMIT(pid->output,-pid->maxOutput,pid->maxOutput);
}
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd=d;
	pid->maxIntegral=maxI;
	pid->maxOutput=maxOut;
}

