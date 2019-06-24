#include "main.h"
/********************************************************************************
初始化LED IO 口，并使能PB时钟，默认LED0、LED1为高，即LED0、LED1灭
********************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();				//使能GPIOB
	__HAL_RCC_GPIOD_CLK_ENABLE();				//使能GPIOB


	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;		//PB0,PB1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);		//PB0置1，默认灯灭
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);		//PB1置1，默认灯灭
}

