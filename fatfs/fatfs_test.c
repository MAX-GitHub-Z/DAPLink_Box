/**
  ************************* Copyright ********************** 
  *
  *          (C) Copyright 2026,China.
  *                    All Rights Reserved
  *                              
  *                 https://github.com/MAX-GitHub-Z
  *                 email:3014759318@qq.com
  *                 QQ   :3014759318
  *    
  * FileName     : fatfs_test.c   
  * Version      : v1.0     
  * Author       : MAX-GitHub-Z         
  * Date         : 2026-01-17         
  * Description  :    
  * Function List:  
  * Toolchain : RealView Development Suite
  *             RealView Microcontroller Development Kit (MDK)
  *             ARM Developer Suite (ADS)
  *             Keil uVision
  **********************************************************
 */

#include "ff.h"			/* Basic definitions of FatFs */
#include "diskio.h"		/* Declarations FatFs MAI */

	FIL file;													/* 文件对象 */
	FRESULT f_res;                    /* 文件操作结果 */
	BYTE WriteBuffer[]= "欢迎使用DAPLink_Box 今天是个好日子，新建文件系统测试文件\r\n";
	UINT fnum;            					  /* 文件成功读写数量 */
	BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
	
#include <stdio.h>
	
	FRESULT Read_Dir();
	
uint8_t FatFS_Test()
{

	
	printf("开始测试SD卡的相关功能 \r\n");
	printf("****** 即将进行文件写入测试... ****** \r\n");
	f_res = f_open(&file, "FatFsReadWrite.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( f_res == FR_OK )
	{
		printf("》打开/创建FatFsReadWrite.txt文件成功，向文件写入数据。\r\n");
		/* 将指定存储区内容写入到文件内 */
		f_res=f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(f_res==FR_OK)
		{
			printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
			printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("！！文件写入失败：(%d)\r\n",f_res);
		}    
		/* 不再读写，关闭文件 */
		f_close(&file);
	}
	else
	{	
		printf("！！打开/创建文件失败。\r\n");
	}
	
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    f_res = f_open(&file, "FatFsReadWrite.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(f_res == FR_OK)
    {
      printf("》打开文件成功。\r\n");
      f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum); 
      if(f_res==FR_OK)
      {
        printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
        printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
      }
      else
      {
        printf("！！文件读取失败：(%d)\r\n",f_res);
      }		
    }
    else
    {
      printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&file);
		Read_Dir();
		return 1;
}

FSIZE_t f_sizeForFile(char *FileName)
{
    FSIZE_t res;
    f_res = f_open(&file, FileName, FA_OPEN_EXISTING | FA_READ);
    if(f_res == FR_OK)
    {
        if(file.obj.sclust !=0)
        {
            res =  file.obj.objsize;
        }
        else
        {
            res = 0;
        }
    }
    else
    {
        res =  0;
    }
    f_close(&file);
    return res;
}

FRESULT Read_Dir()
{
    FRESULT res;
    DIR dir;
    FILINFO fno;
    FSIZE_t fileSize=0;
    TCHAR dir_cwd[64]={0};
    // 打开当前目录
    res = f_opendir(&dir, ".");
    if (res != FR_OK) {
        printf("打开目录失败: %d\r\n", res);
        f_mount(NULL, "", 0);
        return res;
    }
    res = f_getcwd(dir_cwd,sizeof(dir_cwd));
		if(res == FR_OK)
		{
			printf("当前目录路径:%s \r\n",dir_cwd);
		}
    printf("当前目录内容:\r\n");
    printf("%-12s %-8s %s\r\n", "属性", "大小", "名称");
    printf("------------ -------- --------------------\r\n");
    
    // 遍历目录项
    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) {
            break;  // 错误或遍历结束
        }
        
        // 跳过空条目
        if (fno.fname[0] == '.') {
            continue;
        }
        
        // 显示文件属性
        printf("%c%c%c%c%c%c%c%c%c%c ",
               (fno.fattrib & AM_DIR)  ? 'D' : '-',  // 目录
               (fno.fattrib & AM_RDO)  ? 'R' : '-',  // 只读
               (fno.fattrib & AM_HID)  ? 'H' : '-',  // 隐藏
               (fno.fattrib & AM_SYS)  ? 'S' : '-',  // 系统
               (fno.fattrib & AM_ARC)  ? 'A' : '-',  // 归档
               ' ', ' ', ' ', ' ', ' ');
        
        // 显示文件大小（目录显示<DIR>）
        if (fno.fattrib & AM_DIR) {
            printf("%-8s", "<DIR>");
        } else {
					fileSize  = f_sizeForFile(fno.fname);
            printf("%8lu", fileSize);
        }
        
        // 显示文件名
        printf(" %s\r\n", fno.fname);
    }
    
    // 关闭目录和卸载文件系统
    f_closedir(&dir);
    return res;
}
