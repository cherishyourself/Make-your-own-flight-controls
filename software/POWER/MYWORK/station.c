/*
这个文件包含地面站命令解析、涉及舵机位置调整，PID参数
详情请看本工程下的《通讯手册》
*/
#include "station.h"
LINK_USER  LINK_USER1;
//存储陀螺仪的绝对值值 存储陀螺仪的正负号      角度限制        差速    PID反向       控的反向    控通道最大最小中立值
int LEVEL[3]={0},   LEVEL_[5]={1,1,1,1,1},Angle_limit[4]={0},Dif,SRE_PID[3]={0},SRE_C[5]={0},Controller[5][5]={0};
float R_PID[4]={0},P_PID[4]={0};//储存roll和pitch的PID的参数
u8 USART3_RX[BUFFER_SIZE3];//数据接收	
/*    
**************************************************************
* 函 数 名: Command_Analyse()                                *
* 功能说明: 用于通讯命令解析                                  *
* 形    参: 无                                               *
* 返 回 值: 无                                               *
**************************************************************
*/ 
void Command_Analyse(void)
{
int i=0,k=0,j=0,limit=0;
/********************************************************************
	                          PID解析和打印数据指令
********************************************************************/
while(USART3_RX[i++]=='$')
 { 
	 LINK_USER1.Flag=0;//LINK_USER1.Flag!=1保证发送指令时不会触发
	 LINK_USER1.PID_P_Flag=0;
   LINK_USER1.PID_R_Flag=0;
		while(USART3_RX[i]!='#')
		{
		if(USART3_RX[i]==',')
		{k++; i+=1;}
	  if(limit==0)
    {
		if(USART3_RX[i]=='R') LINK_USER1.Message[0]='R';//Roll的pid参数
		if(USART3_RX[i]=='P') LINK_USER1.Message[1]='P';//pitch的pid参数
		if(USART3_RX[i]=='D'&&USART3_RX[i+1]=='D')	LINK_USER1.Message[4]='D';//陀螺仪查询（√）
		if(USART3_RX[i]=='D'&&USART3_RX[i+1]=='F') LINK_USER1.Message[5]='F';//差速设置
		if(USART3_RX[i]=='J'&&USART3_RX[i+1]=='Z') LINK_USER1.Message[6]='J';//校准陀螺仪（√）
		if(USART3_RX[i]=='A'&&USART3_RX[i+1]=='C') LINK_USER1.Message[7]='C';//遥控器校准（√）
		if(USART3_RX[i]=='A'&&USART3_RX[i+1]=='S') LINK_USER1.Message[8]='S';//遥控器校准（√）
		if(USART3_RX[i]=='1') LINK_USER1.Message[9]='1';//舵机调参（CH1）	
		if(USART3_RX[i]=='2') LINK_USER1.Message[10]='2';//舵机调参（CH2）	
		if(USART3_RX[i]=='4') LINK_USER1.Message[11]='4';//舵机调参（CH4）
		if(USART3_RX[i]=='C'&&USART3_RX[i+1]=='P') LINK_USER1.Station_PID='C';//串级PID
		if(USART3_RX[i]=='S'&&USART3_RX[i+1]=='P') LINK_USER1.Station_PID='S';//单级PID
	  }
	limit=1;//置为1，让判断指令语句只运行一遍
		if(LINK_USER1.Message[0]=='R')
   {	
	  switch (k)
		{ case 2: R_PID[0]=R_PID[0]*10+(USART3_RX[i]-48); break;
			case 3: R_PID[1]=R_PID[1]*10+(USART3_RX[i]-48); break;
			case 4: R_PID[2]=R_PID[2]*10+(USART3_RX[i]-48); break;
			case 5: Angle_limit[2]=Angle_limit[2]*10+(USART3_RX[i]-48); break;
			case 6: SRE_PID[2]=(USART3_RX[i]-48);break;
		}
   }
		if(LINK_USER1.Message[1]=='P')
		{
		switch (k)
		{ case 2: P_PID[0]=P_PID[0]*10+(USART3_RX[i]-48); break;
			case 3: P_PID[1]=P_PID[1]*10+(USART3_RX[i]-48); break;
			case 4: P_PID[2]=P_PID[2]*10+(USART3_RX[i]-48); break;
		  case 5: Angle_limit[1]=Angle_limit[1]*10+(USART3_RX[i]-48); break;
		  case 6: SRE_PID[1]=(USART3_RX[i]-48);break;
		}
	  }
			if(LINK_USER1.Message[5]=='F')
		{ Dif=Dif*10+(USART3_RX[i+3]-48);
			Dif=Dif*10+(USART3_RX[i+4]-48);
			break;
	  }
			if(LINK_USER1.Message[9]=='1')
   {	
	  switch (k)
		{ case 2: Controller[1][1]=Controller[1][1]*10+(USART3_RX[i]-48); break;
			case 3: Controller[1][2]=Controller[1][2]*10+(USART3_RX[i]-48); break;
			case 4: Controller[1][3]=Controller[1][3]*10+(USART3_RX[i]-48); break;
			case 5: SRE_C[1]=(USART3_RX[i]-48); break;
		}
   }
	 
	 		if(LINK_USER1.Message[10]=='2')
   {	
	  switch (k)
			{ case 2: Controller[2][1]=Controller[2][1]*10+(USART3_RX[i]-48);break;
			case 3: Controller[2][2]=Controller[2][2]*10+(USART3_RX[i]-48); break;
			case 4: Controller[2][3]=Controller[2][3]*10+(USART3_RX[i]-48); break;
			case 5: SRE_C[2]=(USART3_RX[i]-48); break;
		}
   }
	 	 		if(LINK_USER1.Message[11]=='4')
   {	
	  switch (k)
		{ case 2: Controller[4][1]=Controller[4][1]*10+(USART3_RX[i]-48); break;
			case 3: Controller[4][2]=Controller[4][2]*10+(USART3_RX[i]-48); break;
			case 4: Controller[4][3]=Controller[4][3]*10+(USART3_RX[i]-48); break;
			case 5: SRE_C[4]=(USART3_RX[i]-48); break;
		}
   }
		i++;	
		}
		memset(USART3_RX,0x00,sizeof(USART3_RX));
		break;
	}
/********************************************************************
	                           陀螺仪校准
********************************************************************/
while(USART3_RX[j++]=='&')
		{

			LINK_USER1.Flag=0;//LINK_USER1.Flag!=1保证发送指令时不会触发
			LEVEL_[0]=1,LEVEL_[1]=1;//存储正负号
			memset(LEVEL,0x00,sizeof(LEVEL));
			k=0;
			while(USART3_RX[j]!='#')
		{
		if(USART3_RX[j]==',')
		{
			k++; j+=1;
		}
		switch (k)
		{
			case 1: if(USART3_RX[j]=='R') LINK_USER1.Message[2]='r'; break;
			case 2:	if(USART3_RX[j]=='-') {LEVEL_[0]=-1; } else{LEVEL[0]=LEVEL[0]*10+(USART3_RX[j]-48)*LEVEL_[0];} break;
			case 3: if(USART3_RX[j]=='P') LINK_USER1.Message[3]='p'; break;
			case 4: if(USART3_RX[j]=='-') {LEVEL_[1]=-1; } else {LEVEL[1]=LEVEL[1]*10+(USART3_RX[j]-48)*LEVEL_[1];} break;
		}
		j++;
		}
		memset(USART3_RX,0x00,sizeof(USART3_RX));
		break;
	 }
/********************************************************************/
/********************************************************************/
	 Order_set(LINK_USER1.Message);

 }
