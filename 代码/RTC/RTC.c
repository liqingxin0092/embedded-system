#include "stm32f10x.h"
#include "RTC.h"
//�û������Լ�ʹ��
time_t CurrentTime;
struct tm CurrentTimeStruct;


//����
// CurrentTime= GetRTC_CNT();
// CurrentTimeStruct=*localtime(&CurrentTime);

/**
  * @brief  ��ȡ�趨ʱ���ʱ���
  * @param  ���������
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
    return mktime(&TimeStruct);// ��ȡʱ���
}

/**
  * @brief  ��ʼ��RTCʱ��
  * @param  time: ʱ���. flag���Ƿ������ù��粻��ʧ 1�ǣ�������   random:�������ֵ�������趨ʱ��
  **/
void RTC_Init(uint32_t time, uint8_t flag, uint16_t random)
{
    RCC->APB1ENR |= 3 << 27;//ʹ��PWR��BKPʱ��
    PWR->CR |= 1 << 8;//ʹ�ܶ�BKP��RTC�ķ���

    if (flag == 1)
    {
        if (BKP->DR1 != random)
            BKP->DR1 = random;
        else goto exit;
    }

    RCC->BDCR |= 1;    //����LSEʱ��
    while (((RCC->BDCR >> 1) & 1) == 0);//�ȴ�LSEʱ���������
    RCC->BDCR |= 1 << 8;  //ѡ��LSE��ΪRTCʱ��
    RCC->BDCR |= 1 << 15; //RTCʱ��ʹ��

    while (((RTC->CRL >> 3) & 1) == 0);//�ȴ�����ʱ��ͬ��
    while (((RTC->CRL >> 5) & 1) == 0);//�ȴ���һ��RTC�Ĳ������

    RTC->CRL |= 1 << 4; //��������ģʽ

    RTC->PRLL = 0x7fff;  //����Ԥ��Ƶ��ֵ
    RTC->CNTH = time >> 16;  //����ʱ��
    RTC->CNTL = time & 0xffff;

    RTC->CRL &= ~(1 << 4); //�˳�����ģʽ

    while (((RTC->CRL >> 5) & 1) == 0);//�ȴ���һ��RTC�Ĳ������

exit: while (0);
}
/**
  * @brief  ��ȡRTC��������ֵ
  * @param  ��
  **/
uint32_t GetRTC_CNT()
{
    return (uint32_t)(RTC->CNTH << 16) + RTC->CNTL;
}


