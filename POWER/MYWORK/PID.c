/*
 * @Author: cherish 1771907097@qq.com
 * @Date: 2022-07-10 10:50:48
 * @LastEditors: cherish 1771907097@qq.com
 * @LastEditTime: 2022-08-09 09:30:47
 * @FilePath: \MDK-ARMc:\Users\86131\Desktop\word\POWER\MYWORK\PID.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "PID.h"
#define err 7
#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))
//单级pid计算
void PID_SingleCalc(PID *pid,float reference,float feedback)
{
	//更新数据
	pid->lastError=pid->error;
	pid->error=reference-feedback;
	//抗闭环死区
	if(fabs((double)pid->error)>fabs((double)err)) pid->error=pid->error;
	else if(fabs((double)pid->error)<=fabs((double)err)) pid->error=0;
	//计算微分
	pid->output=(pid->error-pid->lastError)*pid->kd;
	//计算比例
	pid->output+=pid->error*pid->kp;
	//计算积分
	pid->integral+=pid->error*pid->ki;
	//积分限幅
	LIMIT(pid->integral,-pid->maxIntegral,pid->maxIntegral);
	pid->output+=pid->integral;
	//输出限幅
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

