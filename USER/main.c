#include "GUI.h"
#include "WM.h"
#include "main.h"
#include "guidemo.h"
#include "test.h"
#include "timer.h"

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
	WM_SetCreateFlags(WM_CF_MEMDEV);
	
	GUI_Init();
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringAt("HELLO WORLD ! \r\n", 30, 50);
	GUI_SetBkColor(GUI_GREEN);
	GUI_ClearRect(50, 100, 100, 150);
	GUI_DrawGradientH(120, 100, 300, 220,GUI_WHITE, GUI_RED);

	WM_MULTIBUF_Enable(1);
//	MainTask(); 
	GUIDEMO_Main();
	while(1);
	#if 0
	LCD_PWR=0;
	POINT_COLOR=RED;
	LCD_ShowString(0,0,192,32,32,"REMOTE TEST!");
	LCD_ShowString(0,108,144,32,32,"KEY VAL :");
	LCD_ShowString(0,150,144,32,32,"KEY CNT :");
	
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
