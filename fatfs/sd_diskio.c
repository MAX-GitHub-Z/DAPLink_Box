/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @author  MCD Application Team
  * @version V1.4.1
  * @date    14-February-2017
  * @brief   SD Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "sd_diskio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */
  
const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read, 
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used 
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  
  /* Configure the uSD device */
  if(BSP_SD_Init() == MSD_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE lun)
{
  Stat = STA_NOINIT;

  if(BSP_SD_GetCardState() == MSD_OK)
  {
    Stat &= ~STA_NOINIT;
  }
  
  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
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

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
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
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
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
#endif /* _USE_IOCTL == 1 */

#if defined( SD_INFOPRINT)
#include <stdio.h>
void Get_FatFsInfo(FRESULT ret)
{
		uint16_t ret_data = (uint16_t)ret;
		printf("return %d \r\n",ret_data);
		switch(ret_data)
		{
			case FR_OK:
				printf("操作成功 \r\n");
				break;
			case FR_DISK_ERR:
				printf("在低级磁盘 I/O 层发生了严重错误 \r\n");
				break;
			case FR_INT_ERR:
				printf("断言失败 \r\n");
				break;
			case FR_NOT_READY:
				printf("物理驱动器无法工作 \r\n");
				break;
			case FR_NO_FILE:
				printf("找不到该文件 \r\n");
				break;
			case FR_NO_PATH:
				printf("找不到该路径 \r\n");
				break;
			case FR_INVALID_NAME:
				printf("路径名称格式无效 \r\n");
				break;
			case FR_DENIED:
				printf("由于禁止访问或目录已满而被拒绝访问 \r\n");
				break;
			case FR_EXIST:
				printf("由于禁止访问而被拒绝访问\r\n");
				break;
			case FR_INVALID_OBJECT:
				printf("文件/目录对象无效 \r\n");
				break;
			case FR_WRITE_PROTECTED:
				printf("物理驱动器已写保护 \r\n");
				break;
			case FR_INVALID_DRIVE:
				printf("逻辑驱动器编号无效 \r\n");
				break;
			case FR_NOT_ENABLED:
				printf("该卷没有工作区域 \r\n");
				break;
			case FR_NO_FILESYSTEM:
				printf("找不到有效的 FAT 卷  \r\n");
				break;
			case FR_MKFS_ABORTED:
				printf("由于某些问题，f_mkfs 函数被中止  \r\n");
				break;
			case FR_TIMEOUT:
				printf("在规定时间内无法控制该卷 \r\n");
				break;
			case FR_LOCKED:
				printf("根据文件共享策略拒绝执行操作 \r\n");
				break;
			case FR_NOT_ENOUGH_CORE:
				printf("LFN 工作缓冲区无法分配，因为给定的缓冲区大小不足或者路径过长 \r\n");
				break;
			case FR_TOO_MANY_OPEN_FILES:
				printf("打开文件的数量超过 FF_FS_LOCK 的限制 \r\n");
				break;
			case FR_INVALID_PARAMETER:
				printf("给定的参数无效 \r\n");
				break;
			default:
				printf("异常的返回值%d \r\n",ret_data);
				break;
		}
}
extern FATFS SDFatFs;
void GET_SDInfo(void)
{
	BSP_SD_CardInfo CardInfo;
	BSP_SD_GetCardInfo(&CardInfo);
	
	FRESULT res_flash=FR_OK;
	FATFS *pfs=NULL;
	DWORD fre_clust, fre_sect, tot_sect; // 剩余簇数、剩余扇区数、总扇区数
	
	uint64_t CardInfodata;
	float sd_sizeof=0;
	printf("***************************************\r\n");
	CardInfodata= CardInfo.CardType;
	printf("SD卡的类型: %llu ",CardInfodata);
	switch(CardInfodata)
	{
		case CARD_SDSC:
			printf("SC 标准SD卡 \r\n");
			break;
		case CARD_SDHC_SDXC:
			printf("高容量SD卡（SDHC，2GB~32GB）或扩展容量SD卡（SDXC，32GB~2TB）\r\n");
			break;
		case CARD_SECURED:
			printf("CURED 安全数字卡 \r\n");
			break;
		default:
			printf("未定义的SD卡类型\r\n");
			break;
	}
	CardInfodata= CardInfo.CardVersion;
	printf("SD卡的版本: %llu ",CardInfodata);
	switch(CardInfodata)
	{
		case CARD_V1_X:
			printf("V1.XX \r\n");
			break;
		case CARD_V2_X:
			printf("V2.XX \r\n");
			break;
		default:
			printf("未定义的SD卡版本\r\n");
			break;
	}
	CardInfodata= CardInfo.Class;
	printf("SD卡的类型··········: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.RelCardAdd;
	printf("SD卡的相对地址······: %llx \r\n",CardInfodata);
	CardInfodata= CardInfo.BlockNbr;
	printf("SD卡的总扇区数大小··: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.BlockSize;
	printf("SD卡的单个扇区大小··: %llu 字节\r\n",CardInfodata);
	CardInfodata= CardInfo.LogBlockNbr;
	printf("SD卡的逻辑扇区数····: %llu \r\n",CardInfodata);
	CardInfodata= CardInfo.LogBlockSize;
	printf("SD卡的单个逻辑扇大小: %llu 字节\r\n",CardInfodata);
	sd_sizeof = CardInfo.BlockNbr*1.0*CardInfo.BlockSize;
	printf("SD卡的总容量大小····: %.0f 字节 %.0f kb %.0f Mb %.2f Gb\r\n",\
		sd_sizeof,sd_sizeof/1024,sd_sizeof/1024/1024,sd_sizeof/1024/1024/1024);
	printf("***************************************\r\n");
	
//	pfs = &SDFatFs;
//	res_flash = f_getfree("/", &fre_clust, &pfs); // 函数参数：驱动器号、剩余簇数指针、文件系统对象指针
//	Get_FatFsInfo(res_flash);
//  // 计算得到总扇区个数和空簇大小
//  tot_sect = (pfs->n_fatent-2)*pfs->csize;
//  fre_sect = fre_clust * pfs->csize;
//  printf("设备可用总空间：%10lu KB \n 设备剩余空间：%10lu KB \n", tot_sect*4, fre_sect*4);
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

