#ifndef __MALLOC_H
#define __MALLOC_H

#include "main.h"

#ifndef NULL
#define NULL 0
#endif

/*************** 定义三个内存池	**********************************/
#define SRAMIN		0				//内部内存池
#define	SRAMEX		1				//外部内存池
#define	SRAMCCM		2				//CCM内存池（此部分SRAM仅CPU可以访问）

#define	SRAMBANK	3				//定义SRAM支持的块数

/******************************************************************************************
MEM1内存参数设定，MEM1完全处于内部SRAM里
******************************************************************************************/
#define	MEM1_BLOCK_SIZE						64									//内存块大小为64字节
#define	MEM1_MAX_SIZE						160*1024							//最大管理内存160K字节
#define	MEM1_ALLOC_TABLE_SIZE				MEM1_MAX_SIZE/MEM1_BLOCK_SIZE		//内存表大小

/******************************************************************************************
MEM2内存参数设定，MEM1完全处于内部SRAM里
******************************************************************************************/
#define	MEM2_BLOCK_SIZE						64									//内存块大小为64字节
#define	MEM2_MAX_SIZE						28912*1024							//最大管理内存28912K字节
#define	MEM2_ALLOC_TABLE_SIZE				MEM2_MAX_SIZE/MEM2_BLOCK_SIZE		//内存表大小
/******************************************************************************************
MEM1内存参数设定，MEM1完全处于内部SRAM里
******************************************************************************************/
#define	MEM3_BLOCK_SIZE						64									//内存块大小为64字节
#define	MEM3_MAX_SIZE						60*1024								//最大管理内存60K字节
#define	MEM3_ALLOC_TABLE_SIZE				MEM3_MAX_SIZE/MEM3_BLOCK_SIZE		//内存表大小

/*************************  内存管理控制器		    ***************************************************/
struct_m_malloc_dev
{
	void (*init)(u8);									//初始化
	u16	(*perused)(u8);									//内存使用率
	u8 *membase[SRAMBANK];								//内存池管理SRAMBANK个区域的内存
	u32	*memmap[SRAMBANK];								//内存管理状态表
	u8 memrdy[SRAMBANK];								//内存管理是否就绪
};
extern struct_m_malloc_dev	mallco_dev;					//在mallco.c里面定义

void mymemset(void *s,u8 c,u32 count);								//设置内存
void mymemcpy(void *des,void *src,u32 n);							//复制内存
void My_Mem_Init(u8 memx);											//内存管理初始化函数（外/内部调用）
u32 my_mem_malloc(u8 memx,u32 size);								//内存分配，内部调用 
u8  my_mem_free(u8 memx,u32 offset);								//内存释放，内部调用 
u16 my_mem_perused(u8 memx);										//获得内存使用率，外/内部调用 

/*******************************  用户调用函数        *********************************************/
void myfree(u8 memx,void *ptr);										//内存释放，外部调用
void *mymalloc(u8 memx,u32 size);									//内存分配，外部调用
void *myrealloc(u8 memx,void *ptr,u32 size);						//重新分配内存，外部调用

#endif
