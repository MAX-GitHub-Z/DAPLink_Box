/**
  ************************* Copyright ********************** 
  *
  *          (C) Copyright 2025,China.
  *                    All Rights Reserved
  *                              
  *                 https://github.com/MAX-GitHub-Z/MCU_BootLoader
  *                 email:3014759318@qq.com
  *                 QQ   :3014759318
  *    
  * FileName     : DAP_gpio.c   
  * Version      : v1.0     
  * Author       : MAX-GitHub-Z         
  * Date         : 2025-11-08         
  * Description  :    
  * Function List:  
  * Toolchain : RealView Development Suite
  *             RealView Microcontroller Development Kit (MDK)
  *             ARM Developer Suite (ADS)
  *             Keil uVision
  **********************************************************
 */

#include "DAP_gpio.h"

#include "DAP_config.h"
void DAP_gpio_init(void)
{
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = PIN_SWDIO_TMS_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWDIO_TMS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_SWCLK_TCK_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWCLK_TCK_PORT, &GPIO_InitStruct);
	/*默认输出设置为高电平*/
	HAL_GPIO_WritePin(PIN_SWDIO_TMS_PORT,PIN_SWDIO_TMS_MASK,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PIN_SWCLK_TCK_PORT,PIN_SWCLK_TCK_MASK,GPIO_PIN_SET);
	#if defined(DAP_JTAG)
	
	
	GPIO_InitStruct.Pin = PIN_TDI_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_TDI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_TDO_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_TDO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_nRESET_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_nRESET_PORT, &GPIO_InitStruct);
	
		/*默认输出设置为高电平*/
	HAL_GPIO_WritePin(PIN_TDI_PORT,PIN_TDI_MASK,GPIO_PIN_SET);
	HAL_GPIO_WritePin(PIN_TDO_PORT,PIN_TDO_MASK,GPIO_PIN_SET);
	
	#endif
	
}


void DAP_gpio_deinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = PIN_SWDIO_TMS_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWDIO_TMS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_SWCLK_TCK_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWCLK_TCK_PORT, &GPIO_InitStruct);
	#if defined(DAP_JTAG)
	
	
	GPIO_InitStruct.Pin = PIN_TDI_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_TDI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_TDO_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_TDO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_nRESET_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_nRESET_PORT, &GPIO_InitStruct);
	
	
	#endif
}

void DAP_PIN_SWDIO_OUT_ENABLE(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = PIN_SWDIO_TMS_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWDIO_TMS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(PIN_SWDIO_TMS_PORT,PIN_SWDIO_TMS_MASK,GPIO_PIN_SET);
}

void DAP_PIN_SWDIO_OUT_DISABLE(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = PIN_SWDIO_TMS_MASK;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PIN_SWDIO_TMS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(PIN_SWDIO_TMS_PORT,PIN_SWDIO_TMS_MASK,GPIO_PIN_SET);
}

