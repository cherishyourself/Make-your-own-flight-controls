/*
本文件用于控制舵机
主要模式：【1】小板机和太阳能两种机型【2】小板机和太阳能下的手动和自稳模式【3】自稳模式下的普通PID和自适应模糊控制PID
*/
#include "servo_engine.h"
User_Order Order;
User_Order *p_order;
int Pangle_PWM[5]={0},PID_PWM[5]={0},CPID_PWM[5]={0};
extern SBUS_CH_Struct SBUS_CH;
/***********************************************************
***                       PID控制舵机                    ***
************************************************************/
void Control_Init(int CH1Min,int CH1Mid,int CH1MAX,int CH2Min,int CH2Mid,int CH2MAX,int CH3Min,int CH3Mid,int CH3MAX,int CH4Min,int CH4Mid,int CH4MAX,int Pitch_Min,int Pitch_Max,int Roll_Min,int Roll_Max)
{	
if(p_order->MODE==0)                                                                      //手动模式
{	
  #if(PLANE==FlyingWing)                                                                              //太阳能
  {
	HAL_Delay(1);
    Servo_Control1((p_order->RE_CH[1]*SBUS_CH.CH[1]+CH1Mid+p_order->RE_CH[2]*SBUS_CH.CH[2]));
	//printf("1:%d ",p_order->REVERSEMODE_CH[1]*SBUS_CH.CH[1]+CH1Mid+p_order->REVERSEMODE_CH[2]*SBUS_CH.CH[2]);
	Servo_Control5((p_order->RE_CH[1]*SBUS_CH.CH[1]+CH2Mid-p_order->RE_CH[2]*SBUS_CH.CH[2]));
	HAL_Delay(1);
	//printf("%0.1f ",p_order->REVERSEMODE_CH[2]*SBUS_CH.CH[2]*0.72);
	//printf("5:%.1f\r\n",p_order->REVERSEMODE_CH[1]*SBUS_CH.CH[1]+CH1Mid-p_order->REVERSEMODE_CH[2]*SBUS_CH.CH[2]*0.72);
	if(SBUS_CH.CH10<1000)
	{Servo_Control3(p_order->RE_CH[3]*SBUS_CH.leftv);                                  
	HAL_Delay(1);
	Servo_Control6(p_order->RE_CH[3]*SBUS_CH.rightv);                             //右油门
    }
}
	#elif(PLANE==FixedWing)                                                                                               //小板机
	{
	Servo_Control1((p_order->REVERSEMODE_CH[1]*SBUS_CH.CH[1]+CH1Mid));
		HAL_Delay(1);
	Servo_Control2(p_order->REVERSEMODE_CH[2]*SBUS_CH.CH[2]+CH2Mid);                                  
		HAL_Delay(1);
  Servo_Control3(p_order->REVERSEMODE_CH[3]*SBUS_CH.leftv);
		HAL_Delay(1);
		Servo_Control4(p_order->REVERSEMODE_CH[4]*SBUS_CH.rightv+CH4Mid);  
	}
	#endif
}																							 
if(p_order->MODE==1)                                                                       //自稳模式
{	
	#if(PLANE==FlyingWing)                                                                              //太阳能
	{	
		if(SBUS_CH.CH10<1000)
		{Servo_Control3(p_order->RE_CH[3]*SBUS_CH.leftv); Servo_Control6(p_order->RE_CH[3]*SBUS_CH.rightv);}  
	 #if(PIDMODE==FuzzyPID)                                                                             //fuzzy pid
	 {
 	 HAL_Delay(1);
	 Servo_Control1(Fuzzy_pid(fppitch,p_order->RE_PID[1]*SBUS_CH.CH[1]+CH1Mid,map(p_order->RE_PID[1]*pitch,Pitch_Min,Pitch_Max,CH1Min,CH1MAX))+CH1Mid);             
	 HAL_Delay(1);
	 Servo_Control2(Fuzzy_pid(fproll,p_order->RE_PID[2]*SBUS_CH.CH[2]+CH2Mid,map(p_order->RE_PID[2]*roll,Roll_Min,Roll_Max,CH2Min,CH2MAX))+CH2Mid);              
	 HAL_Delay(1);
	 }
   #elif (PIDMODE==SPID)                                                                                                //太阳能pid
   {
   HAL_Delay(1);
	 PID_SingleCalc(ppitch,Pangle_PWM[1],PID_PWM[1]);
	 Servo_Control1(p_order->RE_PID[1]*ppitch->output+CH1Mid-p_order->RE_PID[2]*proll->output);//PID控制翻滚角 right
   HAL_Delay(1);
	 PID_SingleCalc(proll,Pangle_PWM[2],PID_PWM[2]);
	 Servo_Control5(p_order->RE_PID[1]*ppitch->output+CH2Mid+p_order->RE_PID[2]*proll->output);//PID控制翻滚角 left
   HAL_Delay(1);
		// printf("1:%d ",Pangle_PWM[1]); printf("%d ",PID_PWM[1]); printf("%.1f ",p_order->REVERSEMODE_PID[1]*ppitch->output+CH1Mid-proll->output);
		 //printf("2:%d ",Pangle_PWM[2]); printf("%d ",PID_PWM[2]); printf("%.1f\r\n ",p_order->REVERSEMODE_PID[1]*ppitch->output+CH1Mid+proll->output);
   }
   #elif (PIDMODE==CPID) 
   {
	 CPID_Driver();
	  //*****************************************************+*注意这里的反向，后面要改进**********
	 Servo_Control1(p_order->RE_CPID[1]*cppitch->output+CH1Mid-p_order->RE_CPID[2]*cproll->output);//PID控制翻滚角 right
     HAL_Delay(1);
	 //******************************************************+(-1)*注意这里的反向，后面要改进**********
	 Servo_Control5(p_order->RE_CPID[1]*cppitch->output+CH2Mid+p_order->RE_CPID[2]*cproll->output);//PID控制翻滚角 left
   }  
   #endif
 }
 #elif (PLANE==FixedWing)                                                                               //小板机
	{  Servo_Control3(p_order->REVERSEMODE_CH[3]*SBUS_CH.leftv);
		Servo_Control4(p_order->REVERSEMODE_CH[4]*SBUS_CH.rightv+CH4Mid); 
	 #if(PIDMODE==FuzzyPID)                                                                             //fuzzy pid
	 {
 	 HAL_Delay(1);
	 Servo_Control1(Fuzzy_pid(fppitch,p_order->REVERSEMODE_CH[1]*SBUS_CH.CH[1]+CH1Mid,map(p_order->REVERSEMODE_PID[1]*pitch,Pitch_Min,Pitch_Max,CH1Min,CH1MAX))+CH1Mid);             
	 HAL_Delay(1);
	 Servo_Control2(Fuzzy_pid(fproll,p_order->REVERSEMODE_CH[2]*SBUS_CH.CH[2]+CH2Mid,map(p_order->REVERSEMODE_PID[2]*roll,Roll_Min,Roll_Max,CH2Min,CH2MAX))+CH2Mid);              
	 HAL_Delay(1);
	 }
   #elif(PIDMODE==PID)                                                                                                //小扳机PID
   {
   HAL_Delay(1);
	 PID_SingleCalc(ppitch,Pangle_PWM[1],PID_PWM[1]);
	 Servo_Control1(ppitch->output+CH1Mid);//PID控制翻滚角 right
   HAL_Delay(1);
	 PID_SingleCalc(proll,Pangle_PWM[2],PID_PWM[2]);
	 Servo_Control2(proll->output+CH2Mid);//PID控制翻滚角 left
   HAL_Delay(1);
	 }
	#elif (PIDMODE==CPID) 
   {

	
   }  

	 #endif
  }
	#endif
	} 
}

