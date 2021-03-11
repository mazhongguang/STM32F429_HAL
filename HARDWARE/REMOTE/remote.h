#ifndef __REMOTE_H
#define __REMOTE_H

#include "main.h"

#define RDATA PAin(8)	//红外数据输入脚

/***************************************************************
红外遥控识别码（ID），每款红外遥控识别码都不一样，但也有相同的
本遥控识别码为 0X00FF
***************************************************************/
#define REMOTE_ID 0x00

extern u8 RmtCnt;		//按键按下的次数

void Remote_Init(void);					//红外传感器接收头引脚初始化
u8 Remote_Scan(void);

#endif
