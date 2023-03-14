#include <stdio.h>
#include "usart.h"

int fputc(int ch, FILE *f)
{
	char c = ch;
	HAL_UART_Transmit(&huart3, (uint8_t *)&c, 1, 0x0FFF);
	return c;
}
