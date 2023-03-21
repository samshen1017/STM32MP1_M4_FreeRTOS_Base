#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "cmsis_os.h"

#include "ad74413r.h"
#include "spi_port.h"
#include "no_os_print_log.h"
#include "no_os_delay.h"

#include "spi.h"
#include "stm32mp1xx_hal_spi.h"
#include "stm32mp1xx_hal_gpio.h"

#define ad74413r_spi_ip                             \
    {                                               \
        .device_id = 1,                             \
        .max_speed_hz = 512000,                     \
        .bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST, \
        .mode = NO_OS_SPI_MODE_1,                   \
        .platform_ops = &stm32_spi_ops,             \
        .chip_select = 1,                           \
        .extra = NULL,                              \
    }
struct ad74413r_init_param ad74413r_ip = {
    .chip_id = AD74412R,
    .comm_param = ad74413r_spi_ip,
};

struct ad74413r_desc *ad74413r_desc = NULL;

static BaseType_t AD74412_test_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int ret;
    uint8_t val;
    uint32_t dac_code;
    uint16_t temp;
    union ad74413r_live_status status;
    struct ad74413r_decimal result[3];

    /* Assert RESET */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
    osDelay(configTICK_RATE_HZ / 1000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    osDelay(configTICK_RATE_HZ / 1000);

    ret = ad74413r_init(&ad74413r_desc, &ad74413r_ip);
    if (ret)
        goto error;

    ret = ad74413r_clear_errors(ad74413r_desc);
    if (ret)
        goto error;

    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_A, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_A, AD74413R_DIGITAL_INPUT);
    if (ret)
        goto error;

    /** Set the threshold voltage to 5V */
    ret = ad74413r_set_threshold(ad74413r_desc, AD74413R_CH_A, 5000);
    if (ret)
        goto error;

    /** The comparator output will be available on the GPO_A pin */
    ret = ad74413r_set_gpo_config(ad74413r_desc, AD74413R_CH_A, AD74413R_GPO_CONFIG_COMP);
    if (ret)
        goto error;

    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_B, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_B, AD74413R_VOLTAGE_IN);
    if (ret)
        goto error;

    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_C, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_C, AD74413R_VOLTAGE_OUT);
    if (ret)
        goto error;

    ad74413r_dac_voltage_to_code(5000, &dac_code);
    /** Set the DAC output on channel C to 5V */
    ret = ad74413r_set_channel_dac_code(ad74413r_desc, AD74413R_CH_C, dac_code);
    if (ret)
        goto error;

    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_D, true);
    if (ret)
        return ret;

    /** Measure input current on channel D */
    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_D, AD74413R_CURRENT_IN_EXT);
    if (ret)
        return ret;

    ret = ad74413r_set_diag_channel_enable(ad74413r_desc, AD74413R_CH_A, true);
    if (ret)
        goto error;

    ret = ad74413r_set_diag(ad74413r_desc, AD74413R_CH_A, AD74413R_DIAG_TEMP);
    if (ret)
        goto error;

    ret = ad74413r_gpo_get(ad74413r_desc, AD74413R_CH_A, &val);
    if (ret)
        goto error;

    for (uint32_t i = 0; i < 3; i++)
    {
        ret = ad74413r_adc_get_value(ad74413r_desc, AD74413R_CH_B + i, &result[i]);
        if (ret)
            goto error;
    }

    ret = ad74413r_get_live(ad74413r_desc, &status);
    if (ret)
        goto error;

    ret = ad74413r_get_temp(ad74413r_desc, AD74413R_CH_A, &temp);
    if (ret)
        goto error;

    pr_info("Channel A: GPO_A status %d (5V threshold)\r\n", val);
    pr_info("Channel B: %ld.%02lu mV (Voltage input)\r\n", (int32_t)result[0].integer, result[0].decimal);
    pr_info("Channel C: %ld.%02lu mA (Voltage output)\r\n", (int32_t)result[1].integer, result[1].decimal);
    pr_info("Channel D: %ld.%02lu mA (Current input)\r\n", (int32_t)result[2].integer, result[2].decimal);
    pr_info("\r\nDiagnostic channel A: %d deg. C (die's temperature)\r\n", temp);

    pr_info("\r\n================================================================================\r\n\r\n");
    pr_info("Channel A fault: %d\r\n", status.status_bits.VI_ERR_A);
    pr_info("Channel B fault: %d\r\n", status.status_bits.VI_ERR_B);
    pr_info("Channel C fault: %d\r\n", status.status_bits.VI_ERR_C);
    pr_info("Channel D fault: %d\r\n", status.status_bits.VI_ERR_D);

    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;

