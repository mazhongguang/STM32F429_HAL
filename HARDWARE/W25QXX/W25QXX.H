#ifndef _W25QXX_H
#define _W25QXX_H

#include "main.h"

/************************		W25Q系列芯片列表	************************************/
#define W25Q80		0XEF13					//8Mbit,1Mbytes
#define W25Q16		0XEF14					//16Mbit,2Mbytes
#define W25Q32		0XEF15					//32Mbit,4Mbytes
#define W25Q64		0XEF16					//64Mbit,8Mbytes
#define W25Q128		0XEF17					//128Mbit,16Mbytes
#define W25Q256		0XEF18					//256Mbit,32Mbytes

extern u16 W25QXX_TYPE;														//定义W25QXX芯片型号

#define W25QXX_CS PFout(6)												//W25QXX片选信号
/**************************		指令表		*******************************************/
#define W25XX_WriteEnable							0x06
#define W25XX_WriteDisable						0x04
#define W25XX_ReadStatusReg1						0x05
#define W25XX_ReadStatusReg2						0x35
#define W25XX_ReadStatusReg3						0x15
#define W25XX_WriteStatusReg1						0x01
#define W25XX_WriteStatusReg2						0x31
#define W25XX_WriteStatusReg3						0x11
#define W25XX_ReadData								0x03
#define W25XX_FastReadData						0x0B
#define W25XX_FastReadDual						0x3B
#define W25XX_PageProgram							0x02
#define W25XX_BlockErase							0xD8
#define W25XX_SectorErase							0x20
#define W25XX_ChipErase							0xC7
#define W25XX_PowerDown							0xB9
#define W25XX_ReleasePowerDown					0xAB
#define W25XX_DeviceID								0xAB
#define W25XX_ManufactDeviceID					0X90
#define W25XX_JedecDeviceID						0X9F
#define W25XX_Enable4ByteAddr						0XB7
#define W25XX_Exit4ByteAddr						0XE9

void W25QXX_Init(void);
u16 W25QXX_ReadID(void);																//读取FLASH ID
u8 W25QXX_ReadSR(u8 regno);														//读取状态寄存器
void W25QXX_4ByteAddr_Enable(void);											//使能4字节地址模式
void W25QXX_Write_SR(u8 regno,u8 sr);												//写状态寄存器
void W25QXX_Write_Enable(void);													//写使能
void W25QXX_Write_Disable(void);													//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);						//读取FLASH
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);					//写入FLASH
void W25QXX_Erase_Chip(void);														//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);											//扇区擦除
void W25QXX_Wait_Busy(void);														//等待空闲
void W25QXX_Power_Down(void);													//进入掉电模式
void W25QXX_WAKEUP(void);															//唤醒

#endif
