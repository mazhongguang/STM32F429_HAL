#include "main.h"
int main()
{
	HAL_Init();										//初始化HAL库
	Stm32_Clock_Init(216,15,RCC_PLLP_DIV2,4);		//设置时钟180Mhz
	delay_init(180);								//初始化延时函数
	uart_init(115200);								//初始化串口
	LED_Init();										//初始化LED
	LCD_Init();
	LCD_PWR=0;
	POINT_COLOR=RED;
	LCD_ShowString(0,0,192,32,32,"REMOTE TEST!");
	while(1)
		{
			
		}
}
