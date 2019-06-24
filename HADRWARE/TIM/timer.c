#include "main.h"
TIM_HandleTypeDef TIM3_Handler;			//定时器句柄
TIM_OC_InitTypeDef TIM3_CH2Handler;		//定时器3通道2句柄
TIM_HandleTypeDef TIM2_CH1Handler;		//定时器2句柄

/**************************************************************************************
通用定时器3中断初始化
arr:自动重装值
psc:时钟预分频数
定时器溢出时间计算方法：Tout=((arr+1)*(psc+1))/Ft  us
Ft=定时器工作频率，单位：Mhz
定时器3挂在APB1上，时钟为HCLK/2
**************************************************************************************/
void TIM3_Init(u16 arr, u16 psc)
{
	TIM3_Handler.Instance=TIM3;										//通用定时器3
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;			//时钟分频因子
	TIM3_Handler.Init.Period=arr;									//自动装载值
	TIM3_Handler.Init.Prescaler=psc;								//分频系数
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;				//向上计数器
	HAL_TIM_Base_Init(&TIM3_Handler);

	HAL_TIM_Base_Start_IT(&TIM3_Handler);							//使能定时器3和定时器3更新中断:TIM_IT_UPDAET
}
/*************************************************************************************
定时器底层驱动，开启时钟，设置中断优先级
此函数会被HAL_TIM_BASE_Init()函数调用
*************************************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef * htim)
{
	if(htim->Instance==TIM3)
		{
			__HAL_RCC_TIM3_CLK_ENABLE();						//使能TIM3时钟
			HAL_NVIC_SetPriority(TIM3_IRQn,1,3);				//设置中断优先级，抢占优先级1，子优先级3
			HAL_NVIC_EnableIRQ(TIM3_IRQn);					//开启TIM3中断
		}
}
/********************** 定时器3中断服务函数       ****************************************/
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handler);
}
/**************** 回调函数，定时器中断服务函数调用 *********************************/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
//{
//	if(htim==(&TIM3_Handler))
//		{
//			LED_White=!LED_White;        //LED1反转
//		}
//}


/***************************************************************************************************************************/


/**************************************************************************************
TIM3 PWM输出初始化
arr:自动重装值
psc:时钟预分频数
**************************************************************************************/
void TIM3_PWM_Init(u16 arr, u16 psc)
{
	TIM3_Handler.Instance=TIM3;									//定时器3
	TIM3_Handler.Init.Prescaler=psc;							//预分频数
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;			//向上计数模式
	TIM3_Handler.Init.Period=arr;								//自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;	
	HAL_TIM_PWM_Init(&TIM3_Handler);							//初始化PWM

	TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1;						//模式选择PWM1
	TIM3_CH2Handler.Pulse=arr/2;								//设置比较值，此值用来确定占空比。默认比较值为自动重装载值的一半，即占空比为50%
	TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;				//输出比较极性为低
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);				//配置TIM3通道2

	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);					//开启PWM通道2
}
/*****************************************************************************************************

定时器底层驱动，时钟使能，引脚配置
此函数会被HAL_TIM_PWM_Init()调用
htim：定时器句柄
*****************************************************************************************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef * htim)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_TIM3_CLK_ENABLE();				//使能定时器3
	__HAL_RCC_GPIOB_CLK_ENABLE();				//开启GPIOB的时钟

	GPIO_Initure.Pin=GPIO_PIN_5;				//PB5
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;			//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;		//PB5复用为TIM3_CH2
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);			
}
/******************************************************************************************************
设置TIM3通道2的占空比
compare:比较值
******************************************************************************************************/
void TIM_Set_TIM3Compare2(u32 compare)
{
	TIM3->CCR2=compare;
}


/**************************************************************************************************************************/


