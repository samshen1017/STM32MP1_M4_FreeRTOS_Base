#include "serial.h"

#include "usart.h"
#include "stm32mp1xx_hal_uart.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <stdio.h>
#include <assert.h>

typedef struct
{
    SemaphoreHandle_t lock;
    void *handle;
} console;

static console con;

xComPortHandle xSerialPortInitMinimal(unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength)
{
    con.handle = &huart3;
    con.lock = xSemaphoreCreateCounting(1, 1);
    return &con;
}

void vSerialPutString(xComPortHandle pxPort, const signed char *const pcString, unsigned short usStringLength)
{
    assert(pxPort != NULL);
    console *con = (console *)pxPort;
    xSemaphoreTake(con->lock, 0xFFFF);

    HAL_UART_Transmit((UART_HandleTypeDef *)con->handle, (uint8_t *)pcString, usStringLength, 0x0FFF);
    xSemaphoreGive(con->lock);
}

signed portBASE_TYPE xSerialPutChar(xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime)
{
    assert(pxPort != NULL);
    HAL_StatusTypeDef status;
    console *con = (console *)pxPort;
    xSemaphoreTake(con->lock, 0xFFFF);

    status = HAL_UART_Transmit((UART_HandleTypeDef *)con->handle, (uint8_t *)&cOutChar, 1, xBlockTime);
    xSemaphoreGive(con->lock);

    if (status != HAL_OK)
    {
        return -1;
    }
    return 1;
}

signed portBASE_TYPE xSerialGetChar(xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime)
{
    assert(pxPort != NULL);
    HAL_StatusTypeDef status;
    console *con = (console *)pxPort;
    xSemaphoreTake(con->lock, 0xFFFF);

    status = HAL_UART_Receive((UART_HandleTypeDef *)con->handle, (uint8_t *)pcRxedChar, 1, xBlockTime);
    xSemaphoreGive(con->lock);

    if (status != HAL_OK)
    {
        return -1;
    }
    return 1;
}

void vSerialClose(xComPortHandle xPort)
{
}
