#ifndef _TIMER_H
#define _TIMER_H
#include "main.h"
extern TIM_HandleTypeDef TIM3_Handler;					//定时器句柄
extern TIM_OC_InitTypeDef TIM3_CH2Handler;				//定时器3通道2句柄
extern TIM_HandleTypeDef TIM2_CH1Handler;				//定时器2句柄

void TIM3_Init(u16 arr,u16 psc);				//定时器初始化
void TIM3_PWM_Init(u16 arr,u16 psc);			//PWM初始化
void TIM_Set_TIM3Compare2(u32 compare);			//PWM占空比设置
u32 TIM_GET_TIM2_Compare1(void);					//
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);			//输入捕获初始化

#endif