/****************************************************************************************
定时器2通道1输入捕获配置
arr:自动重装载值（TIM2和TIM5是32位的）
psc:时钟预分频数
****************************************************************************************/
void TIM2_CH1_Cap_Init(u32 arr, u16 psc)
{
	TIM_IC_InitTypeDef	TIM2_CH1_Config;

	TIM2_CH1Handler.Instance=TIM2;									//通用定时器2
	TIM2_CH1Handler.Init.Prescaler=psc;								//分频系数
	TIM2_CH1Handler.Init.CounterMode=TIM_COUNTERMODE_UP;			//向上计数
	TIM2_CH1Handler.Init.Period=arr;								//自动重装载值
	TIM2_CH1Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
	HAL_TIM_IC_Init(&TIM2_CH1Handler);								//初始化输入捕获参数

	TIM2_CH1_Config.ICPolarity=TIM_ICPOLARITY_RISING;				//上升沿捕获
	TIM2_CH1_Config.ICSelection=TIM_ICSELECTION_DIRECTTI;			//映射到TI1上
	TIM2_CH1_Config.ICPrescaler=TIM_ICPSC_DIV1;						//不分频
	TIM2_CH1_Config.ICFilter=0;										//配置输入滤波器，不滤波
	HAL_TIM_IC_ConfigChannel(&TIM2_CH1Handler,&TIM2_CH1_Config,TIM_CHANNEL_1);	//配置定时器2通道1

	HAL_TIM_IC_Start_IT(&TIM2_CH1Handler,TIM_CHANNEL_1);			//开启TIM2的捕获通道1，并且开启捕获中断
	__HAL_TIM_ENABLE_IT(&TIM2_CH1Handler,TIM_IT_UPDATE);			//使能更新中断
}
/***************************************************************************************
定时器2底层驱动，时钟使能，引脚配置
此函数会被HAL_TIM_IC_Init()调用
htim:TIM2句柄
***************************************************************************************/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef * htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM2_CLK_ENABLE();					//使能TIM2
	__HAL_RCC_GPIOA_CLK_ENABLE();					//使能GPIOA

	GPIO_Initure.Pin=GPIO_PIN_5;					//PA5
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;				//复用推挽
	GPIO_Initure.Pull=GPIO_PULLDOWN;				//下拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		//高速
	GPIO_Initure.Alternate=GPIO_AF1_TIM2;			//PA5复用为TIM2通道1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);				//初始化

	HAL_NVIC_SetPriority(TIM2_IRQn,2,0);			//设置中断优先级，抢占优先级2，子优先级0
	HAL_NVIC_EnableIRQ(TIM2_IRQn);					//开启TIM2中断通道
}
/*********************************************************************************************
捕获状态
【7】		：0，没有成功的捕获。1，成功捕获到1 次
【6】		：0，还没捕获到高电平。1，已经捕获到高电平了
【5：0】：捕获低电平后溢出的次数。（对于32位定时器来说，1US计数器加1，溢出时间为4294秒）
*********************************************************************************************/
u8 TIM2_CH1_CAPTURE_STA=0;					//输入捕获状态
u32 TIM2_CH1_CAPTURE_VAL;					//输入捕获值

/************ 定时器2中断服务函数 ************************************/
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2_CH1Handler);			//定时器共用处理函数
}
/**************************************************************************************************
定时器更新中断（计数溢出）
中断处理回调函数，该函数在HAL_TIM_IRQHandler()中会被调用
更新中断（溢出）发生时执行
**************************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if((TIM2_CH1_CAPTURE_STA&0x80)==0)						//还未成功捕获
		{
			if(TIM2_CH1_CAPTURE_STA&0X40)					//已经捕获到高电平了
				{
					if((TIM2_CH1_CAPTURE_STA&0x3F)==0X3F)		// 高电平时间太长了
						{
							TIM2_CH1_CAPTURE_STA|=0X80;			//标记成功捕获了一次
							TIM2_CH1_CAPTURE_VAL=0XFFFFFFFF;	
						}
					else
						TIM2_CH1_CAPTURE_STA++;
				}
		}
}
/************************************************************************************************
定时器输入捕获中断处理回调函数
该函数在HAL_TIM_IRQHandler()中会被调用
捕获中断发生时执行
************************************************************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim)
{
	if((TIM2_CH1_CAPTURE_STA&0x80)==0)						//还未成功捕获
		{
			if(TIM2_CH1_CAPTURE_STA&0x40)					//捕获到一个下降沿
				{
					TIM2_CH1_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平的脉宽
					TIM2_CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_CH1Handler,TIM_CHANNEL_1);			//获取当前的捕获值
					TIM_RESET_CAPTUREPOLARITY(&TIM2_CH1Handler,TIM_CHANNEL_1);								//一定要先清除原来的设置
					TIM_SET_CAPTUREPOLARITY(&TIM2_CH1Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);			//配置TIM2通道1上升沿捕获
				}
			else											//还未开始捕获，第一次捕获上升沿
				{
					TIM2_CH1_CAPTURE_STA=0;						//清空
					TIM2_CH1_CAPTURE_VAL=0;
					TIM2_CH1_CAPTURE_STA|=0X40;					//标记捕获到了上升沿
					
					__HAL_TIM_DISABLE(&TIM2_CH1Handler);		//关闭定时器2
					__HAL_TIM_SET_COUNTER(&TIM2_CH1Handler,0);		//计数器为0
					TIM_RESET_CAPTUREPOLARITY(&TIM2_CH1Handler,TIM_CHANNEL_1);		//一定要先清除原来的设置
					TIM_SET_CAPTUREPOLARITY(&TIM2_CH1Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);		//定时器2通道1配置为下降沿捕获
					__HAL_TIM_ENABLE(&TIM2_CH1Handler);					//使能TIM2
				}
		}
}
