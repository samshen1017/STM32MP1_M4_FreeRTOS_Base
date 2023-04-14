#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"

#include "stm32mp1xx_hal.h"
#include "stm32mp1xx_hal_tim.h"
#include "tim.h"

static BaseType_t PWM_ON_cmd(char *pcWriteBuffer, size_t xWriteBufferLen,
                             const char *pcCommandString)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
    printf("TIM5 CH2 PH11 ON.\r\n");
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _on = {
    "pwm-on",
    "pwm-on\r\n",
    PWM_ON_cmd, /* The function to run. */
    -1          /* The user can enter any number of commands. */
};

static BaseType_t PWM_OFF_cmd(char *pcWriteBuffer, size_t xWriteBufferLen,
                              const char *pcCommandString)
{
    HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_2);
    printf("TIM5 CH2 PH11 OFF.\r\n");
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _off = {
    "pwm-off",
    "pwm-off\r\n",
    PWM_OFF_cmd, /* The function to run. */
    -1           /* The user can enter any number of commands. */
};

static BaseType_t PWM_Set_cmd(char *pcWriteBuffer, size_t xWriteBufferLen,
                              const char *pcCommandString)
{
    const char *pcParameter;
    BaseType_t xParameterStringLength, xReturn;
    static UBaseType_t uxParameterNumber = 0;

    /* Remove compile time warnings about unused parameters, and check the
     write buffer is not NULL.  NOTE - for simplicity, this example assumes the
     write buffer length is adequate, so does not check for buffer overflows. */
    (void)pcCommandString;
    (void)xWriteBufferLen;
    // configASSERT(pcWriteBuffer);

    if (uxParameterNumber == 0)
    {
        uxParameterNumber = 1U;
        xReturn = pdPASS;
    }
    else if (uxParameterNumber == 1)
    {
        pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, uxParameterNumber, &xParameterStringLength);
        int pct = atoi(pcParameter);

        printf("pwm : %d\r\n", pct);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pct);

        xReturn = pdFALSE;
        uxParameterNumber = 0;
        memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    }

    return xReturn;
}

static const CLI_Command_Definition_t _set = {
    "pwm-set",
    "pwm-set 0 ~ 1000\r\n",
    PWM_Set_cmd, /* The function to run. */
    -1           /* The user can enter any number of commands. */
};

void vRegisterPWMCLICommands(void)
{
    FreeRTOS_CLIRegisterCommand(&_on);
    FreeRTOS_CLIRegisterCommand(&_off);
    FreeRTOS_CLIRegisterCommand(&_set);
}
