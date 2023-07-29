#include "delay_user.h"
					
#ifdef DELAY_USE_OS

static uint32_t fac_us = 72; //us延时倍乘数，此处为系统时钟频率，单位MHZ

//延时nus
//nus为要延时的us数.
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)
void Delay_us(uint32_t us)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD的值
	ticks = us * fac_us;						 //需要的节拍数
	told = SysTick->VAL;						 //刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
}

//延时nms
//nms:要延时的ms数
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
	SysTick->VAL = 0X00;	//清空计数器
	SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
	do
	{
		temp = SysTick->CTRL;														//读取当前倒计数值
	} while ((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
	SysTick->CTRL = 0x00;															//关闭计数器
}

void Delay_ms(uint32_t ms)
{
	uint32_t temp;
	SysTick->LOAD = 9000 * ms;
	SysTick->VAL = 0X00;	//清空计数器
	SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
	do
	{
		temp = SysTick->CTRL;														//读取当前倒计数值
	} while ((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
	SysTick->CTRL = 0x00;															//关闭计数器
}


//******************************************普通延时**************、
//微秒级的延时
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
//毫秒级的延时
void delay_ms(uint16_t delay_ms)
{    
  volatile unsigned int num;
  for (num = 0; num < delay_ms; num++)
  {
    delay_us(1000);
  }
}



#endif
