#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "cmsis_os.h"

#include "fdcan.h"
#include "stm32mp1xx_hal.h"
#include "stm32mp1xx_hal_fdcan.h"

#include "main.h"
#include "phex.h"

// static FDCAN_RxHeaderTypeDef RxHeader;
static uint8_t RxData[8] = {0};
static uint8_t TxData[8] = {'L', 'a', 'o', 'L', 'i', '1', '2', '3'};

static void FDCAN1_Config(void)
{
    FDCAN_FilterTypeDef sFilterConfig;

    sFilterConfig.IdType = FDCAN_STANDARD_ID;      //  配置为过滤标准帧
    sFilterConfig.FilterIndex = 0;                 //  过滤器的索引号
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE; //  过滤方式为范围，即从FilterID1~FilterID2之间的值
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x0000;
    sFilterConfig.FilterID2 = 0x07ff; //  标准帧为11位ID，即0x7ff，本例配置为接收所有帧

    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
    {
        Error_Handler();
    }
}

void FDCAN2_Config(void)
{
    FDCAN_FilterTypeDef sFilterConfig;

    sFilterConfig.IdType = FDCAN_STANDARD_ID;      //  配置为过滤标准帧
    sFilterConfig.FilterIndex = 0;                 //  过滤器的索引号
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE; //  过滤方式为范围，即从FilterID1~FilterID2之间的值
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
    sFilterConfig.FilterID1 = 0x0000;
    sFilterConfig.FilterID2 = 0x07ff; //  标准帧为11位ID，即0x7ff，本例配置为接收所有帧

    if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan2, FDCAN_RX_FIFO1, FDCAN_RX_FIFO_OVERWRITE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
        Error_Handler();
}

static BaseType_t Can_Conf_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    FDCAN1_Config();
    printf("CAN1 Conf.\r\n");
    FDCAN2_Config();
    printf("CAN2 Conf.\r\n");
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _conf = {
    "can-conf",
    "can-conf\r\n",
    Can_Conf_cmd, /* The function to run. */
    -1            /* The user can enter any number of commands. */
};

void FDCAN1_Send(void)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = 0x123;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0x00;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK)
        Error_Handler();
}

void FDCAN2_Send(void)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = 0x123;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0x00;

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, TxData) != HAL_OK)
        Error_Handler();
}

static BaseType_t Can_Send_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    // FDCAN1_Send();
    // printf("can1 send.\r\n");

    FDCAN2_Send();
    printf("can2 send.\r\n");
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _send = {
    "can-send",
    "can-send\r\n",
    Can_Send_cmd, /* The function to run. */
    -1            /* The user can enter any number of commands. */
};

void FDCAN_Recv(void)
{
    char *str = pHex((const char *)RxData, 8);
    printf("can2 recv: %s\r\n", str);
}

static BaseType_t Can_Recv_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    FDCAN_Recv();
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _recv = {
    "can-recv",
    "can-recv\r\n",
    Can_Recv_cmd, /* The function to run. */
    -1            /* The user can enter any number of commands. */
};

static osThreadId testTaskHandle;
static void _run(void const *param)
{
    for (;;)
    {
        FDCAN1_Send();
        printf("can1 send.\r\n");

        FDCAN2_Send();
        printf("can2 send.\r\n");

        osDelay(configTICK_RATE_HZ);
    }
}

static BaseType_t Can_test_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    osThreadDef(CanTest, _run, osPriorityNormal, 0, 512);
    testTaskHandle = osThreadCreate(osThread(CanTest), NULL);

    if (testTaskHandle == NULL)
    {
        printf("CanTest create failed.\r\n");
    }
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _test = {
    "can-test",
    "can-test\r\n",
    Can_test_cmd, /* The function to run. */
    -1            /* The user can enter any number of commands. */
};

void vRegisterCanCLICommands(void)
{
    FreeRTOS_CLIRegisterCommand(&_conf);
    FreeRTOS_CLIRegisterCommand(&_send);
    FreeRTOS_CLIRegisterCommand(&_recv);
    FreeRTOS_CLIRegisterCommand(&_test);
}

// void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
// {
//     if (hfdcan == &hfdcan1)
//     {
//         HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData);
//     }

//     if (hfdcan == &hfdcan2)
//     {
//         HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &RxHeader, RxData);
//     }
// }

// void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
// {
//     if (hfdcan == &hfdcan1)
//     {
//         HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO1, &RxHeader, RxData);
//     }

//     if (hfdcan == &hfdcan2)
//     {
//         HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO1, &RxHeader, RxData);
//     }
// }
