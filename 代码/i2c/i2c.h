#ifndef __IC2_H
#define __IC2_H

#include "main.h"
#include "i2c.h"

// User Configuration
#define I2C_SCL_PORT GPIOE
#define GPIO_PIN_SCL 0

#define I2C_SDA_PORT GPIOB
#define GPIO_PIN_SDA 8

// Basic Level
void deal(GPIO_TypeDef *IIC_PORT, uint16_t Pin, GPIO_PinState PinState);
#define SCL(PinState) deal(I2C_SCL_PORT, GPIO_PIN_SCL, (GPIO_PinState)(PinState))
#define SDA(PinState) deal(I2C_SDA_PORT, GPIO_PIN_SDA, (GPIO_PinState)(PinState))

// 时序层
void start(void);
void restart(void);
void stop(void);
void send(uint8_t bite);
uint8_t receive(void);
void send_ack(uint8_t i);
uint8_t receive_ack(void);

// 应用层
void WriteReg(uint8_t address, uint8_t reg_address, uint8_t data);
void WriteRegs(uint8_t *ptr, uint8_t length);
uint16_t ReadReg(uint8_t address, uint8_t reg_address);
void ReadRegs(uint8_t address, uint8_t reg_address, uint8_t *buffer, uint8_t lenth);

#endif
