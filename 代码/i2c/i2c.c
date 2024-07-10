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
	SCL(1);//在高的时候边SDA，显示出起始或终止
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
    SDA(0); //因为SDA的开始电平不确定，先操作SDA
	SCL(0);
	SCL(1);
	SDA(1);
}
void send(uint8_t bite) //发一个字节
{
	for (uint8_t i=0;i<8;i++)
	{
		SCL(0);
		SDA((bite&(0x80>>i))>>(7-i));
		SCL(1);
	}
	SCL(0);
}
uint8_t receive()   //收一个字节
{
	uint8_t receive_data=0;
    SDA(1);//主机释放SDA，开漏高电平，相当于输入
	for(uint8_t i=0;i<8;i++)
	{
        SCL(0); //从机放
        SCL(1); //主机读
        receive_data|=   (((GPIO_I2C->IDR)>>GPIO_PIN_SDA)&1) <<(7-i);
	}
	SCL(0);
	return receive_data;
}
void send_ack(uint8_t i)
{
	SCL(0);//开始改变
	SDA(i);//应答
	SCL(1);//从机读取
	SCL(0);//下一次
}
uint8_t receive_ack(void)
{
	SDA(1);//主机释放
	uint8_t receive_ack;  
	SCL(0);  //从机放数据
	SCL(1);   //主机读取
	receive_ack=  ((GPIO_I2C->IDR)>>GPIO_PIN_SDA)&1;
	SCL(0);   //下一次
	return receive_ack;
}
