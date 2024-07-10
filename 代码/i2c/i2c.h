#ifndef __IC2_H
#define __IC2_H

#include "main.h"

#define GPIO_I2C          GPIOB
#define GPIO_PIN_SCL      13
#define GPIO_PIN_SDA      12

void deal(uint16_t Pin,GPIO_PinState PinState);

#define SCL(PinState)   deal(GPIO_PIN_SCL,(GPIO_PinState) (PinState))
#define SDA(PinState)   deal(GPIO_PIN_SDA,(GPIO_PinState) (PinState))

void start(void);
void stop(void);
void send(uint8_t bite);
uint8_t receive(void);
void send_ack(void);
uint8_t receive_ack(void);

#endif
