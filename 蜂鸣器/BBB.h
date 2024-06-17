#ifndef __BBB_H
#define __BBB_H


#include "main.h"

extern uint16_t  i;
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);

void mid_1(uint16_t time);
void mid_2(uint16_t time);
void mid_3(uint16_t time);
void mid_4(uint16_t time);
void mid_5(uint16_t time);
void mid_6(uint16_t time);
void mid_7(uint16_t time);

void high_1(uint16_t time);
void high_2(uint16_t time);
void high_3(uint16_t time);
void high_4(uint16_t time);
void high_5(uint16_t time);
void high_6(uint16_t time);
void high_7(uint16_t time);

void play_FlowerDance(void);
    
#endif
