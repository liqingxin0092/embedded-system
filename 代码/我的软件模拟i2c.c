void SCL(uint8_t pin_state)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, pin_state);
}
void SDA(uint8_t pin_state)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, pin_state);
}
void start()
{
	SCL(1);
	SDA(1);
	SDA(0);
	SCL(0);
}
void stop()
{
	SCL(0);
	SDA(0);
	SCL(1);
	SDA(1);
}
void send(uint8_t bite)
{
	for (uint8_t i=0;i<8;i++)
	{
		SCL(0);
		SDA((bite&(0x80>>i))>>(7-i));
		SCL(1);
	}
	SCL(0);
}
uint8_t receive()
{
	uint8_t receive_data=0;
	for(uint8_t i=0;i<8;i++)
	{
	SCL(0);
	SCL(1);
	receive_data|=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)<<(7-i);
	}
	SCL(0);
	return receive_data;
}

void send_ack(void)
{
	SCL(0);
	SDA(0);
	SCL(1);
	SCL(0);
}
uint8_t receive_ack(void)
{
	SDA(1);
	uint8_t receive_ack;
	SCL(0);
	SCL(1);
	receive_ack=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
	SCL(0);
	return receive_ack;
}