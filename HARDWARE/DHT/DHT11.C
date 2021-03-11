#include "main.h"
/**************************************** 复位DHT11 ********************************/
void DHT11_Rst(void)
{
	DHT11_IO_OUT();						//设置为输出 
	DHT11_DQ_OUT=0;							//拉低DQ
	delay_ms(19);							//至少拉低18MS
	DHT11_DQ_OUT=1;							//DQ=1
	delay_us(30);							//主机拉高20-40us
}
/**********************************************************************************
等待DHT11的回应
返回1，未检测到DHT11
返回0，存在DHT11
**********************************************************************************/
u8 DHT11_Check(void)
{
	u8 retry=0;
	DHT11_IO_IN();							//设置为输入
	while(DHT11_DQ_IN&&retry<100)			//DHT会拉低40-80us
		{
			retry++;
			delay_us(1);
		};
	if(retry>=100)							//DHT11超时未响应
		return 1;
	else
		retry=0;
	while(!DHT11_DQ_IN&&retry<100)			//DHT11拉低后会再次拉高40-80us
		{
			retry++;
			delay_us(1);
		};
	if(retry>=100)							//DHT11超时
		return 1;
	return 0;
}
/**********************************************************************************
从DHT11读取一个位
返回值:1&0
**********************************************************************************/
u8 DHT11_Read_Bit(void)
{
	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)						//等待变为低电平
		{	
			retry++;
			delay_us(1);
		}
	retry=0;
	while(!DHT11_DQ_IN&&retry<55)						//等待变高电平(此时为数据0，1的低电平)
		{
			retry++;
			delay_us(1);
		}
	delay_us(35);										//等待35us(数据0的高电平时间为DHT11{26-28],DHT22[22-30])
	if(DHT11_DQ_IN)										//35us后依然高电平则是数据1的高电平
		return 1;
	else
		return 0;
}
/*****************************************************************************
从DHT读取 一个字节
返回值:读到的数据
*****************************************************************************/
u8 DHT11_Read_Byte(void)
{
	u8 i,dat=0;
	for(i=0;i<8;i++)
		{
			dat<<=1;
			dat|=DHT11_Read_Bit();
		}
	return dat;
}
/*****************************************************************************
从DHT22读取一次数据 
temp:温度值整数部分
temp_f:温度值小数部分
humi:湿度值整数部分
humi_f:湿度值小数部分
sign:零上&零下，温度极性(sign=1,零下，sign=0,零上)
返回值: 0，正常
		1，失败
*****************************************************************************/
u8 DHT22_Read_Data(u8 * temp, u8 * temp_f, u8 * humi, u8 * humi_f, u8 * sign)
{
	u8 buf[5];
	u16 tem;
	u8 i;
	DHT11_Rst();									//复位DHT
	if(DHT11_Check()==0)							//如果存在DHT11，则读取数据
		{
			for(i=0;i<5;i++)						//依次读取40位数据，存于BUF[]中
				{
					buf[i]=DHT11_Read_Byte();
				}
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])		//数据校验成功
				{		
					tem=(buf[0]<<8)+buf[1];					//16位湿度值
					*humi=tem/10;							//湿度整数
					*humi_f=tem%10;						//湿度小数部分
					printf("t1:%d\r\n",tem);
					tem=(buf[2]<<8)+buf[3];					//16位温度值
					printf("h1:%d\r\n",tem);
					*temp=tem/10;							//温度整数
					*temp_f=tem%10;						//温度小数部分
					*sign=(tem&0x8000)>>16;				//温度的正负
				}
		}
	else													//未检测到DHT
		return 1;		
	return 0;												//正常结束
}
/*****************************************************************************
从DHT11读取一次数据 
temp:温度值整数部分
humi:湿度值整数部分
返回值: 0，正常
		1，失败
*****************************************************************************/
u8 DHT11_Read_Data(u8 * temp,u8 * humi)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();											//复位DHT
	if(DHT11_Check()==0)									//如果存在DHT11，则读取数据
		{
			for(i=0;i<5;i++)								//依次读取40位数据，存于BUF[]中
				{
					buf[i]=DHT11_Read_Byte();
				}
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])		//数据校验成功
				{		
					*humi=buf[0];
					*temp=buf[2];
				}
		}
	else													//未检测到DHT
		return 1;		
	return 0;												//正常结束
}
/*************************************************************************************
初始化DHT的IO口DQ，同时检测DHT是否存在
返回1，不存在
	0，存在
*************************************************************************************/
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef	GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();				//使能GPIOB

	GPIO_Initure.Pin=GPIO_PIN_12;					//PB12
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;			//推挽
	GPIO_Initure.Pull=GPIO_PULLUP;					//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;				//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	DHT11_Rst();
	return DHT11_Check();
}

