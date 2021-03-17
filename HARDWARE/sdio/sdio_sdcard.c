#include <stdint.h>
#include <string.h>
#include "sdio_sdcard.h"
#include "stm32_hal_legacy.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_sd.h"
#include "stm32f4xx_ll_fmc.h"
#include "stm32f4xx_ll_sdmmc.h"
#include "sys.h"

SD_HandleTypeDef SDCARD_Handler;
HAL_SD_CardInfoTypeDef SDCardInfo; 
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler; 
/* SD_ReadDisk / SD_WriteDisk 函数专用buffer,当这两个函数的数据缓冲区地址不是4字节对齐的时候，
 * 需要使用到该数组，以确保数据缓冲区地址是4字节对齐的。
 */
__align(4) 
	uint8_t sdio_data_buffer[512];

/* @brief SDcard init
 * @param none
 * @reval 0 -> successful
 *			other -> failed
 */
uint8_t sd_init(void)
{
	uint8_t sd_error;

	/* 初始化时的时钟不能大于400KHz */
	SDCARD_Handler.Instance = SDIO;
	SDCARD_Handler.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	SDCARD_Handler.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	SDCARD_Handler.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	SDCARD_Handler.Init.ClockDiv = SDIO_INIT_CLK_DIV;
	SDCARD_Handler.Init.BusWide = SDIO_BUS_WIDE_1B;
	SDCARD_Handler.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	sd_error = HAL_SD_Init(&SDCARD_Handler);
	if (sd_error !=	HAL_OK)
	{
		return 1;
	}

	if (HAL_SD_ConfigWideBusOperation(&SDCARD_Handler, SDIO_BUS_WIDE_1B) != HAL_OK)
	{
		return 2;
	}

	return 0;
}

/* @brief SD MSP initialization
 * @param hsd SD handle pointer
 * @reval none
 */
void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{
	GPIO_InitTypeDef GPIO_Initure = {0};

	if (hsd->Instance == SDIO)
	{
		__HAL_RCC_SDIO_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_Initure.Alternate = GPIO_AF12_SDIO;
		HAL_GPIO_Init(GPIOC, &GPIO_Initure);

		GPIO_Initure.Pin = GPIO_PIN_2;
		HAL_GPIO_Init(GPIOD, &GPIO_Initure);
	}
}

/* @brief get sdcard infomation
 * @param cardinfo 
 * @reval erorr status
 */
uint8_t sd_getcardinfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
	return HAL_SD_GetCardInfo(&SDCARD_Handler, cardinfo);
}

/* @brief read sd card
 * @param buf read data buffer
 * @param sector sector address
 * @param cnt sector number
 * @reval error status
 *			0 -> OK
 *			other -> error code
 */
uint8_t sd_readdisk(uint8_t *buf, uint32_t sector, uint32_t cnt)
{
	uint8_t sta = HAL_OK;
	uint8_t n;
	uint32_t timeout = SD_TIMEOUT;
	uint64_t lsector = sector;

	INTX_DISABLE();
	if ((uint32_t)buf % 4 != 0)
	{
		for (n = 0; n < cnt; n++)
		{
			sta = HAL_SD_ReadBlocks(&SDCARD_Handler, sdio_data_buffer, lsector + (512 * n), 1, timeout); 
			memcpy(buf, sdio_data_buffer, 512);
			buf += 512;
		}
	}
	else
	{
		sta = HAL_SD_ReadBlocks(&SDCARD_Handler, buf, lsector, cnt, timeout);
	}

	while (HAL_SD_GetCardState(&SDCARD_Handler) != HAL_SD_CARD_TRANSFER)
	{
		if (timeout-- == 0)
		{
			sta = HAL_SD_STATE_BUSY;
		}
	}
	INTX_ENABLE();
	
	return sta;
}

/* @brief write SDcard
 * @param buf buffer of to write data
 * @param sector address of sector
 * @param cnt numbers of sector
 * @reval error status
 *				0 -> OK
 *				other -> error code
 */
uint8_t sd_writedisk(uint8_t *buf, uint32_t sector, uint32_t cnt)
{
	uint8_t sta = HAL_OK;
	uint8_t n;
	uint32_t timeout = SD_TIMEOUT;
	uint64_t lsector = sector;

	INTX_DISABLE();
	if ((uint32_t)buf % 4 != 0)
	{
		for (n = 0; n < cnt; n++)
		{
			memcpy(sdio_data_buffer, buf, 512);
			sta = HAL_SD_WriteBlocks(&SDCARD_Handler, sdio_data_buffer, lsector + (512 * n), 1, timeout);
			buf += 512;
		}
	}
	else
	{
		sta = HAL_SD_WriteBlocks(&SDCARD_Handler, buf, lsector, cnt, timeout);
	}

	while (HAL_SD_GetCardState(&SDCARD_Handler) != HAL_SD_CARD_TRANSFER)
	{
		if (timeout-- == 0)
		{
			sta = HAL_SD_STATE_BUSY;
		}
	}
	INTX_DISABLE();

	return sta;
}
