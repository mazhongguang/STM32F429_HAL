#include "main.h"
#include "ili9341.h"
#include "led.h"
#include "sdio_sdcard.h"
#include "stm32f4xx_hal_sd.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>
#include "malloc.h"
#include "ff.h"
#include "SDRAM.H"

void show_sdcard_info(void)
{
	switch (SDCARD_Handler.SdCard.CardType)
	{
		case CARD_SDSC:
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDSC V");
		printf("card type SDSC \r\n");
			break;
		case CARD_SDHC_SDXC:
			printf("card type SDHC \r\n");
			LCD_ShowString(0, 182, 144, 32, 32, "card type SDXC V");
			break;
	}
	switch (SDCARD_Handler.SdCard.CardVersion)
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
	printf("sd size = %d GB\r\n", (uint32_t)temp);
}

void sd_test_write(uint32_t secaddr, uint32_t seccnt)
{
	uint32_t i;
	uint8_t *buf;
	uint8_t sta = 0;
	buf = mymalloc(SRAMEX, seccnt * 512);
	for (i = 0; i < seccnt * 512; i++);
	{
		buf[i] = 0xFF;
	}
	sta = sd_writedisk(buf, secaddr, seccnt);
	if (sta == 0)
	{
		printf("write over!\r\n");
	}
	else
	{
		printf("err:%d\r\n", sta);
	}
	myfree(SRAMEX, buf);
}

void sd_test_read(uint32_t secaddr, uint32_t seccnt)
{
	uint32_t i;
	uint8_t *buf;
//	uint8_t buf[512] = {0};
	uint8_t	sta = 0;
	buf = mymalloc(SRAMEX, seccnt * 512);
	
	for (uint16_t j = 0; j < seccnt * 512; j++)
	{
		buf[j] = 0;
	}
	sta = sd_readdisk(buf, secaddr, seccnt);
//	sta = sd_readdisk(buf, 0, 1);
	
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

	myfree(SRAMEX, buf);
}


int main()
{
	u8 key = 9;
	FATFS fatfs;
	FIL file;
	FRESULT f_result;
	char line[100];
	UINT br, bw;
	
	HAL_Init();										//初始化HAL库
	Stm32_Clock_Init(216,15,RCC_PLLP_DIV2,4);		//设置时钟180Mhz
	delay_init(180);								//初始化延时函数
	uart_init(115200);								//初始化串口
	LED_Init();										//初始化LED
//	Remote_Init();
	ili9341_init();
	TIM3_Init(999, 89);
	SDRAM_Init();
	My_Mem_Init(SRAMIN);
	My_Mem_Init(SRAMEX);
	
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

	printf("fatfs addr = %#x \r\n", &fatfs);
//	sd_test_write(512, 1);
//		sd_test_read(512, 1);
BYTE work[FF_MAX_SS];

	f_result=f_mount(&fatfs , "0:", 0);
	printf("f_mount result = %d \r\n", f_result);

	f_showfree("0:");
	f_scan_files("0:");

	/*f_result = f_mkdir("test123");*/
	/*printf("f_mkdir result = %d \r\n", f_result);*/
	
	/* 读取并逐行显示数据 */
//	f_result = f_open(&file, ".gitignore", FA_READ);
	//	while (f_gets(line, sizeof(line), &file))
//	{
//		printf(line);
//		printf("\r\n");
//	}
	
	f_result = f_open(&file, "0:file.bin", FA_WRITE | FA_CREATE_ALWAYS);
	printf("F_OPEN result = %d \r\n", f_result);
	f_result = f_printf(&file, "123\r\n %d\r\n", 108279);
//	f_result = f_write(&file, "test111", sizeof("test111"), &bw);
	printf("f_write result = %d \r\n");

	f_result = f_close(&file);
	printf("F_close result = %d \r\n", f_result);
	f_mount(0, "0:", 0);
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
