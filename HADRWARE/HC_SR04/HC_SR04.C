#include "main.h"

void HC_SR_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_Initure.Pin=GPIO_PIN_4;				//PA4
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;		//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	HC_SR=0;									//默认超声波控制脚为低电平
}
