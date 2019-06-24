#ifndef _NRF24L01_H
#define _NRF24L01_H

#include "main.h"

/**********************	NRF24L01寄存器操作指令	*************************************/
#define	NRF_READ_REG	0X00				//读配置寄存器，低5位为寄存器地址
#define	NRF_WRITE_REG	0X20				//写配置寄存器，低5位为寄存器地址
#define RD_RX_PLOAD		0X61				//读RX有效数据，1-32字节
#define	WR_TX_PLOAD		0XA0				//写TX有效数据，1-32字节
#define	FLUSH_TX		0XE1				//清除TX FIFO寄存器，发射模式下用
#define	FLUST_RX		0XE2				//清除RX FIFO寄存器，接收模式下用
#define	REUSE_TX_PL		0XE3				//全新使用上一包数据，CE为高，数据包被不断发送
#define	NOP				0XFF				//空操作，可以用来读状态寄存器

/***************************	SPI(NRF24L01)寄存器地址	********************************/
#define	CONFIG			0X00				//配置寄存器地址，BIT0:1,接收模式；0，发射模式。BIT1:1,上电；0，掉电
											//BIT2:CRC模式。BIT3：CRC使能。BIT4:中断MAX_RT(达到最大重发次数中断)使能
											//BIT5:中断TX_DS使能。BIT6：中断RX_DR使能
#define	EN_AA			0X01				//使能自动应答，BIT0-5对应通道0-5
#define	EN_RXADDR		0X02				//接收地址允许，BIT0-5对应通道0-5
#define	SETUP_AW		0X03				//设置地址宽度（所有数据通道），BIT[1:0]:01,3字节；10，4字节；11，5字节。
#define	SETUP_RETR		0X04				//建立自动重发，BIT[3:0]:自动重发次数。BIT[7:4],自动重发延时，250*（X+1）+86us
#define	RF_CH			0X05				//RF通道，BIT[6:0],工作通道频率
#define	RF_SETUP		0X06				//RF寄存器，BIT3:传输速率。BIT[2:1]:发射功率。BIT0:低噪声放大增益
#define	STATUS			0X07				//状态寄存器。BIT6:接收数据中断。BIT5:数据发送完成中断
											//BIT4:达到最大重发次数中断。BIT[3:1]:接收数据通道号，最大6。
											//BIT0:TX FIFO寄存器满
			//状态寄存器状态
			#define	MAX_TX		0X10		//最大重发次数
			#define	TX_OK		0X20		//发送完成
			#define	RX_OK		0X40		//接收完成
			
#define	OBSERVE_TX		0X08				//发送检测寄存器，BIT[7:4]:数据包丢失计数器。BIT[3:0]:重发计数器
#define	CD				0X09				//载波检测寄存器。
#define	RX_ADDR_P0		0X0A				//数据通道0接收地址，最大长度5个字节，低字节在前
#define	RX_ADDR_P1		0X0B				//数据通道1接收地址，最大长度5个字节，低字节在前
#define	RX_ADDR_P2		0X0C				//数据通道2接收地址，最低字节可设置，高字节必须和RX_ADDR_P1相同
#define	RX_ADDR_P3		0X0D				//数据通道3接收地址，最低字节可设置，高字节必须和RX_ADDR_P1相同
#define	RX_ADDR_P4		0X0E				//数据通道4接收地址，最低字节可设置，高字节必须和RX_ADDR_P1相同
#define	RX_ADDR_P5		0X0F				//数据通道5接收地址，最低字节可设置，高字节必须和RX_ADDR_P1相同
#define	TX_ADDR			0X10				//发送地址，低字节在前，ShockBurstTM模式下，RX_ADDR_P0与此地址相同
#define	RX_PW_P0		0X11				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	RX_PW_P1		0X12				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	RX_PW_P2		0X13				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	RX_PW_P3		0X14				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	RX_PW_P4		0X15				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	RX_PW_P5		0X16				//接收数据通道0有效数据宽度（1-32字节），设置为0则非法
#define	NRF_FIFO_STATUS	0X17				//FIFO状态寄存器。BIT0:RX FIFO寄存器空标志。BIT1:RX FIFO满标志
											//BIT4:TX FIFO寄存器空标志。BIT5:TX FIFO满标志。
											//BIT6:1,循环发送一包数据；0，不循环

/*****************************************************************************
NRF24L01操作线
*****************************************************************************/
#define	NRF24L01_CE		PGout(12)			//24L01片选
#define	NRF24L01_CSN	PGout(10)			//SPI片选
#define	NRF24L01_IRQ	PIin(11)			//IRQ主机数据输入

/*********** 24L01发送接收数据宽度定义	*************************************/
#define	TX_ADR_WIDTH			5			//5字节地址宽度
#define	RX_ADR_WIDTH			5			//5字节地址宽度
#define	TX_PLOAD_WIDTH			32			//32字节的用户数据宽度
#define	RX_PLOAD_WIDTH			32			//32字节的用户数据宽度


void NRF24L01_Init(void);											//初始化
void NRF24L01_RX_Mode(void);										//配置为接收模式
void NRF24L01_TX_Mode(void);										//配置为发送模式
u8 NRF24L01_Write_Buf(u8 reg,u8 *pBuf,u8 len);						//写数据区
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len);						//读数据区
u8 NRF24L01_Read_Reg(u8 reg);										//读寄存器
u8 NRF24L01_Write_Reg(u8 reg,u8 value);								//写寄存器
u8 NRF24L01_Check(void);											//检测24L01是否存在
u8 NRF24L01_TxPacket(u8 *txbuf);									//发送一个包的数据
u8 NRF24L01_RxPacket(u8 *rxbuf);									//接收一个包的数据

#endif

