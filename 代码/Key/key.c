#include "stm32f10x.h" 
#include "Delay.h"
uint16_t Time;
uint8_t MultiKey(GPIO_TypeDef * GPIOx, uint8_t Pin, uint8_t TrigNum, uint16_t DelayTime)
{
    if(((GPIOx->IDR>>Pin)&1)==TrigNum)
    {
        Delay_ms(25); 
        if(((GPIOx->IDR>>Pin)&1)==TrigNum)
        {
             while(((GPIOx->IDR>>Pin)&1)==TrigNum)
             {
                Delay_ms(1);
                Time++;
             }
        }   
        if(Time!=0&&Time<DelayTime)
        {
            Delay_ms(25);
            Time=0;
            return 1;  
        }
        else if(Time>=DelayTime)
        {
            Delay_ms(25);
            Time=0;
            return 2;  
        }
    }
       return 0;
}
