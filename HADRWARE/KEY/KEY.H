#ifndef _KEY_H
#define _KEY_H

#include "main.h"

#define KEY0 PHin(3)		//KEY0  PH3
#define KEY1 PHin(2)		//KEY1  PH2
#define KEY2 PCin(13) 		//KEY2  PC13
#define WK_UP PAin(0)		//WK_UP PA0

#define KEY0_PRES	1		
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES	4

void KEY_Init(void);		//按键初始化
u8 KEY_Scan(u8 mode);	//按键扫描

#endif

