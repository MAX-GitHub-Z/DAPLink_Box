/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "board_api.h"
#include "tusb.h"
#include "diskio.h"	

#if CFG_TUD_MSC

// whether host does safe-eject
static bool ejected = false;

// Some MCU doesn't have enough 8KB SRAM to store the whole disk
// We will use Flash as read-only disk with board that has
// CFG_EXAMPLE_MSC_READONLY defined

#define README_CONTENTS \
"This is tinyusb's MassStorage Class demo.\r\n\r\n\
If you find any bugs or get any questions, feel free to file an\r\n\
issue at github.com/hathach/tinyusb"

enum {
  DISK_BLOCK_NUM = 16,// 8KB is the smallest size that windows allow to mount
  DISK_BLOCK_SIZE = 512
};

static
#ifdef CFG_EXAMPLE_MSC_READONLY
const
#endif
//------------- 第0块：引导扇区 -------------//
// 每扇区字节数 = 磁盘扇区大小；16 位 FAT 扇区数量 = 磁盘扇区总数；
// 每簇扇区数 = 1；保留扇区数 = 1；
// FAT 数量 = 1；16 位 FAT 根目录项数量 = 16；
// 每个 FAT 扇区数 = 1；每个磁道扇区数 = 1；磁头数量 = 1；隐藏扇区数 = 0；
// 驱动器编号 = 0x80；介质类型 = 0xf8；扩展引导签名 = 0x29；
// 文件系统类型 = "FAT12   "；卷序列号 = 0x1234；卷标签 = "TinyUSB MSC"；
// 位于偏移量 510 - 511 的 FAT 魔法代码
uint8_t msc_disk[DISK_BLOCK_NUM][DISK_BLOCK_SIZE] = {
  //------------- Block0: Boot Sector -------------//
  // byte_per_sector    = DISK_BLOCK_SIZE; fat12_sector_num_16  = DISK_BLOCK_NUM;
  // sector_per_cluster = 1; reserved_sectors = 1;
  // fat_num            = 1; fat12_root_entry_num = 16;
  // sector_per_fat     = 1; sector_per_track = 1; head_num = 1; hidden_sectors = 0;
  // drive_number       = 0x80; media_type = 0xf8; extended_boot_signature = 0x29;
  // filesystem_type    = "FAT12   "; volume_serial_number = 0x1234; volume_label = "TinyUSB MSC";
  // FAT magic code at offset 510-511
  {
      0xEB, 0x3C, 0x90, 0x4D, 0x53, 0x44, 0x4F, 0x53, 0x35, 0x2E, 0x30, 0x00, 0x02, 0x01, 0x01, 0x00,
      0x01, 0x10, 0x00, 0x10, 0x00, 0xF8, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x29, 0x34, 0x12, 0x00, 0x00, 'T', 'i', 'n', 'y', 'U',
      'S', 'B', ' ', 'M', 'S', 'C', 0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20, 0x00, 0x00,

      // Zero up to 2 last bytes of FAT magic code
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
  },

  //------------- Block1: FAT12 Table -------------//
  {
      0xF8, 0xFF, 0xFF, 0xFF, 0x0F// first 2 entries must be F8FF, third entry is cluster end of readme file
  },

  //------------- Block2: Root Directory -------------//
  {
      // first entry is volume label
      'T', 'i', 'n', 'y', 'U', 'S', 'B', ' ', 'M', 'S', 'C', 0x08, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x6D, 0x65, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      // second entry is readme file
      'R', 'E', 'A', 'D', 'M', 'E', ' ', ' ', 'T', 'X', 'T', 0x20, 0x00, 0xC6, 0x52, 0x6D,
      0x65, 0x43, 0x65, 0x43, 0x00, 0x00, 0x88, 0x6D, 0x65, 0x43, 0x02, 0x00,
      sizeof(README_CONTENTS) - 1, 0x00, 0x00, 0x00// readme's files size (4 Bytes)
  },

  //------------- Block3: Readme Content -------------//
  {README_CONTENTS}
};

