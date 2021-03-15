#include "main.h"
#include "malloc.h"
#include <cstddef>
#include <stdint.h>

/**********************************************************************************
内存池32字节对齐
**********************************************************************************/
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//内部SRAM
__align(32)	u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC01F4000)));				//外部SRAM内存池，前面2M给LTDC
																					//用了（1280*800*2）
__align(32)	u8 mem3base[MEM3_MAX_SIZE]	__attribute__((at(0X1000000)));				//内部CCM内存池

/*******************************************************************************************
内存管理表，内存MAP
*******************************************************************************************/
u32 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];														//内部SRAM
u32 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC01F4000+MEM2_MAX_SIZE)));		//外部SRAM内存池MAP	
u32 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));		//内部CCM内存池MAP

/********************************************************************************************
内存管理参数
********************************************************************************************/
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};		//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};						//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小

/************************************************************************************************
内存管理控制器
************************************************************************************************/
struct _m_malloc_dev malloc_dev=
{
									My_Mem_Init,								//内存初始化
									my_mem_perused,								//内存使用率
									mem1base,mem2base,mem3base,					//内存池
									mem1mapbase,mem2mapbase,mem3mapbase,		//内存管理状态表
									0,0,0,										//内存管理未就绪
};
/*********************************************************************************************
复制内存
*des:目标地址
*src:源地址
n：需要复制的内存长度（字节为单位）
*********************************************************************************************/
void mymemcpy(void * des, void * src, u32 n)
{
	u8 *xdes=des;
	u8 *xsrc=src;
	while(n--)
		*xdes++=*xsrc++;
}
/*********************************************************************************************
设置内存
*s：内存首地址
c：要设置的值
count:需要设置的内存大小（单位字节）
*********************************************************************************************/
void mymemset(void * s, u8 c, u32 count)
{
	u8 *xs=s;
	while(count--)
		*xs++=c;
}

/* @brief 内存管理初始化
 * @param memx 所属的内存块
 * @reval none
 */
void My_Mem_Init(uint8_t memx)
{
	/* 内存状态表数据清零 */
	mymemset(malloc_dev.memmap[memx], 0, memblksize[memx] * 4);
	/* 内存管理初始化OK */
	malloc_dev.memrdy[memx] = 1;
}

/* @brief 获取内存使用率
 * @param memx 所属的内存块
 * @reval 使用率，扩大了10倍
 *			0 ～ 1000 -> 0.0% ~ 100.0%
 */
uint16_t my_mem_perused(uint8_t memx)
{
	uint32_t used = 0;
	uint32_t i;

	for (i = 0; i < memtblsize[memx]; i++)
	{
		if (malloc_dev.memmap[memx][i])
		{
			used++;
		}
	}
	return (used * 1000) / (memtblsize[memx]);
}

/* @brief 内存分配（内部调用）
 * @param memx 所属的内存块
 * @param size 要分配的内存大小（字节）
 * @reval 0xFFFFFFFF -> ERROR
 *			other -> 内存偏移地址
 */
uint32_t my_mem_malloc(uint8_t memx, uint32_t size)
{
	signed long offset = 0;
	uint32_t nmemb; /* 需要的内存块数 */
	uint32_t cmemb = 0; /* 连续空内存块数 */
	uint32_t i;

	/* 未初始化，先执行初始化 */
	if (malloc_dev.memrdy[memx] == 0)
	{
		malloc_dev.init(memx);
	}

	if (size == 0)
	{
		return 0xFFFFFFFF; /* 不需要分配 */
	}

	/* 获取需要分配的连续内存块数 */
	nmemb = size / memblksize[memx];
	if (size % memblksize[memx] != 0) 
	{
		nmemb++;
	}

	/* 搜索整个内存控制区 */
	for (offset = memtblsize[memx] - 1; offset >= 0; offset--)
	{
		/* 连续空内存块数增加 */
		if (malloc_dev.memmap[memx][offset] == 0)
		{
			cmemb++;
		}
		/* 连续空内存块清零 */
		else
		{
			cmemb = 0;
		}

		/* 找到了连续nmemb个空内存块 */
		if (cmemb == nmemb)
		{
			/* 标注内存块非空 */
			for (i = 0; i < nmemb; i++)
			{
				malloc_dev.memmap[memx][offset + i] = nmemb;
			}
			/* 返回偏移地址 */
			return (offset * memblksize[memx]);
		}
	}

	/* 未找到符合分配条件的内存块 */
	return 0xFFFFFFFF;
}

/* @brief free memory 
 * @param memx own memory block
 * @param offset memory address offset
 * @reval 0 -> free successful
 *			1 -> free failed
 */
uint8_t my_mem_free(uint8_t memx, uint32_t offset)
{
	int i;

	/* none init ,first to init */
	if (malloc_dev.memrdy[memx] == 0)
	{
		malloc_dev.init(memx);
		return 1;
	}

	/* offset in memory pool */
	if (offset < memsize[memx])
	{
		int index = offset / memblksize[memx];	/* 偏移所在内存块号*/
		int nmemb = my_mem_malloc.memmap[memx][index]; /* 内存块数量 */
		/* 内存块清零 */
		for (i = 0; i < nmemb; i++)
		{
			malloc_dev.memmap[memx][index + i] = 0;
		}
		return 0;
	}
	else
	{
		return 2;	/* 偏移超区了 */
	}
}

/* @brief free memory (extern callback)
 * @param memx own memory block
 * @param ptr point or memory first address
 * @reval none
 */
void myfree(uint8_t memx, void *ptr)
{
	uint32_t offset;
	if (ptr == NULL)
	{
		return ; /* address is 0 */
	}
	offset = (uint32_t)ptr - (uint32_t)malloc_dev.membase[memx];
	my_mem_free(memx, offset);
}

/* @brief 分配内存（外部调用）
 * @param memx 所属的内存块
 * @param size 内存大小（字节）
 * @reval 分配到的内存首地址
 */
void *mymalloc(uint8_t memx, uint32_t size)
{
	uint32_t offset;
	offset = my_mem_malloc(memx, size);
	if (offset == 0xFFFFFFFF)
	{
		return NULL;
	}
	else
	{
		return (void *)((uint32_t)malloc_dev.membase[memx] + offset);
	}
}

/* @brief 重新分配内存（外部调用）
 * @param memx 所属的内存块
 * @param ptr 旧内存首地址
 * @param size 要分配的内存大小（字节）
 * @reval 新分配到的内存首地址
 */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size)
{
	uint32_t offset;
	offset = my_mem_malloc(memx, size);
	if (offset == 0xFFFFFFFF)
	{
		return NULL;
	}
	else
	{
		mymemcpy((void *)((uint32_t)malloc_dev.membase[memx] + offset), ptr, size); /* 拷贝旧内存内容到新内存 */
		myfree(memx, ptr);															/* free old memory */
		return (void *)((uint32_t)malloc_dev.membase[memx] + offset);				/* return new memory first address */
	}
}
