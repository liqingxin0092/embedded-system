#include "i2c.h"
#include "Delay.h"

// User Configuration , Unit: microsecond
const uint16_t IIC_Delay_Time = 1;

void deal(GPIO_TypeDef *IIC_PORT, uint16_t Pin, GPIO_PinState PinState)
{
    if (PinState == 1)
    {
        IIC_PORT->ODR |= 1 << Pin;
    }
    else if (PinState == 0)
    {
        IIC_PORT->ODR &= ~(1 << Pin);
    }
}
void start()
{
    SCL(1);
    SDA(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
    SDA(0);
    SCL(0);
}
void restart()
{
    SDA(1);
    SCL(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
    SDA(0);
    SCL(0);
}
void stop()
{
    SDA(0);
    SCL(0);
    SCL(1);
    SDA(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
}
void send(uint8_t bite)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        SCL(0);
        SDA((bite & (0x80 >> i)) >> (7 - i));
        SCL(1);
        if (IIC_Delay_Time > 0)
            Delay_us(IIC_Delay_Time);
    }
    SCL(0);
}
uint8_t receive()
{
    uint8_t receive_data = 0;
    SDA(1);
    Delay_us(1);
    for (uint8_t i = 0; i < 8; i++)
    {
        SCL(0);
        SCL(1);
        Delay_us(1);
        receive_data |= (((I2C_SDA_PORT->IDR) >> GPIO_PIN_SDA) & 1) << (7 - i);
    }
    SCL(0);
    return receive_data;
}
void send_ack(uint8_t i)
{
    SCL(0);
    SDA(i);
    SCL(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
    SCL(0);
}
uint8_t receive_ack(void)
{
    SDA(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
    uint8_t receive_ack;
    SCL(0);
    SCL(1);
    if (IIC_Delay_Time > 0)
        Delay_us(IIC_Delay_Time);
    receive_ack = ((I2C_SDA_PORT->IDR) >> GPIO_PIN_SDA) & 1;
    SCL(0);
    return receive_ack;
}

void WriteReg(uint8_t address, uint8_t reg_address, uint8_t data)
{
    start();

    send(address);
    receive_ack();

    send(reg_address);
    receive_ack();

    send(data);
    receive_ack();

    stop();
}

void WriteRegs(uint8_t *ptr, uint8_t length)
{
    start();

    for (uint8_t i = 0; i < length; i++)
    {
        send(ptr[i]);
        receive_ack();
    }

    stop();
}
uint16_t ReadReg(uint8_t address, uint8_t reg_address)
{
    uint16_t data;
    start();

    send(address);
    receive_ack();

    send(reg_address);
    receive_ack();

    restart();
    send(address | 1);
    receive_ack();

    data = receive();
    send_ack(1);

    stop();
    return data;
}

void ReadRegs(uint8_t address, uint8_t reg_address, uint8_t *buffer, uint8_t lenth)
{
    uint8_t i;
    start();

    send(address);
    receive_ack();

    send(reg_address);
    receive_ack();

    restart();
    send(address | 1);
    receive_ack();
    for (i = 0; i < lenth - 1; i++)
    {
        *(buffer + i) = receive();
        send_ack(0);
    }
    *(buffer + i) = receive();
    send_ack(1);

    stop();
}