/******************************************************
***                 终端控制舵机                    ***
*******************************************************/
void Servo_Control1(int CH)//right
{	
if(CH>CH1MAX) CH=CH1MAX;
if(CH<CH1Min) CH=CH1Min;
//printf(" 1:%d ",CH);
   __HAL_TIM_SET_COMPARE(&htim4 , TIM_CHANNEL_4, (uint16_t )CH);
}
void Servo_Control2(int CH)//left
{ 	
if(CH>CH2MAX) CH=CH2MAX;
if(CH<CH2Min) CH=CH2Min;
		//printf(" 2:%d\r\n ",CH);
   __HAL_TIM_SET_COMPARE(&htim4 , TIM_CHANNEL_3, (uint16_t )CH);
}
void Servo_Control3(int CH)//roll
{   //printf(" 油门:%d",CH);
   __HAL_TIM_SET_COMPARE(&htim4 , TIM_CHANNEL_2, (uint16_t )CH);
}
void Servo_Control4(int CH)//roll
{	 
if(CH>CH4MAX) CH=CH4MAX;
if(CH<CH4Min) CH=CH4Min;
	// printf(" 4:%d\r\n ",CH);
   __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_1, (uint16_t )CH);
}
void Servo_Control5(int CH)//roll
{	  
if(CH>CH2MAX) CH=CH2MAX;
if(CH<CH2Min) CH=CH2Min;
// printf(" 5:%d\r\n ",CH);
   __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_2, (uint16_t )CH);
}
void Servo_Control6(int CH)//roll
{	  //printf(" 6:%d\r\n ",CH);
   __HAL_TIM_SET_COMPARE(&htim3 , TIM_CHANNEL_1, (uint16_t )CH);
}