error:
    printf("err: %d\r\n", ret);
    ad74413r_remove(ad74413r_desc);
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _test = {
    "ad-test",
    "ad-test\r\n",
    AD74412_test_cmd, /* The function to run. */
    -1                /* The user can enter any number of commands. */
};

static BaseType_t AD74412_init_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int ret;
    union ad74413r_live_status status;

    /* Assert RESET */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
    osDelay(configTICK_RATE_HZ / 1000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    osDelay(configTICK_RATE_HZ / 1000);

    ret = ad74413r_init(&ad74413r_desc, &ad74413r_ip);
    if (ret)
        goto error;

    ret = ad74413r_clear_errors(ad74413r_desc);
    if (ret)
        goto error;

    /* Channel A Config */
    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_A, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_A, AD74413R_VOLTAGE_IN);
    if (ret)
        goto error;

    /* Channel B Config */
    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_B, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_B, AD74413R_VOLTAGE_IN);
    if (ret)
        goto error;

    /* Channel C Config */
    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_C, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_C, AD74413R_VOLTAGE_IN);
    if (ret)
        goto error;

    /* Channel D Config */
    ret = ad74413r_set_adc_channel_enable(ad74413r_desc, AD74413R_CH_D, true);
    if (ret)
        goto error;

    ret = ad74413r_set_channel_function(ad74413r_desc, AD74413R_CH_D, AD74413R_VOLTAGE_IN);
    if (ret)
        goto error;

    ret = ad74413r_get_live(ad74413r_desc, &status);
    if (ret)
        goto error;

    pr_info("\r\n================================================================================\r\n\r\n");
    pr_info("Channel A fault: %d\r\n", status.status_bits.VI_ERR_A);
    pr_info("Channel B fault: %d\r\n", status.status_bits.VI_ERR_B);
    pr_info("Channel C fault: %d\r\n", status.status_bits.VI_ERR_C);
    pr_info("Channel D fault: %d\r\n", status.status_bits.VI_ERR_D);

    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;

error:
    printf("err: %d\r\n", ret);
    ad74413r_remove(ad74413r_desc);
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _init = {
    "ad-init",
    "ad-init\r\n",
    AD74412_init_cmd, /* The function to run. */
    -1                /* The user can enter any number of commands. */
};

static BaseType_t AD74412_read_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int ret;
    struct ad74413r_decimal result[4];
    union ad74413r_live_status status;

    for (uint32_t i = 0; i < 4; i++)
    {
        ret = ad74413r_adc_get_value(ad74413r_desc, AD74413R_CH_A + i, &result[i]);
        if (ret)
            goto error;
    }

    ret = ad74413r_get_live(ad74413r_desc, &status);
    if (ret)
        goto error;

    pr_info("Channel A: %d.%02u mV (Voltage input)\r\n", (int32_t)result[0].integer, result[0].decimal);
    pr_info("Channel B: %d.%02u mV (Voltage input)\r\n", (int32_t)result[1].integer, result[1].decimal);
    pr_info("Channel C: %d.%02u mV (Voltage input)\r\n", (int32_t)result[2].integer, result[2].decimal);
    pr_info("Channel D: %d.%02u mV (Voltage input)\r\n", (int32_t)result[3].integer, result[3].decimal);

    pr_info("\r\n================================================================================\r\n\r\n");
    pr_info("Channel A fault: %d\r\n", status.status_bits.VI_ERR_A);
    pr_info("Channel B fault: %d\r\n", status.status_bits.VI_ERR_B);
    pr_info("Channel C fault: %d\r\n", status.status_bits.VI_ERR_C);
    pr_info("Channel D fault: %d\r\n", status.status_bits.VI_ERR_D);
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;

error:
    printf("err: %d\r\n", ret);
    ad74413r_remove(ad74413r_desc);
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t _read = {
    "ad-read",
    "ad-read\r\n",
    AD74412_read_cmd, /* The function to run. */
    -1                /* The user can enter any number of commands. */
};

void vRegisterAD74412CLICommands(void)
{
    FreeRTOS_CLIRegisterCommand(&_test);
    FreeRTOS_CLIRegisterCommand(&_init);
    FreeRTOS_CLIRegisterCommand(&_read);
}
