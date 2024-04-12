#include <stdarg.h>

void USART_Printf(UART_HandleTypeDef* chuankou, char* format, ...)
{
	char String[30];						//�����ַ�����
	va_list arg;							//����ɱ�����б��������͵ı���arg
	va_start(arg, format);					//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);			//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);							//��������arg
	HAL_UART_Transmit(chuankou, (uint8_t*)String, sizeof(String) / sizeof(String[0]), 200);
}