#include "main.h"
SPI_HandleTypeDef	SPI5_Handler;			//SPI5句柄
SPI_HandleTypeDef	SPI2_Handler;			//SPI2句柄

/**********************************************************************
SPI模块的初始化，配置成主机
SPI口初始化
针对SPI5
*********************************************************************/
void SPI5_Init(void)
{
	SPI5_Handler.Instance=SPI5;
	SPI5_Handler.Init.Mode=SPI_MODE_MASTER;				//SPI模式，主机模式
	SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;		//SPI设置为双线模式
	SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;			//SPI发送接收8位帧结构
	SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;		//同步时钟空闲状态为高电平
	SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;			//同步时钟第二个跳变沿数据被采样
	SPI5_Handler.Init.NSS=SPI_NSS_SOFT;					//NSS信号由硬件(NSS管脚)控制
	SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;		//定义波特率预分频值为256
	SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;						//指定数据传输从MSB位开始
	SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;						//关闭TI模式
	SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;		//关闭CRC
	SPI5_Handler.Init.CRCPolynomial=7;									//CRC值计算的多项式,(默认值0X0007)
	HAL_SPI_Init(&SPI5_Handler);

	__HAL_SPI_ENABLE(&SPI5_Handler);									//使能SPI5

	SPI5_ReadWriteByte(0XFF);										//启动一次传输
}
/**********************************************************************
SPI模块的初始化，配置成主机
SPI口初始化
针对SPI2
*********************************************************************/
void SPI2_Init(void)
{
	SPI2_Handler.Instance=SPI2;
	SPI2_Handler.Init.Mode=SPI_MODE_MASTER;				//SPI模式，主机模式
	SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;		//SPI设置为双线模式
	SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;			//SPI发送接收8位帧结构
	SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;		//同步时钟空闲状态为高电平
	SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;			//同步时钟第二个跳变沿数据被采样
	SPI2_Handler.Init.NSS=SPI_NSS_SOFT;					//NSS信号由硬件(NSS管脚)控制
	SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;		//定义波特率预分频值为256
	SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;						//指定数据传输从MSB位开始
	SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;						//关闭TI模式
	SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;		//关闭CRC
	SPI2_Handler.Init.CRCPolynomial=7;									//CRC值计算的多项式,(默认值0X0007)
	HAL_SPI_Init(&SPI2_Handler);

	__HAL_SPI_ENABLE(&SPI5_Handler);									//使能SPI5

	SPI5_ReadWriteByte(0XFF);										//启动一次传输
}

/**************************************************************************************
SPI5底层驱动，时钟使能，引脚配置
此函数会被HAL_Spi_Init()调用
hspi:SPI句柄
*************************************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef * hspi)
{
	GPIO_InitTypeDef	GPIO_Initure;

	__HAL_RCC_GPIOF_CLK_ENABLE();						//使能GPIOF时钟
	__HAL_RCC_SPI5_CLK_ENABLE();							//使能SPI5时钟
	__HAL_RCC_SPI2_CLK_ENABLE();							//使能SPI2时钟

	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;		//PF7,PF8,PF9
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;						//复用推挽输出
	GPIO_Initure.Speed=GPIO_SPEED_FAST;						//快速
	GPIO_Initure.Pull=GPIO_PULLUP;							//上拉
	GPIO_Initure.Alternate=GPIO_AF5_SPI5;					//复用为SPI5
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;	//PB13,PB14,PB15
	GPIO_Initure.Alternate=GPIO_AF5_SPI2;					//复用为SPI2
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);						//初始化
}
/*******************************************************************************
SPI速度设置函数
SPI速度=fAPB1/分频系数
@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
fAPB1时钟一般为45MHz
*******************************************************************************/
void SPI5_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	__HAL_SPI_DISABLE(&SPI5_Handler);									//关闭SPI
	SPI5_Handler.Instance->CR1&=0xFFC7;									//位3-5清零，用来设置波特率
	SPI5_Handler.Instance->CR1|=SPI_BaudRatePrescaler;					//设置SPI速度
	__HAL_SPI_ENABLE(&SPI5_Handler);										//使能SPI
}
/*******************************************************************************
SPI速度设置函数
SPI速度=fAPB1/分频系数
@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
fAPB1时钟一般为45MHz
*******************************************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	__HAL_SPI_DISABLE(&SPI2_Handler);									//关闭SPI
	SPI2_Handler.Instance->CR1&=0xFFC7;									//位3-5清零，用来设置波特率
	SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;					//设置SPI速度
	__HAL_SPI_ENABLE(&SPI2_Handler);										//使能SPI
}

/*******************************************************************************
SPI5读写一个字节
TxData:要写入的字节
返回值:读取到的字节
*******************************************************************************/
u8 SPI5_ReadWriteByte(u8 TxData)
{

	u8 Rxdata;
	HAL_SPI_TransmitReceive(&SPI5_Handler,&TxData,&Rxdata,1,1000);
	return Rxdata;
}
/*******************************************************************************
SPI2读写一个字节
TxData:要写入的字节
返回值:读取到的字节
*******************************************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{
	u8 Rxdata; 
	HAL_SPI_TransmitReceive(&SPI2_Handler,&TxData,&Rxdata,1,1000);
	return Rxdata;
}

