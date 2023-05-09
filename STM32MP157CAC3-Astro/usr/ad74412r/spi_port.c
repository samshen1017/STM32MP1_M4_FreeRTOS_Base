#include "spi_port.h"

#include <stdio.h>
#include <stdbool.h>
#include "phex.h"

#include "no_os_alloc.h"
#include "no_os_delay.h"
#include "errno.h"

#include "spi.h"
#include "stm32mp1xx_hal_spi.h"
#include "stm32mp1xx_hal_gpio.h"

#define SPI_DEV &hspi1

/**
 * @brief Initialize the SPI communication peripheral.
 * @param desc - The SPI descriptor.
 * @param param - The structure that contains the SPI parameters.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t stm32_spi_init(struct no_os_spi_desc **desc, const struct no_os_spi_init_param *param)
{
	struct no_os_spi_desc *spi_desc;
	if (!desc || !param)
		return -EINVAL;
	spi_desc = (struct no_os_spi_desc *)no_os_calloc(1, sizeof(*spi_desc));
	if (!spi_desc)
		return -ENOMEM;

	/* copy settings to device descriptor */
	spi_desc->device_id = param->device_id;
	spi_desc->max_speed_hz = param->max_speed_hz;
	spi_desc->mode = param->mode;
	spi_desc->bit_order = param->bit_order;
	spi_desc->chip_select = param->chip_select;

	printf("stm32_spi_init\r\n");
	*desc = spi_desc;
	return 0;
}

/**
 * @brief Free the resources allocated by no_os_spi_init().
 * @param desc - The SPI descriptor.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t stm32_spi_remove(struct no_os_spi_desc *desc)
{
	if (!desc)
	{
		return -EINVAL;
	}
	no_os_free(desc);
	printf("stm32_spi_remove\r\n");
	return 0;
}

/**
 * @brief Write/read multiple messages to/from SPI.
 * @param desc - The SPI descriptor.
 * @param msgs - The messages array.
 * @param len - Number of messages.
 * @return 0 in case of success, errno codes otherwise.
 */
int32_t stm32_spi_transfer(struct no_os_spi_desc *desc,
						   struct no_os_spi_msg *msgs,
						   uint32_t len)
{
	if (!desc || !msgs)
		return -EINVAL;

	for (uint32_t i = 0; i < len; i++)
	{
		/* Assert CS */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

		while (HAL_SPI_GetState(SPI_DEV) != HAL_SPI_STATE_READY)
			;
		HAL_SPI_TransmitReceive(SPI_DEV, msgs[i].tx_buff, msgs[i].rx_buff, msgs[i].bytes_number, 1000);

		if (msgs[i].cs_delay_last)
		{
			no_os_udelay(msgs[i].cs_delay_last);
		}

		if (msgs[i].cs_change)
		{
			/* De-assert CS */
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		}

		if (msgs[i].cs_change_delay)
		{
			no_os_udelay(msgs[i].cs_change_delay);
		}
	}
	return 0;
}

/**
 * @brief Write and read data to/from SPI.
 * @param desc - The SPI descriptor.
 * @param data - The buffer with the transmitted/received data.
 * @param bytes_number - Number of bytes to write/read.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t stm32_spi_write_and_read(struct no_os_spi_desc *desc, uint8_t *data, uint16_t bytes_number)
{
	struct no_os_spi_msg msg = {
		.bytes_number = bytes_number,
		.cs_change = true,
		.rx_buff = data,
		.tx_buff = data,
	};

	if (!desc || !data)
		return -EINVAL;

	if (!bytes_number)
		return 0;

	return stm32_spi_transfer(desc, &msg, 1);
}

/**
 * @brief stm32 platform specific SPI platform ops structure
 */
const struct no_os_spi_platform_ops stm32_spi_ops = {
	.init = &stm32_spi_init,
	.write_and_read = &stm32_spi_write_and_read,
	.remove = &stm32_spi_remove,
	.transfer = &stm32_spi_transfer,
};
