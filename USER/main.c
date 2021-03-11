#include "main.h"
#include "ili9341.h"
#include "led.h"
#include "sdio_sdcard.h"
#include "stm32f4xx_hal_sd.h"
#include "timer.h"

void show_sdcard_info(void)
{
	switch (SDCardInfo.CardType)
	{
		case CARD_SDSC:
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDSC V");
			break;
		case CARD_SDHC_SDXC:
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDXC V");
			break;
	}
	switch (SDCardInfo.CardVersion)
	{
		case CARD_V1_X:
			LCD_ShowString(256, 182, 48, 32, 32, "1.0");
			break;
		case CARD_V2_X:
			LCD_ShowString(256, 182, 48, 32, 32, "2.0");
			break;
	}

	LCD_ShowString(0, 220, 144, 16, 26, "manuid");
	LCD_ShowNum(48, 220, SDCardInfo.BlockSize, 8, 16);
	LCD_ShowNum(0, 236, SDCARD_Handler.SdCard.BlockNbr, 8, 16);
	LCD_ShowNum(64, 252, SDCardInfo.BlockSize, 8, 16);
	LCD_ShowNum(0, 252, SDCARD_Handler.SdCard.BlockNbr, 8, 16);
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
		delay_ms(500);
		LCD_ShowString(0, 220, 144, 32, 32, "please check!");
		delay_ms(500);
	}
	show_sdcard_info();
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
