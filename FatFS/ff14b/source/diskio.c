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
#include "bsp_sdio_sd.h"
#include "bsp_flash_w25q.h"

/* Definitions of physical drive number for each drive */
#define SD_CARD		0	/* Example: Map Ramdisk to physical drive 0 */
#define W25Q_FLASH 1



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
		case SD_CARD :
			//nothin to do
			return RES_OK;
		case W25Q_FLASH :
			if(W25Q_flash_ReadID()!=W25Q_ID) return RES_ERROR;
			else return RES_OK;
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
	DSTATUS stat;
	int result;

	switch (pdrv) {
		case SD_CARD :
			stat = bsp_SD_Init();
			return stat;
			// translate the reslut code here
		case W25Q_FLASH :
			flash_w25qxx_Init();
			return RES_OK;
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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here
		if(bsp_SD_Read(buff, sector, count) != 0) return RES_ERROR;
		else return RES_OK;
	case W25Q_FLASH :
		W25Q_flash_Read((BYTE *)buff,(sector*4096),(count*4096));
		return RES_OK;
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
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		
		if(bsp_SD_Write((BYTE *)buff, sector, count) != 0) return RES_ERROR;
		else return RES_OK;
		
	case W25Q_FLASH :
		bsp_flash_write((BYTE *)buff,sector,count);
		return RES_OK;
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
	DRESULT res;
	int result;

	switch (pdrv) {
		case SD_CARD :
			if(cmd == CTRL_SYNC) res= RES_OK;
			else if(cmd == GET_SECTOR_COUNT) *(DWORD *)buff = SD_Info.BlockNbr;
			else if(cmd == GET_SECTOR_SIZE)  *(WORD *)buff = 512;
			else if(cmd == GET_BLOCK_SIZE)  *(DWORD *)buff = SD_Info.BlockSize;
			break;
		case W25Q_FLASH :
			if(cmd == CTRL_SYNC) res= RES_OK;
			else if(cmd == GET_SECTOR_COUNT) *(DWORD *)buff = 1536;
			else if(cmd == GET_SECTOR_SIZE)  *(WORD *)buff = 4096;
			else if(cmd == GET_BLOCK_SIZE)  *(DWORD *)buff = 1;
			break;
			
	}
	
	if(cmd == CTRL_SYNC || cmd == GET_SECTOR_COUNT || cmd == GET_SECTOR_SIZE || cmd == GET_BLOCK_SIZE) return RES_OK;
	return RES_PARERR;
}

uint32_t get_fattime(void){
	//There is not nessary to implement this function
	return 0;
}