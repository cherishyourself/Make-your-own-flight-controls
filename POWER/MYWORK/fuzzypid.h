/******************************************************************************
/// @brief:模糊pid控制器
/// @time:2021-8-31
/// 

/// THE SOFTWARE.
*******************************************************************************/
#include "main.h"
#ifndef __fuzzypid_H
#define __fuzzypid_H

enum {
    FUZZY_LLAST	= 0,
    FUZZY_LAST 	= 1,
    FUZZY_NOW 	= 2,

  
};

#define NB -6
#define NM -4
#define NS -2
#define ZO 0
#define PS 2
#define PM 4
#define PB 6


typedef struct    //定义pid结构体
{
	
	  float set;
    float fuzzy_kp;
    float fuzzy_ki;
    float fuzzy_kd;
	  float fuzzy_max_out;
	  float fuzzy_err[3];
	  float fuzzy_i_band;
	  int fuzzy_out;
	  
	  float max_input;
	  float min_input;
	
	  float maxdKp;   //模糊pid计算增量最值
	  float mindKp;
	  float maxdKi;
	  float mindKi;
	  
	  float maxdKd;
	  float mindKd;
	  

} fuzzy_pid;
extern fuzzy_pid fpproll,fpppitch,fppyaw;
extern fuzzy_pid * fproll;//翻滚角
extern fuzzy_pid * fppitch;//俯仰角
extern fuzzy_pid * fpyaw;//偏航角
extern float P_FPID_K,P_FPID_I,P_FPID_D,P_FPID_MAXI,P_FPID_MINI,P_FPID_MAXO,R_FPID_K,R_FPID_I,R_FPID_D,R_FPID_MAXI,R_FPID_MINI,R_FPID_MAXO;//FPID

extern fuzzy_pid fuzz_contrl;
//void PID_Reset(fuzzy_pid *pid,float kp_get,float ki_get,float kd_get);
void FUZZY_Init(fuzzy_pid *fuzzy_pid,float kp_get,float ki_get,float kd_get,float maxout,float maxinput,float mininput);
static void LinearQuantization(fuzzy_pid *vPID,float pv,float *qValue);
float LinearRealization(int max,int min,float value);
static void CalcMembership(float *ms,float qv,int * index);
static void Fuzzy_Calc(fuzzy_pid *vPID,float pv,float *deltaK);
float Fuzzy_pid(fuzzy_pid *vPID,float set,float get);  //pid，位式;
#endif
