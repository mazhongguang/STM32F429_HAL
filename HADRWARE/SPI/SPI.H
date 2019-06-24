#ifndef _SPI_H
#define _SPI_H

#include "main.h"

extern SPI_HandleTypeDef SPI5_Handler;				//SPI句柄
extern SPI_HandleTypeDef SPI2_Handler;				//SPI句柄


void SPI5_Init(void);
void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI5_ReadWriteByte(u8 TxData);

void SPI2_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);

#endif
