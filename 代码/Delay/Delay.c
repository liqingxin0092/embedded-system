#include "delay.h"
void Delay_us(uint32_t xus)		// 注意LOAD寄存器是24位的
{								// 最大值16,777,215
	SysTick->LOAD = 72 * xus;	// 设定重装值，系数是:系统时钟/1000,000
	SysTick->VAL = 0x00;		// 当前值清0
	SysTick->CTRL = 0x00000005; // 选用AHB为时钟源，不开启到0请求，使能计数器
	while (!(SysTick->CTRL & (1 << 16)))
		;						// 看这个寄存器第16位，等待它是0的情况，1的时候说明倒数到0了
	SysTick->CTRL = 0x00000004; // 关闭计数器
}
void Delay_ms(uint32_t xms)
{
	uint32_t i = xms;
	while (i--)
	{
		Delay_us(1000);
	}
}