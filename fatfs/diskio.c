/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2025        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Basic definitions of FatFs */
#include "diskio.h"		/* Declarations FatFs MAI */
#include "usart.h"
/* Example: Declarations of the platform and disk functions in the project */
//#include "platform.h"
//#include "storage.h"

/* Example: Mapping of physical drive number for each drive */
//#define DEV_FLASH	0	/* Map FTL to physical drive 0 */
//#define DEV_MMC		1	/* Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS Stat = STA_NOINIT;

  if(BSP_SD_GetCardState() == MSD_OK)
  {
    Stat &= ~STA_NOINIT;
  }
  
  return Stat;
}


uint8_t disk_get_initstatus()
{
	return 1;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS Stat = STA_NOINIT;
  
  /* Configure the uSD device */
  if(BSP_SD_Init() == MSD_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
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
  DRESULT res = RES_ERROR;
  uint32_t timeout = 100000;

  if(BSP_SD_ReadBlocks((uint32_t*)buff, 
                       (uint32_t) (sector), 
                       count, SD_DATATIMEOUT) == MSD_OK)
  {
    while(BSP_SD_GetCardState()!= MSD_OK)
    {
      if (timeout-- == 0)
      {
        return RES_ERROR;
      }
    }
    res = RES_OK;
  }
  
  return res;
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
  DRESULT res = RES_ERROR;
  uint32_t timeout = 100000;

  if(BSP_SD_WriteBlocks((uint32_t*)buff, 
                        (uint32_t)(sector), 
                        count, SD_DATATIMEOUT) == MSD_OK)
  {
    while(BSP_SD_GetCardState()!= MSD_OK)
    {
      if (timeout-- == 0)
      {
        return RES_ERROR;
      }
    }    
    res = RES_OK;
  }
  
  return res;
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
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;
  DRESULT Stat = disk_status(pdrv);
  if (Stat & STA_NOINIT) 
	{return RES_NOTRDY;}
  
  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;
  
  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BSP_SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;
  
  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_SD_GetCardInfo(&CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;
  
  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_SD_GetCardInfo(&CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}

#if defined( SD_INFOPRINT)
#include <stdio.h>
void Get_FatFsInfo(FRESULT ret)
{
		uint16_t ret_data = (uint16_t)ret;
		LOG("return %d \r\n",ret_data);
		switch(ret_data)
		{
			case FR_OK:
				LOG("操作成功 \r\n");
				break;
			case FR_DISK_ERR:
				LOG("在低级磁盘 I/O 层发生了严重错误 \r\n");
				break;
			case FR_INT_ERR:
				LOG("断言失败 \r\n");
				break;
			case FR_NOT_READY:
				LOG("物理驱动器无法工作 \r\n");
				break;
			case FR_NO_FILE:
				LOG("找不到该文件 \r\n");
				break;
			case FR_NO_PATH:
				LOG("找不到该路径 \r\n");
				break;
			case FR_INVALID_NAME:
				LOG("路径名称格式无效 \r\n");
				break;
			case FR_DENIED:
				LOG("由于禁止访问或目录已满而被拒绝访问 \r\n");
				break;
			case FR_EXIST:
				LOG("由于禁止访问而被拒绝访问\r\n");
				break;
			case FR_INVALID_OBJECT:
				LOG("文件/目录对象无效 \r\n");
				break;
			case FR_WRITE_PROTECTED:
				LOG("物理驱动器已写保护 \r\n");
				break;
			case FR_INVALID_DRIVE:
				LOG("逻辑驱动器编号无效 \r\n");
				break;
			case FR_NOT_ENABLED:
				LOG("该卷没有工作区域 \r\n");
				break;
			case FR_NO_FILESYSTEM:
				LOG("找不到有效的 FAT 卷  \r\n");
				break;
			case FR_MKFS_ABORTED:
				LOG("由于某些问题，f_mkfs 函数被中止  \r\n");
				break;
			case FR_TIMEOUT:
				LOG("在规定时间内无法控制该卷 \r\n");
				break;
			case FR_LOCKED:
				LOG("根据文件共享策略拒绝执行操作 \r\n");
				break;
			case FR_NOT_ENOUGH_CORE:
				LOG("LFN 工作缓冲区无法分配，因为给定的缓冲区大小不足或者路径过长 \r\n");
				break;
			case FR_TOO_MANY_OPEN_FILES:
				LOG("打开文件的数量超过 FF_FS_LOCK 的限制 \r\n");
				break;
			case FR_INVALID_PARAMETER:
				LOG("给定的参数无效 \r\n");
				break;
			default:
				LOG("异常的返回值%d \r\n",ret_data);
				break;
		}
}

void GET_SDInfo(const TCHAR* path,FATFS* fatfs)
{
	BSP_SD_CardInfo CardInfo;
	BSP_SD_GetCardInfo(&CardInfo);
	
	FRESULT res_flash=FR_OK;
	DWORD nclst; // 剩余簇数、剩余扇区数、总扇区数
	
	uint64_t CardInfodata;
	float sd_sizeof_all=0,sd_sizeof_used=0,sd_sizeof_usable=0;
	LOG("***************************************\r\n");
	CardInfodata= CardInfo.CardType;
	LOG("SD卡的类型: %llu ",CardInfodata);
	switch(CardInfodata)
	{
		case CARD_SDSC:
			LOG("SC 标准SD卡 \r\n");
			break;
		case CARD_SDHC_SDXC:
			LOG("高容量SD卡（SDHC，2GB~32GB）或扩展容量SD卡（SDXC，32GB~2TB）\r\n");
			break;
		case CARD_SECURED:
			LOG("CURED 安全数字卡 \r\n");
			break;
		default:
			LOG("未定义的SD卡类型\r\n");
			break;
	}
	CardInfodata= CardInfo.CardVersion;
	LOG("SD卡的版本: %llu ",CardInfodata);
	switch(CardInfodata)
	{
		case CARD_V1_X:
			LOG("V1.XX \r\n");
			break;
		case CARD_V2_X:
			LOG("V2.XX \r\n");
			break;
		default:
			LOG("未定义的SD卡版本\r\n");
			break;
	}
	CardInfodata= CardInfo.Class;
	LOG("SD卡的类型··········: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.RelCardAdd;
	LOG("SD卡的相对地址······: %llx \r\n",CardInfodata);
	CardInfodata= CardInfo.BlockNbr;
	LOG("SD卡的总扇区数大小··: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.BlockSize;
	LOG("SD卡的单个扇区大小··: %llu  字节\r\n",CardInfodata);
	CardInfodata= CardInfo.LogBlockNbr;
	LOG("SD卡的逻辑扇区数····: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.LogBlockSize;
	LOG("SD卡的单个逻辑扇大小: %llu  字节\r\n",CardInfodata);
	sd_sizeof_all = CardInfo.BlockNbr*1.0*CardInfo.BlockSize;
	LOG("SD卡的总容量大小····: %.0f  字节 %.0f kb %.0f Mb %.2f Gb\r\n",\
		sd_sizeof_all,sd_sizeof_all/1024,sd_sizeof_all/1024/1024,sd_sizeof_all/1024/1024/1024);

	
	res_flash = f_getfree("",&nclst,&fatfs);
    //Get_FatFsInfo(res_flash);
    if(res_flash != FR_OK)
    {
        LOG(">>SD卡未挂载或挂载失败<<\r\n");
    }
    else
    {
        //LOG(">>读取剩余空间成功<<\r\n");
        sd_sizeof_usable = nclst * fatfs->csize;
        LOG("SD卡的可用容量大小··: %.0f 字节 %.0f kb %.0f Mb %.2f Gb\r\n",\
            sd_sizeof_usable,sd_sizeof_usable/1024,sd_sizeof_usable/1024/1024,sd_sizeof_usable/1024/1024/1024);
        sd_sizeof_used = sd_sizeof_all - sd_sizeof_usable;
        LOG("SD卡的已用容量大小··: %.0f 字节 %.0f kb %.0f Mb %.2f Gb\r\n",\
            sd_sizeof_used,sd_sizeof_used/1024,sd_sizeof_used/1024/1024,sd_sizeof_used/1024/1024/1024);
    }
			LOG("***************************************\r\n");
}
#endif

