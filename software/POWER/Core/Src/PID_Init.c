#include "PID_Init.h"
//     ������ ������ ƫ����
 PID *proll,*ppitch,*pyaw,*cproll,*cppitch,*cpyaw ;
 PID pproll,pppitch,ppyaw,cpproll,cpppitch,cppyaw; 
float P_PID_MAXI,P_PID_MAXO,R_PID_MAXI,R_PID_MAXO,P_PID_K,P_PID_I,P_PID_D,R_PID_K,R_PID_I,R_PID_D;//PID
float CPID_MAXI[3],CPID_MAXO[3],CP_PID[4],CR_PID[4];

/*
**************************************************************
***������:PID_AllInit                                        *
***��  ��:��ʼ��PID����                                        *
***��  ��:��                                                 *
**************************************************************
*/
void PID_AllInit()
{
  #if(PIDMODE==SPID)
 {	
//   K          I         D            �������        ������
P_PID_K=1;P_PID_I=0;P_PID_D=0.1;P_PID_MAXI=2000;P_PID_MAXO=2000;//PITCH����
R_PID_K=1.02;R_PID_I=0;R_PID_D=0.3;R_PID_MAXI=2000;R_PID_MAXO=2000;//ROLL����
proll=&pproll;
ppitch=&pppitch;
pyaw=&ppyaw; 
PID_Init(ppitch,P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO);
PID_Init(proll,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO);
 }
 #elif (PIDMODE==FuzzyPID)
 {
	 //   K          I         D            �������        ��С����           ������
P_FPID_K=1;P_PID_I=0;P_FPID_D=0.01;P_FPID_MAXO=2000;P_FPID_MAXI=2000;P_FPID_MINI=2000;//PITCH����
R_FPID_K=1;R_FPID_I=0;R_FPID_D=0.01;R_FPID_MAXO=2000;P_FPID_MAXI=2000;R_FPID_MINI=2000;//ROLL����
fproll=&fpproll;
fppitch=&fpppitch;
fpyaw=&fppyaw;
 FUZZY_Init(fppitch,P_FPID_K,P_FPID_I,P_FPID_D,P_FPID_MAXO,P_FPID_MAXI,P_FPID_MINI);
 FUZZY_Init(fproll,R_FPID_K,R_FPID_I,R_FPID_D,R_FPID_MAXO,R_FPID_MAXI,R_FPID_MINI);
 }
 #elif (PIDMODE==CPID)
 {
	 //   K          I         D            �������        ������
P_PID_K=1;P_PID_I=0;P_PID_D=0;P_PID_MAXI=2000;P_PID_MAXO=2000;//PITCH����
R_PID_K=1;R_PID_I=0;R_PID_D=0;R_PID_MAXI=2000;R_PID_MAXO=2000;//ROLL����
	 
//   K          I         D            �������        ������
CP_PID[1]=1;CP_PID[2]=0;CP_PID[3]=0.1;CPID_MAXI[1]=2000;CPID_MAXO[1]=2000;//PITCH����
CR_PID[1]=1.02;CR_PID[2]=0;CR_PID[3]=0.3;CPID_MAXI[2]=2000;CPID_MAXO[2]=2000;//ROLL����
proll=&pproll,ppitch=&pppitch,pyaw=&ppyaw;//λ�û�
cproll=&cpproll,cppitch=&cpppitch,cpyaw=&cppyaw;//�ٶȻ�

PID_Init(ppitch,P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO);
PID_Init(proll,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO);
PID_Init(cppitch,CP_PID[1],CP_PID[2],CP_PID[3],CPID_MAXI[1],CPID_MAXO[1]);
PID_Init(cproll,CR_PID[1],CR_PID[2],CR_PID[3],CPID_MAXI[2],CPID_MAXI[2]);
 }
 #endif
 }



/*
**************************************************************
***������:PID_Reinit                                         *
***��  ��:����PID����                                        *
***��  ��:��                                                 *
**************************************************************
*/
void PID_Reinit()
{
 #if(PIDMODE==SPID)
 {	
PID_Init(ppitch,P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO);
PID_Init(proll,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO);
 }
 #elif (PIDMODE==FuzzyPID)
 {
 FUZZY_Init(fppitch,P_FPID_K,P_FPID_I,P_FPID_D,P_FPID_MAXO,P_FPID_MAXI,P_FPID_MINI);
 FUZZY_Init(fproll,R_FPID_K,R_FPID_I,R_FPID_D,R_FPID_MAXO,R_FPID_MAXI,R_FPID_MINI);
 }
 #elif (PIDMODE==CPID)
 {
PID_Init(ppitch,P_PID_K,P_PID_I,P_PID_D,P_PID_MAXI,P_PID_MAXO);
PID_Init(proll,R_PID_K,R_PID_I,R_PID_D,R_PID_MAXI,R_PID_MAXO);
PID_Init(cppitch,CP_PID[1],CP_PID[2],CP_PID[3],CPID_MAXI[1],CPID_MAXO[1]);
PID_Init(cproll,CR_PID[1],CR_PID[2],CR_PID[3],CPID_MAXI[2],CPID_MAXI[2]);
 }
 #endif
}