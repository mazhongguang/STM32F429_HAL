#ifndef __MAIN_H
#define __MAIN_H

/******************************************************************************
条件编译 宏定义
******************************************************************************/
#define LED_H
#define KEY_H
//#define EXTI_H
#define LCD_H
//#define REMOTE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stdlib.h"

/*******************个人包含头文件*******************************************/

#include "sys.h"
#include "delay.h"
#include "usart.h"

#ifdef LED_H
	#include "led.h"
#endif

#ifdef KEY_H
	#include "key.h"
#endif

#ifdef EXTI_H
	#include "exti.h"
#endif	
	
#ifdef LCD_H
	#include "font.h"
	#include "ili9341.h"
#endif

#ifdef USMART_H
	#include "usmart_str.h"
	#include "usmart.h"
#endif	
	
#ifdef IIC_H	
	#include "iic.h"
#endif	

#ifdef IIC_24CXX_H
	#include "24cxx.h"
#endif	

#ifdef RTC_H
	#include "rtc.h"
#endif

#ifdef PCF8574_H
	#include "pcf8574.h"
#endif	

#ifdef AP3216C_H
	#include "ap3216c.h"
#endif

#ifdef SPI_H
	#include "spi.h"
#endif

#ifdef	W25QXX_H
	#include "w25qxx.h"
#endif

#ifdef DHT11_H
	#include "dht11.h"
#endif

#ifdef TIMER_H
	#include "timer.h"
#endif

#ifdef HC_SR04_H
	#include "hc_sr04.h"
#endif

#ifdef SDRAM_H
	#include "sdram.h"
#endif

#ifdef NRF24L01_H
	#include "nrf24l01.h"
#endif

#ifdef STMFLASH_H
	#include "stmflash.h"
#endif

#ifdef WKUP_H
	#include "wkup.h"
#endif

#ifdef REMOTE_H
	#include "remote.h"
#endif

#endif /* __MAIN_H */
