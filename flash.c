uint16_t  FlashRead(uint32_t add)
{
    return   *(__IO uint16_t *)add;
}
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