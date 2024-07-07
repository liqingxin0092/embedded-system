#include "stm32f10x.h" 

//读取flash
uint16_t  FlashRead(uint32_t add)
{
    return   *(__IO uint16_t *)add;
}
//写flash
void Flash_write(uint32_t add, uint16_t data)
{
    //解锁
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xcdef89ab;

    //页擦除
    FLASH->CR |= 1 << 1;      //即将擦除 
    while (FLASH->SR & 1);   //等待忙

    FLASH->AR = add;   //选择页数
    FLASH->CR |= 1 << 6;    //使能
    while (FLASH->SR & 1);   //等待忙

    FLASH->CR &= ~(1 << 1);      //结束擦除

    //页写入
    FLASH->CR |= 1;   //即将要写入数据
    while (FLASH->SR & 1);//等待忙

    *(__IO uint16_t*)(add) = data;  //写入数据
    while (FLASH->SR & 1);    //等待忙

    FLASH->CR &= ~(1 << 0);  //结束写入

    //加锁
    FLASH->CR |= 1 << 7;
}
//写选项字节
void Flash_write_OptionBytes(uint32_t add, uint16_t data)
{
    //解锁Flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xcdef89ab;

    //解锁选项字节
    FLASH->OPTKEYR = 0x45670123;
    FLASH->OPTKEYR = 0xcdef89ab;

    //页擦除
    FLASH->CR |= 1 << 5;   //即将擦除
    while (FLASH->SR & 1);   //等待忙

    FLASH->CR |= 1 << 6;    //使能
    while (FLASH->SR & 1);   //等待忙

    FLASH->CR &= ~(1 << 5);      //结束擦除

    //写入
    FLASH->CR |= 1 << 4;   //即将要写入数据
    while (FLASH->SR & 1);//等待忙

    *(__IO uint16_t*)0x1ffff800 = 0xa5;  //  避免读写保护
    while (FLASH->SR & 1);//等待忙

    *(__IO uint16_t*)add = data;  //写入用户数据
    while (FLASH->SR & 1);//等待忙

    *(__IO uint16_t*)(add + 2) = data + 2;  //写入用户数据
    while (FLASH->SR & 1);//等待忙

    FLASH->CR &= ~(1 << 4);  //结束写入

    //加锁
    FLASH->CR |= 1 << 7;
}
