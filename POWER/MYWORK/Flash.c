#include "Flash.h"
FLASH_ProcessTypeDef p_Flash; 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];    //��������
int Flash_Buf_WCH[20]={0};//Flash�ϵ�洢д��ͨ������
int Flash_Buf_WGY[4]={0};//Flash�ϵ�洢д��GY25����
int Flash_Buf_RCH[20]={0};//Flash�ϵ�洢��ȡͨ������
int Flash_Buf_RGY[4]={0};//Flash�ϵ�洢��ȡGY25����
/******************ע��*****************
 //ÿһ������(16λ)�տ�ʼ��ʼֵΪ0xFFFF
***************************************/

 /**********************************************************************************
  * ��������: ��ȡָ����ַ�İ���(16λ����) 
  * �������: faddr������ַ
  * �� �� ֵ: ��Ӧ����
  * ˵    ���� 
  **********************************************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
 
#if STM32_FLASH_WREN	//���ʹ����д   
 /**********************************************************************************
  * ��������:������д��
  * �������: WriteAddr:��ʼ��ַ��pBuffer:����ָ�롢NumToWrite:����(16λ)�� 
  * �� �� ֵ: ��
  * ˵    ���� 
  **********************************************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);//���ֶ�����ȡ��ַ���洢��λ��
	    WriteAddr+=2;//��ַ����2.
	}  
} 
 /**********************************************************************************
  * ��������:��ָ����ַ��ʼд��ָ�����ȵ�����
  * �������:WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)��pBuffer:����ָ�롢NumToWrite������(16λ)��(����Ҫд���16λ���ݵĸ���.)
  * �� �� ֵ: ��
  * ˵    ���� 
  **********************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	HAL_FLASH_Unlock();					    //����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~64 for STM32F103C8T6   
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	//                        WriteAddr
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������ ��ʼ��ַ��:����ָ�롢����(16λ)��
		for(i=0;i<secremain;i++)	//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)				//��Ҫ����
		{
			Flash_PageErase(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	//�����������
			FLASH_WaitForLastOperation(FLASH_WAITETIME);            	//�ȴ��ϴβ������
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);							//���CR�Ĵ�����PERλ���˲���Ӧ����FLASH_PageErase()����ɣ�
																		//����HAL�����沢û������Ӧ����HAL��bug��
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else 
		{
			FLASH_WaitForLastOperation(FLASH_WAITETIME);       	//�ȴ��ϴβ������
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
		}
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();		//����
}
#endif
 /**********************************************************************************
  * ��������:��ָ����ַ��ʼ����ָ�����ȵ�����
  * �������:ReadAddr:��ʼ��ַ��pBuffer:����ָ�롢NumToWrite:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ���� 
  **********************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
/*******************************************
***
               �û��Զ������Flash
                                          ***
*********************************************/
 void USER_Flash_W()
 {	
//******�洢GY25����******
Flash_Buf_WGY[0]=GY25_Now[0];
Flash_Buf_WGY[1]=GY25_Now[1];
//******�洢ͨ������******
Flash_Buf_WCH[0]=SBUS_CH.CH_AMax[1]; Flash_Buf_WCH[1]=SBUS_CH.CH_AMid[1]; Flash_Buf_WCH[2]=SBUS_CH.CH_AMin[1];
Flash_Buf_WCH[3]=SBUS_CH.CH_AMax[2]; Flash_Buf_WCH[4]=SBUS_CH.CH_AMid[2]; Flash_Buf_WCH[5]=SBUS_CH.CH_AMin[2];
Flash_Buf_WCH[6]=SBUS_CH.CH_AMax[3]; Flash_Buf_WCH[7]=SBUS_CH.CH_AMid[3]; Flash_Buf_WCH[8]=SBUS_CH.CH_AMin[3];
	
STMFLASH_Write(FLASH_SAVE_ADDR,(uint16_t *)Flash_Buf_WGY,Flash_WGY);
STMFLASH_Write(FLASH_SAVE_ADDR+(u32)Flash_WGY,(uint16_t *)Flash_Buf_WCH,Flash_WCH);
	 printf("OK\r\n");
 }
  void USER_Flash_R()
	{
STMFLASH_Read(FLASH_SAVE_ADDR,(uint16_t *)Flash_Buf_RGY,Flash_RGY); 
STMFLASH_Read(FLASH_SAVE_ADDR+(u32)Flash_WGY,(uint16_t *)Flash_Buf_RCH,Flash_RCH);
//******��ȡ���ݡ���GY25*******
GY25_Now[0]=Flash_Buf_RGY[0];
GY25_Now[1]=Flash_Buf_RGY[1];
//******��ȡ���ݡ���ͨ��*******
SBUS_CH.CH_AMax[1]=Flash_Buf_RCH[0]; SBUS_CH.CH_AMid[1]=Flash_Buf_RCH[1]; SBUS_CH.CH_AMin[1]=Flash_Buf_RCH[2];
SBUS_CH.CH_AMax[2]=Flash_Buf_RCH[3]; SBUS_CH.CH_AMid[2]=Flash_Buf_RCH[4]; SBUS_CH.CH_AMin[2]=Flash_Buf_RCH[5];
SBUS_CH.CH_AMax[3]=Flash_Buf_RCH[6]; SBUS_CH.CH_AMid[3]=Flash_Buf_RCH[7]; SBUS_CH.CH_AMin[3]=Flash_Buf_RCH[8];
	}
	 
 /**********************************************************************************
  * ��������:��������
  * �������:PageAddress:����������ַ
  * �� �� ֵ: ��
  * ˵    ���� 
  ***********************************************************************************/
void Flash_PageErase(uint32_t PageAddress)
{
  /* Clean the error context */
  p_Flash.ErrorCode = HAL_FLASH_ERROR_NONE;
 
#if defined(FLASH_BANK2_END)
  if(PageAddress > FLASH_BANK1_END)
  { 
    /* Proceed to erase the page */
    SET_BIT(FLASH->CR2, FLASH_CR2_PER);
    WRITE_REG(FLASH->AR2, PageAddress);
    SET_BIT(FLASH->CR2, FLASH_CR2_STRT);
  }
  else
  {
#endif /* FLASH_BANK2_END */
    /* Proceed to erase the page */
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, PageAddress);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
#if defined(FLASH_BANK2_END)
 
  }
#endif /* FLASH_BANK2_END */
  }

