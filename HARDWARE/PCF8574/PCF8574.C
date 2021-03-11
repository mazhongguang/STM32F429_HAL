#include "main.h"
/********************* 初始化PCF8674 *************************************/
u8 PCF8574_Init(void)
{
	u8 temp=0;

	GPIO_InitTypeDef	GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB

	GPIO_Initure.Pin=GPIO_PIN_12;
	GPIO_Initure.Mode=GPIO_MODE_INPUT;			//输入
	GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);		//初始化

	IIC_Init();								//IIC初始化

	/************** 检查PCF8574是否存在 ****************************/
	IIC_Start();
	IIC_Send_Byte(PCF8574_Cmd_Write);			//写地址
	temp=IIC_Wait_Ack();						//等待应答，通过是否有ACK应答来判断PCF8574的状态
	IIC_Stop();								//产生停止条件
	PCF8574_WriteOneByte(0xFF);				//默认所有IO输出高

	return temp;
}
/************************************************************************
读取PCF8574的8位IO值
返回值:读到的数据
***********************************************************************/
u8 PCF8574_ReadOneByte(void)
{
	u8 temp=0;

	IIC_Start();
	IIC_Send_Byte(PCF8574_Cmd_Read);				//进入接收模式
	IIC_Wait_Ack();
	temp=IIC_Read_Byte(0);
	IIC_Stop();
	return temp;
}
/**********************************************************************
向PCF8574写入8位的IO值
DataToWrite:要写入的数据
**********************************************************************/
void PCF8574_WriteOneByte(u8 DataToWrite)
{
	IIC_Start();
	IIC_Send_Byte(PCF8574_Cmd_Write);				//写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);					//发送字节
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
}
/********************************************************************
设置PCF8574某个IO的高低电平
bit:要设置的IO编号，0~7
sta:IO的状态，0&1
********************************************************************/
void PCF8574_WriteBit(u8 bit, u8 sta)
{
	u8 data;
	data=PCF8574_ReadOneByte();					//先读出原来的设置
	if(sta==0)
		 data&=~(1<<bit);
	else
		data|=1<<bit;
	PCF8574_WriteOneByte(data);					//写入新的数据
}
/***********************************************************************
读取PCF8574某个IO的值
bit:要读取的IO编号，0~7
返回值:此IO的值，0&1
***********************************************************************/
u8 PCF8574_ReadBit(u8 bit)
{
	u8 data;
	data=PCF8574_ReadOneByte();					//先读取这8 个IO的值
	if(data&(1<<bit))
		return 1;
	else
		return 0;
}
