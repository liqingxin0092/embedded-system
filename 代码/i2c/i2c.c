#include "i2c.h"
void deal(uint16_t Pin,GPIO_PinState PinState)
{
    if(PinState==1)
    {
        GPIO_I2C->ODR |=1<<Pin;
    }
    else if (PinState==0)
    {
        GPIO_I2C->ODR&=~(1<<Pin);
    }
}
void start()
{	
	SCL(1);//�ڸߵ�ʱ���SDA����ʾ����ʼ����ֹ
    SDA(1);
	SDA(0);
	SCL(0);
}
void restart()
{
    SDA(1);
    SCL(1);
    SDA(0);
	SCL(0);
}
void stop()
{	
    SDA(0); //��ΪSDA�Ŀ�ʼ��ƽ��ȷ�����Ȳ���SDA
	SCL(0);
	SCL(1);
	SDA(1);
}
void send(uint8_t bite) //��һ���ֽ�
{
	for (uint8_t i=0;i<8;i++)
	{
		SCL(0);
		SDA((bite&(0x80>>i))>>(7-i));
		SCL(1);
	}
	SCL(0);
}
uint8_t receive()   //��һ���ֽ�
{
	uint8_t receive_data=0;
    SDA(1);//�����ͷ�SDA����©�ߵ�ƽ���൱������
	for(uint8_t i=0;i<8;i++)
	{
        SCL(0); //�ӻ���
        SCL(1); //������
        receive_data|=   (((GPIO_I2C->IDR)>>GPIO_PIN_SDA)&1) <<(7-i);
	}
	SCL(0);
	return receive_data;
}
void send_ack(uint8_t i)
{
	SCL(0);//��ʼ�ı�
	SDA(i);//Ӧ��
	SCL(1);//�ӻ���ȡ
	SCL(0);//��һ��
}
uint8_t receive_ack(void)
{
	SDA(1);//�����ͷ�
	uint8_t receive_ack;  
	SCL(0);  //�ӻ�������
	SCL(1);   //������ȡ
	receive_ack=  ((GPIO_I2C->IDR)>>GPIO_PIN_SDA)&1;
	SCL(0);   //��һ��
	return receive_ack;
}
