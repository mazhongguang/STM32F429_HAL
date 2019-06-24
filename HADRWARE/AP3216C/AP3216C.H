#ifndef _AP3216C_H
#define _AP3216C_H

#include "main.h"

#define AP3216C_ADDR	0X3C	//AP316C的IIC地址(左移了一位)
#define AP3216C_Cmd_Write	(AP3216C_ADDR|0x00)		//AP3216C写命令
#define AP3216C_Cmd_Read	(AP3216C_ADDR|0x01)		//AP3216C读命令

u8 AP3216C_Init(void);
u8 AP3216C_WriteOneByte(u8 reg,u8 data);
u8 AP3216C_ReadOneByte(u8 reg);
void AP3216C_ReadData(u16* ir,u16* ps,u16* als);

#endif
