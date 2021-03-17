#include "fattester.h"
#include "ff.h"
#include "ffconf.h"
#include <stdint.h>
#include <stdio.h>

/* common file */
DIR dir;
FILINFO fileinfo;

/* @brief show free size
 * @param drv driver letter
 * @reval free size (unit byte)
 */
uint32_t f_showfree(uint8_t *drv)
{
	FATFS *fs1;
	uint8_t res;
	uint32_t free_clust = 0, free_sect = 0, total_sect = 0;

	/* get disk info and free cluster number */
	res = f_getfree((const TCHAR *)drv, (DWORD *)&free_clust, &fs1);

	if (res == 0)
	{
		total_sect = (fs1->n_fatent - 2) * fs1->csize;
		free_sect = free_clust * fs1->csize;

#if __MAX_SS != 512
//		total_sect *= fs1->ssize / 512;
//		free_sect = free_clust * fs1->csize;
#endif
		if (total_sect < 20480)	/* less 10MB */
		{
			printf("\r\n磁盘总容量：%d KB\r\n"
					"可用空间： %d KB\r\n",
					total_sect >> 1, free_sect >> 1);
		}
		else
		{
			printf("\r\n磁盘总容量：%d MB\r\n"
					"可用空间： %d MB\r\n",
					total_sect >> 11, free_sect >> 11);
		}
	}
	return free_sect;
}

/* @brief scan file
 * @param path 
 * @reval result
 */
uint8_t f_scan_files(uint8_t *path)
{
	FRESULT res;

	res = f_opendir(&dir, (const TCHAR *)path);
	if (res == FR_OK)
	{
		printf("\r\n");
		while (1)
		{
			res = f_readdir(&dir, &fileinfo);
			if (res != FR_OK || fileinfo.fname[0] == 0)
			{
				break;
			}
			printf("%s/", path);
			printf("%s\r\n", fileinfo.fname);
		}
	}
	return res;
}
