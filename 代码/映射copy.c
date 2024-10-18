/*当前映射*/

// D0 ~ PD14
// D1 ~ PD15
// D2 ~ PD0
// D3 ~ PD1
// D4 ~ PE7
// D5 ~ PE8
// D6 ~ PE9
// D7 ~ PE10
// D8 ~ PE11
// D9 ~ PE12
// D10 ~ PE13
// D11 ~ PE14
// D12 ~ PE15
// D13 ~ PD8
// D14 ~ PD9
// D15 ~ PD10

// WE ~ PD5
// LCD_CS ~ PG12
// RS ~ PF12
// RD ~ PD4
// RST ~ RST(单片机复位)
// BL ~ PB15(经过三极管驱动反转后,低电平灭,高电平亮.)

/*触摸部分*/
// T_CS ~ PC13
// T_MISO ~ PB12
// T_MOSI ~ PF11
// T_SCK ~ PB0
// T_PEN ~ PB1

/*PORT基地址*/
//  GPIOB ~ 0x4002 0400
//  GPIOD ~ 0x4002 0C00
//  GPIOE ~ 0x4002 1000
//  GPIOF ~ 0x4002 1400
//  GPIOG ~ 0x4002 1800
//  ODR ~ 0x14
//  IDR ~ 0x10

#define GPIOD_ODR_M 0x40020C14
#define GPIOG_ODR_M 0x40021814
#define GPIOE_ODR_M 0x40021014

#define GPIOD_IDR_M 0x40020C10
#define GPIOE_IDR_M 0x40021010

#define GPIOD_MODER_M 0x40020C00
#define GPIOE_MODER_M 0x40021000
#define GPIOD_PUPDR_M 0x40020C0c
#define GPIOE_PUPDR_M 0x4002100c

#define D0_Pin 14
#define D1_Pin 15
#define D2_Pin 0
#define D3_Pin 1
#define D4_Pin 7
#define D5_Pin 8
#define D6_Pin 9
#define D7_Pin 10
#define CLE_Pin 11
#define ALE_Pin 12
#define WE_Pin 5
#define RE_Pin 4
#define CE_Pin 9
#define RB_Pin 6

/*注意这个只针对0x4000 0000的区域!*/
#define __BIT_SET(WORD_ADDR, BIT_ADDR, BIT) *(uint32_t *)(0x42000000 + (((WORD_ADDR) & 0x0fffffff) << 5) + ((BIT_ADDR) << 2)) = (BIT)