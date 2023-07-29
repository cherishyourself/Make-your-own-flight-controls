#include "Flash.h"
FLASH_ProcessTypeDef p_Flash; 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];    //缓存数组
int Flash_Buf_WCH[20]={0};//Flash断电存储写入通道数据
int Flash_Buf_WGY[4]={0};//Flash断电存储写入GY25数据
int Flash_Buf_RCH[20]={0};//Flash断电存储读取通道数据
int Flash_Buf_RGY[4]={0};//Flash断电存储读取GY25数据
/******************注意*****************
 //每一个半字(16位)刚开始初始值为0xFFFF
***************************************/

 /**********************************************************************************
  * 函数功能: 读取指定地址的半字(16位数据) 
  * 输入参数: faddr：读地址
  * 返 回 值: 对应数据
  * 说    明： 
  **********************************************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
 
#if STM32_FLASH_WREN	//如果使能了写   
 /**********************************************************************************
  * 函数功能:不检查的写入
  * 输入参数: WriteAddr:起始地址、pBuffer:数据指针、NumToWrite:半字(16位)数 
  * 返 回 值: 无
  * 说    明： 
  **********************************************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);//半字读，读取地址，存储的位置
	    WriteAddr+=2;//地址增加2.
	}  
} 
 /**********************************************************************************
  * 函数功能:从指定地址开始写入指定长度的数据
  * 输入参数:WriteAddr:起始地址(此地址必须为2的倍数!!)、pBuffer:数据指针、NumToWrite：半字(16位)数(就是要写入的16位数据的个数.)
  * 返 回 值: 无
  * 说    明： 
  **********************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	
	HAL_FLASH_Unlock();					    //解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~64 for STM32F103C8T6   
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	//                        WriteAddr
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容 起始地址、:数据指针、半字(16位)数
		for(i=0;i<secremain;i++)	//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)				//需要擦除
		{
			Flash_PageErase(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	//擦除这个扇区
			FLASH_WaitForLastOperation(FLASH_WAITETIME);            	//等待上次操作完成
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);							//清除CR寄存器的PER位，此操作应该在FLASH_PageErase()中完成！
																		//但是HAL库里面并没有做，应该是HAL库bug！
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else 
		{
			FLASH_WaitForLastOperation(FLASH_WAITETIME);       	//等待上次操作完成
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 
		}
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	HAL_FLASH_Lock();		//上锁
}
#endif
 /**********************************************************************************
  * 函数功能:从指定地址开始读出指定长度的数据
  * 输入参数:ReadAddr:起始地址、pBuffer:数据指针、NumToWrite:半字(16位)数
  * 返 回 值: 无
  * 说    明： 
  **********************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}
/*******************************************
***
               用户自定义读存Flash
                                          ***
*********************************************/
 void USER_Flash_W()
 {	
//******存储GY25数据******
Flash_Buf_WGY[0]=GY25_Now[0];
Flash_Buf_WGY[1]=GY25_Now[1];
//******存储通道数据******
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
//******读取数据——GY25*******
GY25_Now[0]=Flash_Buf_RGY[0];
GY25_Now[1]=Flash_Buf_RGY[1];
//******读取数据——通道*******
SBUS_CH.CH_AMax[1]=Flash_Buf_RCH[0]; SBUS_CH.CH_AMid[1]=Flash_Buf_RCH[1]; SBUS_CH.CH_AMin[1]=Flash_Buf_RCH[2];
SBUS_CH.CH_AMax[2]=Flash_Buf_RCH[3]; SBUS_CH.CH_AMid[2]=Flash_Buf_RCH[4]; SBUS_CH.CH_AMin[2]=Flash_Buf_RCH[5];
SBUS_CH.CH_AMax[3]=Flash_Buf_RCH[6]; SBUS_CH.CH_AMid[3]=Flash_Buf_RCH[7]; SBUS_CH.CH_AMin[3]=Flash_Buf_RCH[8];
	}
	 
 /**********************************************************************************
  * 函数功能:擦除扇区
  * 输入参数:PageAddress:擦除扇区地址
  * 返 回 值: 无
  * 说    明： 
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

