#include "main.h"
/**********************************************************************
 初始化AP3216C
 返回值:0,	初始化成功
 		1，	初始化失败
*********************************************************************/
u8 AP3216C_Init(void)
{
	u8 temp=0;
	IIC_Init();
	AP3216C_WriteOneByte(0x00,0x04);			//复位AP3216C
	delay_ms(50);									//AP3216C复位至少10MS
	AP3216C_WriteOneByte(0X00,0X03);			//开启ALS+PS+IR
	temp=AP3216C_ReadOneByte(0X00);				//读取刚刚写进去的0X03
	if(temp==0x03)									//AP3216C正常
		return 0;
	else											//AP3216C失败
		return 1;					
}
/***********************************************************************
读取AP3216C的数据
读取原始数据，包括ALS,PS,IR
注意:如果同时打开ALS+PS+IR的话，两次数据读取的时间间隔要大于112.5ms
***********************************************************************/
void AP3216C_ReadData(u16 * ir, u16 * ps, u16 * als)
{
	u8 i,buf[6];
	for(i=0;i<6;i++)
		{
			buf[i]=AP3216C_ReadOneByte(0x0A+i);				//循环读取所有传感器数据
		}
	if(buf[0]&0x80)											//IR_OF位为1，则数据无效
		*ir=0;	
	else														//读取IR传感器的数据
		*ir=((u16)buf[1]<<2)|(buf[0]&0x03);
	*als=((u16)buf[3]<<8)|buf[2];								//读取ALS传感器的数据
	if(buf[4]&0x40)											//IR_OF位为1，则数据无效
		*ps=0;
	else														//读取PS传感器的数据
		*ps=((u16)(buf[5]&0x3F)<<4)|(buf[4]&0X0F);
}
/***********************************************************************
IIC写一个字节
reg:寄存器地址
data:要写入的数据 
返回值:		0,		正常
		其它，		错误代码
***********************************************************************/
u8 AP3216C_WriteOneByte(u8 reg, u8 data)
{
	IIC_Start();
	IIC_Send_Byte(AP3216C_Cmd_Write);				//发送器件地址加写命令
	if(IIC_Wait_Ack())							//等待应答
		{
			IIC_Stop();							//接收应答失败
			return 1;
		}
	IIC_Send_Byte(reg);							//写寄存器地址
	IIC_Wait_Ack();								//等待应答
	IIC_Send_Byte(data);							//发送数据
	if(IIC_Wait_Ack())							//等待应答
		{
			IIC_Stop();							//接收应答失败
			return 1;
		}
	IIC_Stop();
	return 0;
}
/**********************************************************************
IIC读一个字节
reg:寄存器地址
返回值:读到的数据
*********************************************************************/
u8 AP3216C_ReadOneByte(u8 reg)
{
	u8 res;
	IIC_Start();
	IIC_Send_Byte(AP3216C_Cmd_Write);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(AP3216C_Cmd_Read);
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);							//读数据，发送NAck
	IIC_Stop();
	return res;
}

