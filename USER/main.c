#include "main.h"
#include "ili9341.h"
#include "led.h"
#include "sdio_sdcard.h"
#include "stm32f4xx_hal_sd.h"
#include "timer.h"
#include <stdint.h>
#include "malloc.h"

void show_sdcard_info(void)
{
	switch (SDCardInfo.CardType)
	{
		case CARD_SDSC:
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDSC V");
		printf("card type SDSC \r\n");
			break;
		case CARD_SDHC_SDXC:
			printf("card type SDXC \r\n");
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDXC V");
			break;
	}
	switch (SDCardInfo.CardVersion)
	{
		case CARD_V1_X:
			LCD_ShowString(256, 182, 48, 32, 32, "1.0");
		printf("V 1.0\r\n");
			break;
		case CARD_V2_X:
					printf("V 2.0\r\n");
			LCD_ShowString(256, 182, 48, 32, 32, "2.0");
			break;
	}

	/*LCD_ShowString(0, 220, 144, 16, 26, "manuid");*/
	/*LCD_ShowNum(48, 220, SDCardInfo.BlockSize, 8, 16);*/
	/*LCD_ShowNum(0, 236, SDCARD_Handler.SdCard.BlockNbr, 8, 16);*/
	/*LCD_ShowNum(64, 252, SDCardInfo.BlockSize, 8, 16);*/
	/*LCD_ShowNum(0, 252, SDCARD_Handler.SdCard.BlockNbr, 8, 16);*/
	printf("sd block = %d \r\n", SDCARD_Handler.SdCard.BlockNbr);
	printf("sd block size = %d \r\n", SDCARD_Handler.SdCard.BlockSize);
	uint64_t temp = ((uint64_t)SDCARD_Handler.SdCard.BlockNbr * SDCARD_Handler.SdCard.BlockSize) >> 30;
	printf("sd size = %d GB", (uint32_t)temp);
}

void sd_test_read(uint32_t secaddr, uint32_t seccnt)
{
	uint32_t i;
	uint8_t buf[seccnt * 512];
	uint8_t	sta = 0;

//	buf = mymalloc(SRAMEX, seccnt * 512);
	sta = sd_readdisk(buf, secaddr, seccnt);
	if (sta == 0)
	{
		printf("sector %d data:\r\n", secaddr);
		for (i = 0; i < 512 * seccnt; i++)
		{
			printf("%x ", buf[i]);
		}
		printf("\r\ndata ended\r\n");
	}
	else
	{
		printf("error:%d\r\n", sta);
	}

//	myfree(SRAMEX, buf);
}


int main()
{
	u8 key = 9;
	
	HAL_Init();										//初始化HAL库
	Stm32_Clock_Init(216,15,RCC_PLLP_DIV2,4);		//设置时钟180Mhz
	delay_init(180);								//初始化延时函数
	uart_init(115200);								//初始化串口
	LED_Init();										//初始化LED
//	Remote_Init();
	ili9341_init();
	TIM3_Init(999, 89);
	
	__HAL_RCC_CRC_CLK_ENABLE();

//	while(1);
	#if 1
	LCD_PWR=0;
	POINT_COLOR=RED;
	LCD_ShowString(0,0,192,32,32,"REMOTE TEST!");
	LCD_ShowString(0,108,144,32,32,"KEY VAL :");
	LCD_ShowString(0,150,144,32,32,"KEY CNT :");
	
	while (sd_init())
	{
		LCD_ShowString(0, 220, 144, 32, 32, "sd card error!");
		printf("sd card error\r\n");
		delay_ms(500);
		LCD_ShowString(0, 220, 144, 32, 32, "please check!");
		printf("please check\r\n");
		delay_ms(500);
	}
	printf("sd card info \r\n");
	show_sdcard_info();
	sd_test_read(512, 1);
	LCD_ShowString(0, 268, 144, 16, 16,"sdcard size :    MB");
	LCD_ShowNum(104, 268, (SDCARD_Handler.SdCard.BlockSize * SDCARD_Handler.SdCard.BlockNbr) >> 20, 4, 16);
	LCD_ShowNum(104, 284, (SDCARD_Handler.SdCard.LogBlockSize * SDCARD_Handler.SdCard.LogBlockNbr) >> 20, 4, 16);
	while(1)
		{
//			key = Remote_Scan();
			if (key)
				{
					LED_Green = 0;
					LCD_ShowNum(145,108,key,3,32);
					/* LCD_ShowNum(145,150,RmtCnt,3,32); */
					LED_Green = 1;
				}
			else 
				delay_ms(1000);
			LED_White = !LED_White;
		}
		#endif

}
