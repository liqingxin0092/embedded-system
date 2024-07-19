#ifndef __UART_H
#define __UART_H

void UART_Init(void);
void SendByte(uint8_t Byte);
void SendString(char *str);
void NVIC_Config(void);
#endif // __UART_H
