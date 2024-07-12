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
	SCL(1);//在高的时候变SDA，显示出起始或终止
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
//指定地址写一个字节
void WriteReg(uint8_t address,uint8_t reg_address,uint8_t data)
{
    start();
    
    send(address);   //发送设备地址
    receive_ack();//判断从机有没有接收应答，为了拼接时序
    
    send(reg_address); //发送设备寄存器地址
    receive_ack();
    
    send(data);//指定地址下的数据
    receive_ack();
    
    stop();
}
uint16_t ReadReg(uint8_t address,uint8_t reg_address)
{
    uint16_t data;
    start();
    
    send(address);   //设备地址
    receive_ack();//判断从机有没有接收应答
    
    send(reg_address); //发送设备寄存器地址
    receive_ack();
    
    restart();
    send(address|1);  //意思是要读了
    receive_ack();
    
    data=receive();
    send_ack(1);//非应答
    
    stop();
    return data;
}

void ReadRegs(uint8_t address,uint8_t reg_address,uint8_t *buffer,uint8_t lenth)
{
    uint8_t i;
    start();
    
    send(address);   //设备地址
    receive_ack();//判断从机有没有接收应答
    
    send(reg_address); //发送设备寄存器地址
    receive_ack();
    
    restart();
    send(address|1);  //意思是要读了
    receive_ack();
    for(i=0;i<lenth-1;i++)
    {
        *(buffer+i)=receive();
         send_ack(0);//应答
    }
    *(buffer+i)=receive();
    send_ack(1);//非应答
    
    stop();
}
