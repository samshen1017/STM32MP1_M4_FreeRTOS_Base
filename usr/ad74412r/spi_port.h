#ifndef SPI_PORT_H
#define SPI_PORT_H

#include <stdint.h>
#include "no_os_spi.h"

/**
 * @brief stm32 specific SPI platform ops structure
 */
extern const struct no_os_spi_platform_ops stm32_spi_ops;

#endif // STM32_SPI_H_
