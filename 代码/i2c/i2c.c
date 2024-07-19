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

 //硬件方式
void GPIOx_I2c_ToFast_Init()
{
    //I2C1重映射
    RCC->APB2ENR|= RCC_APB2ENR_AFIOEN;  //使能复用时钟
    AFIO->MAPR|=1<<1;   
    
   //初始化I2c外设
    RCC->APB1ENR|=1<<21;//开启i2c1外设时钟 
    I2C1->CR2|=11110;//设置时钟频率30MHZ
    I2C1->CR2&=~((1<<11)|(1<<8)|(1<<9));
    I2C1->CCR|=1<<15; //快速模式
    I2C1->CCR&=~(1<<14);//占空比1：2
    I2C1->CCR|=25;//给时钟频率 到400khz
    I2C1->TRISE=7; //设置上升时间 200ns 

    //初始化GPIO口
    RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;  //开启GPIOB时钟
    GPIOB->CRH=0x444444ff; //PB8,9端口配置为复用开漏输出 50MHZ
    GPIOB->ODR|=(1<<8)|(1<<9);//上拉     
    //i2c模块使能
    I2C1->CR1|=1;
}
void Hardare_WriteReg(uint8_t address,uint8_t reg_address,uint8_t data)
{
    
    uint8_t i=0;

    I2C1->CR1|=1<<8; //产生起始条件
    while(((I2C1->SR1)&1)==0);//等待起始条件发送 对
    
    I2C1->DR=address; //发送地址
    while((((I2C1->SR1)>>1)&1)==0);//等待地址发送结束   对
    
    I2C1->DR=reg_address; //发送寄存器地址 
    I2C1->DR=data; //发送数据
    while((((I2C1->SR1)>>2)&1)==1);
    
    I2C1->CR1|=1<<9;               //产生终止条件          
}
uint16_t Hardare_ReadReg(uint8_t address,uint8_t reg_address)
{
    uint16_t data=0;   
    
    I2C1->CR1|=1<<8; //产生起始条件
    while(((I2C1->SR1)&1)==0);//等待起始条件发送
    
    I2C1->DR=address; //发送地址
    while((((I2C1->SR1)>>1)&1)==0);//等待地址发送结束
    
    I2C1->DR=reg_address; //发送寄存器地址 
    while((((I2C1->SR1)>>2)&1)==1);
    
    I2C1->CR1|=1<<9;               //产生终止条件 
    
    
    
    
    
    
    
    
    I2C1->CR1|=1<<10;//应答使能
    I2C1->CR1|=1<<8; //产生起始条件
    while(((I2C1->SR1)&1)==0);//等待起始条件发送

    I2C1->DR=address|1; //发送地址
    while((((I2C1->SR1)>>1)&1)==1);//等待地址发送结束
         
    I2C1->CR1&=~(1<<10);         //应答失能 
    data=I2C1->DR;
    while((((I2C1->SR1)>>2)&1)==1);
    data=I2C1->DR;
    I2C1->CR1|=1<<9;             //产生终止条件  
        data=I2C1->DR; 
    while((((I2C1->SR1)>>6)&1)==0);
    
    return data;
}
