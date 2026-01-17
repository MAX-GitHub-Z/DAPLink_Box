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
uint8_t FatFS_Test()
{

	
	printf("开始测试SD卡的相关功能 \r\n");
	printf("****** 即将进行文件写入测试... ****** \r\n");
	f_res = f_open(&file, "FatFsReadWrite.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( f_res == FR_OK )
	{
		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
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
		return 1;
}