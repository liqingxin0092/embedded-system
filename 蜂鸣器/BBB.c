 #include "BBB.h"
 
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//清空当前计数值//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}
void Delay_ms(uint32_t xms)
{
    uint32_t i=xms;
    while(i--)
    {
        Delay_us(1000);
    }
}


uint16_t  i;
void mid_1(uint16_t time)
{
    for( i =0;i<523*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/523));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/523));
    }   
}

void mid_2(uint16_t time)
{
    for( i =0;i<587*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/587));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/587));
    } 
}
void mid_3(uint16_t time)
{
    for( i =0;i<659*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/659));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/659));
    } 
}
void mid_4(uint16_t time)
{
    for( i =0;i<698*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/698));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/698));
    }   
}

void mid_5(uint16_t time)
{
    for( i =0;i<784*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/784));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/784));
    } 
}
void mid_6(uint16_t time)
{
{
    for( i =0;i<880*time/1000;i++)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/880));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/880));
    } 
}
void mid_7(uint16_t time)
{
{
    for( i =0;i<988*time/1000;i++)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/988));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/988));
    } 
}

void high_1(uint16_t time)
{
    for( i =0;i<1046*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1046));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1046));
    } 
}
void high_2(uint16_t time)
{
    for( i =0;i<1175*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1175));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1175));
    } 
}
void high_3(uint16_t time)
{
    for( i =0;i<1318*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1318));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1318));
    } 
}
void high_4(uint16_t time)
{
    for( i =0;i<1397*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1397));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1397));
    } 
}
void high_5(uint16_t time)
{
    for( i =0;i<1568*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1568));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1568));
    } 
}
void high_6(uint16_t time)
{
    for( i =0;i<1760*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1760));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1760));
    } 
}
void high_7(uint16_t time)
{
    
    for( i =0;i<1976*time/1000;i++)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET );
        Delay_us((uint32_t)(500000/1976));
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET );
        Delay_us((uint32_t)(500000/1976));
    } 
    
}
    
void play_FlowerDance()
{
     high_3(250);
     high_2(250);
     high_6(250);
     high_2(250);
     high_3(250);
     high_2(250);
     mid_6(250);
     high_2(250);
     
     Delay_ms(100);
      
     mid_3(400);
     high_6(300);
     mid_1(120);
     mid_3(140);
     mid_2(200);
     mid_5(500);
     
     Delay_ms(100);
     
     mid_2(290);
     mid_7(160);
     mid_6(210);
     mid_5(160);
     mid_6(360);
 }
