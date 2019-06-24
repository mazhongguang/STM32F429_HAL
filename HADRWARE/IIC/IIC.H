#ifndef _IIC_H
#define _IIC_H

#include "main.h"
/***************** IO方向设置 *********************************/
#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}				//PH5输入模式(前半段清空模式位，后半段设置模式位)
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;}				//PH5输出模式(前半段清空模式位，后半段设置模式位)

/**************** IO操作 *************************************/
#define IIC_SCL   PHout(4)			//SCL
#define IIC_SDA   PHout(5)			//SDA
#define READ_SDA  PHin(5)			//输入SDA

/**************************** IIC所有操作函数 **********************************/
void IIC_Init(void);					//初始化IIC的IO口
void IIC_Start(void);				//开始信号
void IIC_Stop(void);				//停止信号
void IIC_Send_Byte(u8 txd);		//IIC发送一个字节
u8 IIC_Read_Byte(u8 ack);		//读取一个字节
u8 IIC_Wait_Ack(void);			//等待ACK信号
void IIC_Ack(void);					//发送ACK信号
void IIC_NAck(void);				//不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
#endif
