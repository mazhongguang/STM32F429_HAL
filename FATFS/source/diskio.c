/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "sdio_sdcard.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		3	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SD_CARD 0


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = FR_OK;
	int result;

	switch (pdrv) {
#if 0
	case DEV_RAM :
		/*result = RAM_disk_status();*/

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		/*result = MMC_disk_status();*/

		// translate the reslut code here

		return stat;

	case DEV_USB :
		/*result = USB_disk_status();*/

		// translate the reslut code here

		return stat;
#endif
	case SD_CARD:
		result = 0;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = FR_OK;
	int result;

	switch (pdrv) {
#if 0
	case DEV_RAM :
		/*result = RAM_disk_initialize();*/

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		/*result = MMC_disk_initialize();*/

		// translate the reslut code here

		return stat;

	case DEV_USB :
		/*result = USB_disk_initialize();*/

		// translate the reslut code here

		return stat;
#endif
	case SD_CARD:
		result = sd_init();
		return result;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = 0;
	int result;

	if (count == 0)
	{
		return RES_PARERR;
	}

	switch (pdrv) {
#if 0
	case DEV_RAM :
		// translate the arguments here

		/*result = RAM_disk_read(buff, sector, count);*/

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		/*result = MMC_disk_read(buff, sector, count);*/

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		/*result = USB_disk_read(buff, sector, count);*/

		// translate the reslut code here

		return res;
#endif	
	case SD_CARD:
			res  = sd_readdisk(buff, sector, count);
		while (res != 0)
		{
			sd_init();
			res = sd_readdisk(buff, sector, count);
		}
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = 0;
	int result;

	if (count == 0)
	{
		return RES_PARERR;
	}

	switch (pdrv) {
#if 0
	case DEV_RAM :
		// translate the arguments here

		/*result = RAM_disk_write(buff, sector, count);*/

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		/*result = MMC_disk_write(buff, sector, count);*/

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		/*result = USB_disk_write(buff, sector, count);*/

		// translate the reslut code here

		return res;
#endif	
	case SD_CARD:
		res = sd_writedisk((uint8_t *)buff, sector, count);
		while (res != 0)
		{
			sd_init();
			res = sd_writedisk((uint8_t *)buff, sector, count);
		}
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = 0;
	int result;

	switch (pdrv) {
#if 0
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
#endif
	case SD_CARD:
		switch (cmd)
		{
			case CTRL_SYNC:
				return RES_OK;
			case GET_SECTOR_SIZE:
				*(DWORD *)buff = 512;
				return RES_OK;
			case GET_BLOCK_SIZE:
				*(WORD *)buff = SDCARD_Handler.SdCard.BlockSize;
				return RES_OK;
			case GET_SECTOR_COUNT:
				*(DWORD *)buff = SDCARD_Handler.SdCard.BlockNbr;
				return RES_OK;
			default:
				return RES_PARERR;
		}
	}

	return RES_PARERR;
}

// get current time
/*User defined function to give a current time to fatfs module      */
/*31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/*15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}
