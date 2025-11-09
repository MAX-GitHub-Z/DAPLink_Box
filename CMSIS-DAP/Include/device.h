/**
  ************************ Copyright *************** 
  *           (C) Copyright 2025,txt1994,China.
  *                  All Rights Reserved
  *
  *                 https://github.com/MAX-GitHub-Z/
  *                 email:3014759318@qq.com
  *                 QQ   :3014759318
  *      
  * FileName     : device.h   
  * Version      : v1.0     
  * Author       : MAX-GitHub-Z         
  * Date         : 2025-11-08         
  * Description  :    
  * Function List:  
  * Toolchain : RealView Development Suite
  *             RealView Microcontroller Development Kit (MDK)
  *             ARM Developer Suite (ADS)
  *             Keil uVision
  ******************************************************
**/

#ifndef __DEVICE_H_
#define __DEVICE_H_


/*后续可以在这里新增 对应的芯片平台*/
#if defined (STM32F1) && defined (USE_HAL_DRIVER)
#include "stm32f1xx_hal.h"
#else
#error "Please link the firmware library or the HAL library of the chip. "
#endif




#endif /*__DEVICE_H_*/

