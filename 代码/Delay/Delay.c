#include "main.h"
void Delay_us(uint32_t xus)		// ע��LOAD�Ĵ�����24λ��
{								// ���ֵ16,777,215
	SysTick->LOAD = 72 * xus;	// �趨��װֵ��ϵ����:ϵͳʱ��/1000,000
	SysTick->VAL = 0x00;		// ��ǰֵ��0
	SysTick->CTRL = 0x00000005; // ѡ��AHBΪʱ��Դ����������0����ʹ�ܼ�����
	while (!(SysTick->CTRL & (1 << 16)))
		;						// ������Ĵ�����16λ���ȴ�����0�������1��ʱ��˵��������0��
	SysTick->CTRL = 0x00000004; // �رռ�����
}
void Delay_ms(uint32_t xms)
{
	uint32_t i = xms;
	while (i--)
	{
		Delay_us(1000);
	}
}