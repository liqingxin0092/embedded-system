#ifndef __KEY_H
#define __KEY_H

extern uint16_t Time;
uint8_t MultiKey(GPIO_TypeDef * GPIOx, uint8_t Pin, uint8_t TrigNum, uint16_t DelayTime);


#endif
