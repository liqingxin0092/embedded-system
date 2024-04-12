#include <stdarg.h>

void USART_Printf(UART_HandleTypeDef* chuankou, char* format, ...)
{
	char String[30];						//定义字符数组
	va_list arg;							//定义可变参数列表数据类型的变量arg
	va_start(arg, format);					//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);			//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);							//结束变量arg
	HAL_UART_Transmit(chuankou, (uint8_t*)String, sizeof(String) / sizeof(String[0]), 200);
}