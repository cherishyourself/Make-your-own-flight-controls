/*
本文件用于解析SBUS
解析完的数据用于：【1】调整各通道上下限和功能设置【2】连接控制舵机API ：Control_Init
*/
#include "sbus.h"
//******************************************************************注意SBUS的串口通讯用100K波特率
extern LINK_USER  LINK_USER1;//station.h内结构体
SBUS_CH_Struct SBUS_CH;
/// BUS接收缓存区  串口接收缓冲          接收缓冲
u8 SBUS[30],aRxBuffer2[USART_REC_LEN],USART2_RX_BUF[USART_REC_LEN];                                          
u16 USART2_RX_STA=0;                 //接收标注位
u8 update_sbus(u8 *buf)//将sbus信号转化为通道值
{ 
    int i2;
    for (i2=0;i2<25;i2++)
        SBUS_CH.signal[i2] = buf[i2];
    if (buf[23] == 0)
    {
        SBUS_CH.ConnectState = 1;//遥控器与接收器连接状态 0=未连接，1=正常连接
        SBUS_CH.CH1 = ((int16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
        SBUS_CH.CH2 = ((int16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
        SBUS_CH.CH3 = ((int16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 ) | (int16_t)buf[ 5] << 10 ) & 0x07FF;
        SBUS_CH.CH4 = ((int16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
        SBUS_CH.CH5 = ((int16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
        SBUS_CH.CH6 = ((int16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 ) | (int16_t)buf[9] << 9 ) & 0x07FF;
        SBUS_CH.CH7 = ((int16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
        SBUS_CH.CH8 = ((int16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
        SBUS_CH.CH9 = ((int16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
        SBUS_CH.CH10 = ((int16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
	    SBUS_CH.CH11 = ((int16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 ) | (int16_t)buf[16] << 10 ) & 0x07FF;
	    SBUS_CH.CH12 = ((int16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
		SBUS_CH.CH13 = ((int16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
	    SBUS_CH.CH14 = ((int16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 ) | (int16_t)buf[20] << 9 ) & 0x07FF;
	  	SBUS_CH.CH15 = ((int16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
	 	SBUS_CH.CH16 = ((int16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;
		    //printf("CH1:%d ",SBUS_CH.CH1);
		    //printf("CH2:%d ",SBUS_CH.CH2);
		    //printf(" CH3:%d ",SBUS_CH.CH3);
		    //printf("CH4:%d ",SBUS_CH.CH4);
		    //printf(l" CH5:%d ",SBUS_CH.CH5);
	    	//printf(" CH7:%d ",SBUS_CH.CH7);
			// printf(" CH8:%d ",SBUS_CH.CH8);
		   //printf(" CH10:%d ",SBUS_CH.CH10);
			//  printf("1:%d",SBUS_CH.CH_AMax[1]); printf(" %d",SBUS_CH.CH_AMid[1]); printf(" %d ",SBUS_CH.CH_AMin[1]);
			//  printf("2:%d",SBUS_CH.CH_AMax[2]); printf(" %d",SBUS_CH.CH_AMid[2]); printf(" %d ",SBUS_CH.CH_AMin[2]);
			//  printf("4:%d",SBUS_CH.CH_AMax[4]); printf(" %d",SBUS_CH.CH_AMid[4]); printf(" %d\r\n",SBUS_CH.CH_AMin[4]);
        return 1;
    }
    else 
    {
        SBUS_CH.ConnectState = 0;
        return 0;
    }
}
/*
**************************************************************************************************************
***函数名:SBUS_Limit                                                                                         *
***功  能:调整各通道上下限和功能设置                                                                         *
***形  参:CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX;    *
**************************************************************************************************************
*/
void Limit_Init(int CH1Min,int CH1Mid,int CH1MAX,int CH2Min,int CH2Mid,int CH2MAX,int CH3Min,int CH3Mid,int CH3MAX,int CH4Min,int CH4Mid,int CH4MAX,int Pitch_Min,int Pitch_Max,int Roll_Min,int Roll_Max)	
{
/*********************************************************                                                                                   *
***                 副翼升降方向通道设置               ***
*********************************************************/		
//                        手动模式
// 通道原始值     校准后的中立值  map之后的通道值     通道原始     校准后中立值        校准后最大值
if(SBUS_CH.CH1>SBUS_CH.CH_AMid[1]) SBUS_CH.CH[1]=map(SBUS_CH.CH1,SBUS_CH.CH_AMid[1],SBUS_CH.CH_AMax[1],0,CH1MAX-CH1Mid);//一二参数根据接收SBUS信号范围调节
if(SBUS_CH.CH1<SBUS_CH.CH_AMid[1]) SBUS_CH.CH[1]=map(SBUS_CH.CH1,SBUS_CH.CH_AMin[1],SBUS_CH.CH_AMid[1],CH1Min-CH1Mid,0);
if(SBUS_CH.CH2>SBUS_CH.CH_AMid[2]) SBUS_CH.CH[2]=map(SBUS_CH.CH2,SBUS_CH.CH_AMid[2],SBUS_CH.CH_AMax[2],0,CH2MAX-CH2Mid);
if(SBUS_CH.CH2<SBUS_CH.CH_AMid[2]) SBUS_CH.CH[2]=map(SBUS_CH.CH2,SBUS_CH.CH_AMin[2],SBUS_CH.CH_AMid[2],CH2Min-CH2Mid,0);
if(SBUS_CH.CH4>SBUS_CH.CH_AMid[4]) SBUS_CH.CH[4]=map(SBUS_CH.CH4,SBUS_CH.CH_AMid[4],SBUS_CH.CH_AMax[4],0,CH4MAX-CH4Mid);
if(SBUS_CH.CH4<SBUS_CH.CH_AMid[4]) SBUS_CH.CH[4]=map(SBUS_CH.CH4,SBUS_CH.CH_AMin[4],SBUS_CH.CH_AMid[4],CH4Min-CH4Mid,0);	
// printf("CH1:%d",SBUS_CH.CH[1]);
// printf("CH2:%d",SBUS_CH.CH[2]);
// printf("CH4:%d",SBUS_CH.CH[4]);
// printf(" :%d",SBUS_CH.CH[1]+CH1Mid);
// printf(" :%d",SBUS_CH.CH[2]+CH2Mid);
// printf(" :%d\r\n",SBUS_CH.CH[4]+CH4Mid);
//                        自稳模式
if(pitch>=0) PID_PWM[1]=map(pitch,0,Pitch_Max,CH1Mid,(Pitch_Max/9.0+15)*100);
if(pitch<0) PID_PWM[1]=map(pitch,Pitch_Min,0,(Pitch_Min/9.0+15)*100,CH1Mid);
if(roll>=0) PID_PWM[2]=map(roll,0,Roll_Max,CH2Mid,(Roll_Max/9.0+15)*100);
if(roll<0)  PID_PWM[2]=map(roll,Roll_Min,0,(Roll_Min/9.0+15)*100,CH2Mid);
#if(PIDMODE==CPID)
if(GY25_GYRO[1]>=0) CPID_PWM[1]=map(GY25_GYRO[1],0,150,CH1Mid,(Pitch_Max/9.0+15)*100);
if(GY25_GYRO[1]<0)  CPID_PWM[1]=map(GY25_GYRO[1],-150,0,(Pitch_Min/9.0+15)*100,CH1Mid);
if(GY25_GYRO[2]<=0) CPID_PWM[2]=map(GY25_GYRO[2],0,150,CH2Mid,(Roll_Max/9.0+15)*100);
if(GY25_GYRO[2]>0)  CPID_PWM[2]=map(GY25_GYRO[2],-150,0,(Roll_Min/9.0+15)*100,CH2Mid);

if(SBUS_CH.CH1<SBUS_CH.CH_AMid[1]) Pangle_PWM[1]=map(p_order->RE_CCON[1]*SBUS_CH.CH[1]+CH1Mid,CH1Mid,CH1MAX,CH1Mid,(Pitch_Max/9.0+15)*100);
if(SBUS_CH.CH1>SBUS_CH.CH_AMid[1]) Pangle_PWM[1]=map(p_order->RE_CCON[1]*SBUS_CH.CH[1]+CH1Mid,CH1Min,CH1Mid,(Pitch_Min/9.0+15)*100,CH1Mid);
if(SBUS_CH.CH2>SBUS_CH.CH_AMid[2]) Pangle_PWM[2]=map(p_order->RE_CCON[2]*SBUS_CH.CH[2]+CH2Mid,CH2Mid,CH2MAX,CH2Mid,(Roll_Max/9.0+15)*100);
if(SBUS_CH.CH2<SBUS_CH.CH_AMid[2]) Pangle_PWM[2]=map(p_order->RE_CCON[2]*SBUS_CH.CH[2]+CH2Mid,CH2Min,CH2Mid,(Roll_Min/9.0+15)*100,CH2Mid);
#elif
if(SBUS_CH.CH1<SBUS_CH.CH_AMid[1]) Pangle_PWM[1]=map(p_order->RE_CON[1]*SBUS_CH.CH[1]+CH1Mid,CH1Mid,CH1MAX,CH1Mid,(Pitch_Max/9.0+15)*100);
if(SBUS_CH.CH1>SBUS_CH.CH_AMid[1]) Pangle_PWM[1]=map(p_order->RE_CON[1]*SBUS_CH.CH[1]+CH1Mid,CH1Min,CH1Mid,(Pitch_Min/9.0+15)*100,CH1Mid);
if(SBUS_CH.CH2>SBUS_CH.CH_AMid[2]) Pangle_PWM[2]=map(p_order->RE_CON[2]*SBUS_CH.CH[2]+CH2Mid,CH2Mid,CH2MAX,CH2Mid,(Roll_Max/9.0+15)*100);
if(SBUS_CH.CH2<SBUS_CH.CH_AMid[2]) Pangle_PWM[2]=map(p_order->RE_CON[2]*SBUS_CH.CH[2]+CH2Mid,CH2Min,CH2Mid,(Roll_Min/9.0+15)*100,CH2Mid);
#endif
/**********************************************************
***                       数据处理                      ***
***********************************************************/
	if(SBUS_CH.CH10>1000) SBUS_CH.CH[3]=1000;                    //油门锁
    else SBUS_CH.CH[3]=map(SBUS_CH.CH3,300,1700,CH3Min,CH3MAX);
	#if (PLANE==FlyingWing)                                             //如果是太阳能用差速			
	SBUS_CH.leftv=(SBUS_CH.CH[3])+(SBUS_CH.CH[4])*p_order->KV;SBUS_CH.rightv=(SBUS_CH.CH[3])-(SBUS_CH.CH[4])*p_order->KV;
	#elif (PLANE==FixedWing)  
	SBUS_CH.leftv=SBUS_CH.CH[3]; SBUS_CH.rightv=SBUS_CH.CH[4]; //如果是小板机 3油门4方向
    #endif
/***********************************************************
***                       PID控制舵机                    ***
************************************************************/
 Control_Init( CH1Min, CH1Mid, CH1MAX, CH2Min, CH2Mid, CH2MAX, CH3Min, CH3Mid, CH3MAX, CH4Min, CH4Mid, CH4MAX, Pitch_Min, Pitch_Max, Roll_Min, Roll_Max);
}
/*
**************************************************************
***函数名:SBUS_Analyse                                       *
***功  能:解析SUBS数据                                       *
***形  参:huart                                              *
**************************************************************
*/
void SBUS_Analyse(UART_HandleTypeDef *huart)
{
while(huart->Instance==USART2 )//如果是串口
	{
		USART2_RX_BUF[USART2_RX_STA] = aRxBuffer2[0];
		if (USART2_RX_STA == 0 && USART2_RX_BUF[USART2_RX_STA] != 0x0F) break; //帧头不对，丢掉
		USART2_RX_STA++;
		if (USART2_RX_STA > USART_REC_LEN) USART2_RX_STA = 0;  ///接收数据错误,重新开始接收
		if (USART2_RX_BUF[0] == 0x0F  && USART2_RX_STA == 25)	//接受完一帧数据	
		{int i;
			for (i = 0; i<25; i++)
			{SBUS[i] = USART2_RX_BUF[i]; USART2_RX_BUF[i] = 0;}
			USART2_RX_STA = 0;  update_sbus(SBUS);
//***********************手动自稳指令**************************
		if(SBUS_CH.CH6<1000) p_order->MODE=0; else p_order->MODE=1;
//********************陀螺仪水平校准指令***********************
	  	if(SBUS_CH.CH5<800) p_order->LEVELMODE=1; else if(SBUS_CH.CH5>800) p_order->LEVELMODE=0;	
//********************遥控器量程校准指令**************************
		if(SBUS_CH.CH8<1300) p_order->C_Adjust=1; else p_order->C_Adjust=0;
		if(p_order->C_Adjust==1||p_order->S_Adjust==1) //LINK_USER1.Flag!=1保证发送指令时不会触发
		{    
			if(LINK_USER1.Flag!=0)
			{
			SBUS_CH.CH[3]=1000;//油门锁
			if(SBUS_CH.CH1_A[0]<SBUS_CH.CH1) SBUS_CH.CH1_A[0]=SBUS_CH.CH1;
			 if(SBUS_CH.CH1_A[2]>SBUS_CH.CH1) SBUS_CH.CH1_A[2]=SBUS_CH.CH1;
			if(SBUS_CH.CH2_A[0]<SBUS_CH.CH2) SBUS_CH.CH2_A[0]=SBUS_CH.CH2;
			 if(SBUS_CH.CH2_A[2]>SBUS_CH.CH2) SBUS_CH.CH2_A[2]=SBUS_CH.CH2;
			if(SBUS_CH.CH4_A[0]<SBUS_CH.CH4) SBUS_CH.CH4_A[0]=SBUS_CH.CH4;
			 if(SBUS_CH.CH4_A[2]>SBUS_CH.CH4) SBUS_CH.CH4_A[2]=SBUS_CH.CH4;
			}
		}
        if(SBUS_CH.CH8<800||p_order->S_Adjust_Sure==1) 
        { 
			if(LINK_USER1.Flag!=0)
			{
		    SBUS_CH.CH[3]=1000;//油门锁
			SBUS_CH.CH_AMid[1]=SBUS_CH.CH1; SBUS_CH.CH_AMid[2]=SBUS_CH.CH2; SBUS_CH.CH_AMid[4]=SBUS_CH.CH4;
		    SBUS_CH.CH_AMax[1]=SBUS_CH.CH1_A[0]; SBUS_CH.CH_AMin[1]=SBUS_CH.CH1_A[2];
		    SBUS_CH.CH_AMax[2]=SBUS_CH.CH2_A[0]; SBUS_CH.CH_AMin[2]=SBUS_CH.CH2_A[2];
			SBUS_CH.CH_AMax[3]=SBUS_CH.CH3_A[0]; SBUS_CH.CH_AMin[3]=SBUS_CH.CH3_A[2];
		    SBUS_CH.CH_AMax[4]=SBUS_CH.CH4_A[0]; SBUS_CH.CH_AMin[4]=SBUS_CH.CH4_A[2];
		    printf("TRUE");
		    p_order->S_Adjust_Sure=0;
			}
		}
        memset(SBUS,0x00,sizeof(SBUS));
	  	}
				  break;
    }
}