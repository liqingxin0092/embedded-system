#ifndef __RTC_H
#define __RTC_H

#include "time.h"
extern time_t CurrentTime;
extern struct tm CurrentTimeStruct;
time_t GetTimeStamp(uint8_t hour, uint8_t min, uint8_t sec, uint16_t year, uint8_t month, uint8_t day);
void RTC_Init(uint32_t time, uint8_t flag, uint16_t random);
uint32_t GetRTC_CNT(void);

#endif



