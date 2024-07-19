#ifndef __IC2_H
#define __IC2_H

#include "main.h"

#define GPIO_I2C          GPIOB
#define GPIO_PIN_SCL      8
#define GPIO_PIN_SDA      9

void deal(uint16_t Pin,GPIO_PinState PinState);

#define SCL(PinState)   deal(GPIO_PIN_SCL,(GPIO_PinState) (PinState))
#define SDA(PinState)   deal(GPIO_PIN_SDA,(GPIO_PinState) (PinState))
//软件时序层
void start(void);  
void restart(void);
void stop(void);
void send(uint8_t bite);
uint8_t receive(void);
void send_ack(uint8_t i);
uint8_t receive_ack(void);
//软件应用层
void WriteReg(uint8_t address,uint8_t reg_address,uint8_t data);
uint16_t ReadReg(uint8_t address,uint8_t reg_address);
void ReadRegs(uint8_t address,uint8_t reg_address,uint8_t *buffer,uint8_t lenth);
//硬件i2c  //注意IO口不同
void GPIOx_I2c_ToFast_Init(void);   //硬件初始化，注意是快速模式 30mhz
void Hardare_WriteReg(uint8_t address,uint8_t reg_address,uint8_t data);
uint16_t Hardare_ReadReg(uint8_t address,uint8_t reg_address);
#endif
