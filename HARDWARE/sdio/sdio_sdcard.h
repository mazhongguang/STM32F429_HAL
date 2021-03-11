#ifndef __SDIO_SDCARD_H
#define __SDIO_SDCARD_H 

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_sd.h"

#define SD_TIMEOUT ((uint32_t)(180 * 5000))		// 180 ticks = 1ms , 5S = 5000ms = 5000 * 180

#define SD_DMA_MODE 0

extern SD_HandleTypeDef SDCARD_Handler;
extern HAL_SD_CardInfoTypeDef SDCardInfo;

uint8_t sd_init(void);
uint8_t sd_getcardinfo(HAL_SD_CardInfoTypeDef *cardinfo);
uint8_t sd_readdisk(uint8_t *buf, uint32_t sector, uint32_t cnt);
uint8_t sd_writedisk(uint8_t *buf, uint32_t sector, uint32_t cnt);

#endif
