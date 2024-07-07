#include "stm32f10x.h"
#include "RTC.h"
//用户可以自己使用
time_t CurrentTime;
struct tm CurrentTimeStruct;


//例如
// CurrentTime= GetRTC_CNT();
// CurrentTimeStruct=*localtime(&CurrentTime);

/**
  * @brief  获取设定时间的时间戳
  * @param  参数请意会
  **/
time_t GetTimeStamp(uint8_t hour, uint8_t min, uint8_t sec, uint16_t year, uint8_t month, uint8_t day)
{
    struct tm  TimeStruct;

    TimeStruct.tm_sec = sec;
    TimeStruct.tm_min = min;
    TimeStruct.tm_hour = hour;
    TimeStruct.tm_mday = day;
    TimeStruct.tm_mon = month;
    TimeStruct.tm_year = year - 1900;
    return mktime(&TimeStruct);// 获取时间戳
}

/**
  * @brief  初始化RTC时钟
  * @param  time: 时间戳. flag：是否开启备用供电不丢失 1是，其他否   random:更改这个值以重新设定时间
  **/
void RTC_Init(uint32_t time, uint8_t flag, uint16_t random)
{
    RCC->APB1ENR |= 3 << 27;//使能PWR和BKP时钟
    PWR->CR |= 1 << 8;//使能对BKP和RTC的访问

    if (flag == 1)
    {
        if (BKP->DR1 != random)
            BKP->DR1 = random;
        else goto exit;
    }

    RCC->BDCR |= 1;    //开启LSE时钟
    while (((RCC->BDCR >> 1) & 1) == 0);//等待LSE时钟启动完成
    RCC->BDCR |= 1 << 8;  //选择LSE作为RTC时钟
    RCC->BDCR |= 1 << 15; //RTC时钟使能

    while (((RTC->CRL >> 3) & 1) == 0);//等待两个时钟同步
    while (((RTC->CRL >> 5) & 1) == 0);//等待上一次RTC的操作完成

    RTC->CRL |= 1 << 4; //进入配置模式

    RTC->PRLL = 0x7fff;  //配置预分频的值
    RTC->CNTH = time >> 16;  //配置时间
    RTC->CNTL = time & 0xffff;

    RTC->CRL &= ~(1 << 4); //退出配置模式

    while (((RTC->CRL >> 5) & 1) == 0);//等待上一次RTC的操作完成

exit: while (0);
}
/**
  * @brief  读取RTC计数器的值
  * @param  无
  **/
uint32_t GetRTC_CNT()
{
    return (uint32_t)(RTC->CNTH << 16) + RTC->CNTL;
}


