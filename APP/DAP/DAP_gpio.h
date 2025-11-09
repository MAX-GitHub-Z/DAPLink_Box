/**
  ************************ Copyright *************** 
  *           (C) Copyright 2025,txt1994,China.
  *                  All Rights Reserved
  *
  *                 https://github.com/MAX-GitHub-Z/MCU_BootLoader
  *                 email:3014759318@qq.com
  *                 QQ   :3014759318
  *      
  * FileName     : DAP_gpio.h   
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

#ifndef __D_A_P_GPIO_H_
#define __D_A_P_GPIO_H_

#include <stdint.h>

 void DAP_gpio_init(void);


void DAP_gpio_deinit(void);



void DAP_PIN_SWDIO_OUT_ENABLE(void);

void DAP_PIN_SWDIO_OUT_DISABLE(void);
#endif

