#include "main.h"
/***************************外部中断初始化************************************************/
void EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();		//开启 GPIOA  时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();		//开启 GPIOC  时钟
	__HAL_RCC_GPIOH_CLK_ENABLE();		//开启 GPIOH  时钟

	GPIO_Initure.Pin=GPIO_PIN_0;			//PA0
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING;	//上升沿触发
	GPIO_Initure.Pull=GPIO_PULLDOWN;		//下拉
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_13;			//PC13
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;	//下降沿触发
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;		//PH2,PH3
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);

	/**************  中断线0 *******************************/
	HAL_NVIC_SetPriority(EXTI0_IRQn,4,0);			//抢占优先级2，子优先级0
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);					//使能中断线0

	/**************  中断线2 *****************************/
	HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);			//抢占优先级2，子优先级1
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);					//使能中断线2

	/**************  中断线3 *******************************/
	HAL_NVIC_SetPriority(EXTI3_IRQn,5,2);			//抢占优先级2，子优先级2
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);					//使能中断线3

	/**************  中断线13 *****************************/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);			//抢占优先级2，子优先级3
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);					//使能中断线13

}
/****************************** 中断服务函数 ******************************/
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);			//调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);			//调用中断处理公用函数
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);			//调用中断处理公用函数
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);			//调用中断处理公用函数
}
/*********************************************************************
中断服务程序中需要做的事情
在HAL库中所有的外部中断服务函数都会调用此函数
GPIO_PIN:中断引脚序号
********************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_ms(100);									//消抖
	switch(GPIO_Pin)
		{
			case GPIO_PIN_0:
				if(WK_UP==1)
					{
						LED_White=!LED_White;
						LED_Green=!LED_White;
					}
				break;
			case GPIO_PIN_2:
				if(KEY1==0)
					{
						LED_Green=!LED_Green;
					}
				break;
 			case GPIO_PIN_3:
				if(KEY0==0)
					{
						LED_Green=!LED_Green;
						LED_White=!LED_White;
					}
				break;
			case GPIO_PIN_13:
				if(KEY2==0)
					{
						LED_White=!LED_White;
					}
				break;
		}
}