/*    
**************************************************************
* 函 数 名: Order_set()                                      *
* 功能说明: 用于通讯命令解析后设置功能指令参数               *
* 形    参: 无                                               *
* 返 回 值: 无                                               *
**************************************************************
*/
void Order_set(u8* message)
{	
if(message[0]=='R')
{
Roll_Min=-1*Angle_limit[2]; Roll_Max=Angle_limit[2];
if(SRE_PID[2]==0) p_order->RE_PID[2]=-1; else if(SRE_PID[2]==1)p_order->RE_PID[2]=SRE_PID[2];
	if(LINK_USER1.Station_PID=='S')
	{
		R_PID_K=R_PID[0]/100; R_PID_I=R_PID[1]/100; R_PID_D=R_PID[2]/100;
    	printf("ROLL:P:%f I:%f D:%f A:%d RE:%d\r\n",R_PID_K,R_PID_I,R_PID_D,Roll_Max,p_order->RE_PID[2]);
	}
	else if(LINK_USER1.Station_PID=='C')
	{
		CR_PID[1]=R_PID[0]/100; CR_PID[2]=R_PID[1]/100; CR_PID[3]=R_PID[2]/100;
		printf("CROLL:P:%f I:%f D:%f A:%d RE:%d\r\n",CR_PID[1],CR_PID[2],CR_PID[3],Roll_Max,p_order->RE_PID[2]);
	}
PID_Reinit();//调整PID参数	
}

if(message[1]=='P')
{
Pitch_Min=-1*Angle_limit[1]; Pitch_Max=Angle_limit[1];
if(SRE_PID[1]==0) p_order->RE_PID[1]=-1; else if(SRE_PID[1]==1) p_order->RE_PID[1]=SRE_PID[1];
	if(LINK_USER1.Station_PID=='S')
	{
		P_PID_K=P_PID[0]/100; P_PID_I=P_PID[1]/100; P_PID_D=P_PID[2]/100;
		printf("PITCH:P:%f I:%f D:%f A:%d RE:%d\r\n",P_PID_K,P_PID_I,P_PID_D,Pitch_Max,p_order->RE_PID[1]);
	}
	else if(LINK_USER1.Station_PID=='C')
	{
		CP_PID[1]=P_PID[0]/100;CP_PID[2]=P_PID[1]/100;CP_PID[3]=P_PID[2]/100;
		printf("CPITCH:P:%f I:%f D:%f A:%d RE:%d\r\n",CP_PID[1],CP_PID[2],CP_PID[3],Pitch_Max,p_order->RE_PID[1]);
	}
PID_Reinit();	//调整PID参数
}

if(message[2]=='r')	{printf("roll:%.1f\r\n",roll);}
if(message[3]=='p') {printf("pitch:%.1f\r\n",pitch);}
if(message[4]=='D') {printf("p:%.1f ",pitch); printf("r:%.f ",roll);	printf("y:%.1f\r\n ",yaw);}
if(message[5]=='F') {p_order->KV=Dif/100.0;printf("Dif:%.2f\r\n",p_order->KV);}
if(message[6]=='J') {p_order->LEVELMODE=1;printf("LEVELMODE=1\r\n");}
if(message[7]=='C') {p_order->S_Adjust=1;p_order->S_Adjust_Sure=0;printf("Controller\r\n");}
if(message[8]=='S') {p_order->S_Adjust=0;p_order->S_Adjust_Sure=1;printf("Sure\r\n");}
if(message[9]=='1') {CH1Min=Controller[1][1]; CH1Mid=Controller[1][2]; CH1MAX=Controller[1][3];if(SRE_C[1]==0)p_order->RE_CH[1]=-1;else p_order->RE_CH[1]=1;printf("CH1%d %d %d %d\r\n",CH1Min,CH1Mid,CH1MAX,p_order->RE_CH[1]);}
if(message[10]=='2') {CH2Min=Controller[2][1]; CH2Mid=Controller[2][2]; CH2MAX=Controller[2][3];if(SRE_C[2]==0)p_order->RE_CH[2]=-1;else p_order->RE_CH[2]=1;printf("CH2%d %d %d %d\r\n",CH2Min,CH2Mid,CH2MAX,p_order->RE_CH[2]);}
if(message[11]=='4') {CH4Min=Controller[4][1]; CH4Mid=Controller[4][2]; CH4MAX=Controller[4][3];if(SRE_C[4]==0)p_order->RE_CH[4]=-1;else p_order->RE_CH[4]=1;printf("CH4%d %d %d %d\r\n",CH4Min,CH4Mid,CH4MAX,p_order->RE_CH[3]);}

LINK_USER1.PID_P_Flag=0; LINK_USER1.PID_R_Flag=0; Dif=0;
memset(Angle_limit,0x00,sizeof(Angle_limit));
memset (SRE_PID,0x00,sizeof(SRE_PID));
memset(R_PID,0x00,sizeof(R_PID));
memset(P_PID,0x00,sizeof(R_PID));
memset(LINK_USER1.Message,0x00,sizeof(LINK_USER1.Message));
memset(Controller,0x00,sizeof(Controller));
LINK_USER1.Flag=1;
}

void RudderDisplay()//QT界面显示舵量
{
//DMA_printf("@,%d,%d,%d,%d,#\r\n",SBUS_CH.CH[1]+CH1Mid,SBUS_CH.CH[2]+CH2Mid,SBUS_CH.CH[3],SBUS_CH.CH[4]+CH4Mid);
	DMA_printf("*,%.0f,%.0f,#",pitch,roll);
}