static int memset_32bit(void* s, int c, uint32_t _size);
// Invoked when received SCSI_CMD_INQUIRY, v2 with full inquiry response
// Some inquiry_resp's fields are already filled with default values, application can update them
// Return length of inquiry response, typically sizeof(scsi_inquiry_resp_t) (36 bytes), can be longer if included vendor data.
uint32_t tud_msc_inquiry2_cb(uint8_t lun, scsi_inquiry_resp_t *inquiry_resp, uint32_t bufsize) {
  (void) lun;
  (void) bufsize;
  const char vid[] = "TinyUSB";
  const char pid[] = "Mass Storage";
  const char rev[] = "1.0";

  (void) strncpy((char*) inquiry_resp->vendor_id, vid, 8);
  (void) strncpy((char*) inquiry_resp->product_id, pid, 16);
  (void) strncpy((char*) inquiry_resp->product_rev, rev, 4);

  return sizeof(scsi_inquiry_resp_t); // 36 bytes
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
  (void) lun;

  // RAM disk is ready until ejected
  if (ejected) {
    // Additional Sense 3A-00 is NOT_FOUND
    return tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
  }

  return true;
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
	(void) lun;

	int ret;
	uint32_t LogBlockNbr=0,LogBlocksize=0;
	disk_ioctl(0,GET_SECTOR_COUNT,&LogBlockNbr);
	disk_ioctl(0,GET_SECTOR_SIZE,&LogBlocksize);
	printf("capacity_cb n=%d z=%d\r\n",LogBlockNbr,LogBlocksize);
		*block_count = LogBlockNbr;
		*block_size  = LogBlocksize;	
	//printf("tud_msc_capacity_cb n=%d s=%d \r\n",LogBlockNbr,LogBlocksize);
		return ;

}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
  (void) lun;
  (void) power_condition;

  if (load_eject) {
    if (start) {
      // load disk storage
    } else {
      // unload disk storage
      ejected = true;
    }
  }

  return true;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
	(void) lun;

	uint8_t ret;
	uint32_t LogBlockNbr=0,LogBlocksize=0;
	disk_ioctl(0,GET_SECTOR_COUNT,&LogBlockNbr);
	disk_ioctl(0,GET_SECTOR_SIZE,&LogBlocksize);
//	if(lun == SDDISK)  
	{

		if(disk_get_initstatus() == 1) 
			{
			
			// 检测 sd 卡是否插入  。。？
			if(LogBlockNbr == 0 || LogBlocksize == 0) 
			{
				return -1;
			}
			// out of ramdisk
			if (lba >=  LogBlockNbr) {
				return -1;
			}

			// Check for overflow of offset + bufsize
			if (offset + bufsize >  LogBlocksize) {
				return -1;
			}
			//extern DRESULT sd_disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
			//			volatile uint32_t addr = (lba * 512)+offset;   这个是错误的， 因为 sd 底层会 *512
			volatile uint32_t addr = lba + offset;    
			ret = disk_read( 0, (uint8_t *)buffer, 
								addr,                                			// 给出 块的 索引即可
								bufsize/LogBlocksize			// 需要读取 块的大小, 一块为单位
							  );			
			
			if(ret == RES_OK) 
			{
				printf("read10_cb n=%d z=%d\r\n",LogBlockNbr,LogBlocksize);
				return bufsize;
			}
			else
			{
				printf("read10_cberr n=%d z=%d\r\n",LogBlockNbr,LogBlocksize);
			}
		}
	}
	return -1;
}

bool tud_msc_is_writable_cb(uint8_t lun) {
  (void) lun;

  #ifdef CFG_EXAMPLE_MSC_READONLY
  return false;
  #else
  return true;
  #endif
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
  (void) lun;
	
	uint8_t ret;
	uint32_t LogBlockNbr=0,LogBlocksize=0;
	disk_ioctl(0,GET_SECTOR_COUNT,&LogBlockNbr);
	disk_ioctl(0,GET_SECTOR_SIZE,&LogBlocksize);
//	if(lun == SDDISK)  
	{

		if(disk_get_initstatus() == 1)   
			{
			// 检测 sd 卡是否插入  。。？
			if(LogBlockNbr == 0 || LogBlocksize == 0) 
			{
				return -1;
			}
			disk_ioctl(0,GET_SECTOR_COUNT,&LogBlockNbr);
			disk_ioctl(0,GET_SECTOR_SIZE,&LogBlocksize);
			if (lba >=  LogBlockNbr) {
				return -1;
			}
			if(LogBlocksize == 0)
			{
				return -1;
			}
			
			//extern DRESULT sd_disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
			//(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
			volatile uint32_t addr = lba + offset;
			ret = disk_write(0, (const BYTE*) buffer,  addr,  bufsize/LogBlocksize);
			if(ret == RES_OK) 
			{
				printf("write10_cb n=%d z=%d\r\n",LogBlockNbr,LogBlocksize);
				return bufsize;
			}
			else
			{
				printf("write10_cberr n=%d z=%d\r\n",LogBlockNbr,LogBlocksize);
			}
		}
	}
	return -1;	
}

// Callback invoked when received an SCSI command not in built-in list below
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
  void const* response = NULL;
  int32_t resplen = 0;

  // most scsi handled is input
  bool in_xfer = true;

  switch (scsi_cmd[0])
  {
    default:
      // Set Sense = Invalid Command Operation
      tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

      // negative means error -> tinyusb could stall and/or response with failed status
      resplen = -1;
    break;
  }

  // return resplen must not larger than bufsize
  if ( resplen > bufsize ) resplen = bufsize;

  if ( response && (resplen > 0) )
  {
    if(in_xfer)
    {
      memcpy(buffer, response, (size_t) resplen);
    }else
    {
      // SCSI output
    }
  }

  return (int32_t) resplen;
}

static int memset_32bit(void* s, int c, uint32_t _size)
{
	if((_size % 4) != 0)  return -1;
	
    volatile unsigned int * p = (volatile unsigned int *) s;
	_size /= 4;
    while (_size > 0) {
		 *p++ = (unsigned char) c;
		 --_size;
    }
	return 0;
}
#endif
