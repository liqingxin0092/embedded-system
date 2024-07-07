#ifndef __FLASH_H
#define __FLASH_H



uint16_t  FlashRead(uint32_t add);
void Flash_write(uint32_t add, uint16_t data);
void Flash_write_OptionBytes(uint32_t add, uint16_t data);

#endif
