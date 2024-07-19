#include "stm32f10x.h"
#include "string.h"
#include "Delay.h"
#include "stdio.h"
void UART_Init()
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

    GPIOA->CRH = 0x444444b4;                  
    USART1->BRR = 0x271;                           // 115200
                                                   // 默认8位无校验
    USART1->CR1 |= (1 << 2) | (1 << 3) | (1 << 5); // 使能发送接收和接收中断
    USART1->CR1 |= 1 << 13;
}
void SendString(char *str) // 发送数组
{
    uint8_t i;
    for (i = 0; *(str + i) != '\0'; i++)
    {
        USART1->DR = i[str];
        while (((USART1->SR >> 7) & 1) != 1)
            ;
    }
}
void SendByte(uint8_t Byte)
{
    USART1->DR = Byte;
    while (((USART1->SR >> 7) & 1) != 1);
}
int fputc(int ch,FILE *f)
{
    SendByte(ch);
    return ch;
}
// 这个是定长接收，数据数量要严格对齐
#define FIXED_NUMBER 6
uint8_t arr[FIXED_NUMBER];
void USART1_IRQHandler() // 接收中断
{
    if (((USART1->SR >> 5) & 1) == 1)
    {
        uint8_t i;
        for (i = 0; i < FIXED_NUMBER - 1; i++)
        {
            *(arr + i) = USART1->DR;
            while (((USART1->SR >> 5) & 1) != 1)
                ;
        }
        *(arr + FIXED_NUMBER - 1) = USART1->DR;
        i = 0;
    }
}
void NVIC_Config()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // 注意分组就一次

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = 37;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
