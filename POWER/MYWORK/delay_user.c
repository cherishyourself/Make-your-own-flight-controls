#include "delay_user.h"
					
#ifdef DELAY_USE_OS

static uint32_t fac_us = 72; //us��ʱ���������˴�Ϊϵͳʱ��Ƶ�ʣ���λMHZ

//��ʱnus
//nusΪҪ��ʱ��us��.
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)
void Delay_us(uint32_t us)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD��ֵ
	ticks = us * fac_us;						 //��Ҫ�Ľ�����
	told = SysTick->VAL;						 //�ս���ʱ�ļ�����ֵ
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void Delay_ms(uint32_t ms)
{
	uint32_t i;
	for (i = 0; i < ms; i++)
		Delay_us(1000);
}

#else

void Delay_us(uint32_t us)
{
	uint32_t temp;
	SysTick->LOAD = 9 * us;
	SysTick->VAL = 0X00;	//��ռ�����
	SysTick->CTRL = 0X01; //ʹ�ܣ����������޶����������ⲿʱ��Դ
	do
	{
		temp = SysTick->CTRL;														//��ȡ��ǰ������ֵ
	} while ((temp & 0x01) && (!(temp & (1 << 16)))); //�ȴ�ʱ�䵽��
	SysTick->CTRL = 0x00;															//�رռ�����
}

void Delay_ms(uint32_t ms)
{
	uint32_t temp;
	SysTick->LOAD = 9000 * ms;
	SysTick->VAL = 0X00;	//��ռ�����
	SysTick->CTRL = 0X01; //ʹ�ܣ����������޶����������ⲿʱ��Դ
	do
	{
		temp = SysTick->CTRL;														//��ȡ��ǰ������ֵ
	} while ((temp & 0x01) && (!(temp & (1 << 16)))); //�ȴ�ʱ�䵽��
	SysTick->CTRL = 0x00;															//�رռ�����
}


//******************************************��ͨ��ʱ**************��
//΢�뼶����ʱ
void delay_us(uint32_t delay_us)
{    
  volatile unsigned int num;
  volatile unsigned int t;
 
  
  for (num = 0; num < delay_us; num++)
  {
    t = 11;
    while (t != 0)
    {
      t--;
    }
  }
}
//���뼶����ʱ
void delay_ms(uint16_t delay_ms)
{    
  volatile unsigned int num;
  for (num = 0; num < delay_ms; num++)
  {
    delay_us(1000);
  }
}



#endif
