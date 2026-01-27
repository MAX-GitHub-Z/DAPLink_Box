/*
 * *****************************************************************************
 * MIT License
 * 
 * Copyright (C) 2025 Ji Youzhou. or its affiliates.  All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************************/

#include "time.h"
#include "stdint.h"
#include "nr_micro_shell_port.h"
#include "nr_micro_shell.h"
#include "stm32f1xx_hal.h"
#include "tusb.h"
FILE *dbug_log;
FILE *key_rec_log;

uint8_t shell_id =0;
uint8_t shell_out =0;
uint64_t get_sys_timestamp_ns(void)
{
	return HAL_GetTick();
}

void nr_shell_debug_log_init(void)
{
//	dbug_log = fopen("debug.log", "w");
//	if (!dbug_log)
//		printf("failed to open debug log\n");
//	setvbuf(dbug_log, NULL, _IONBF, 0);
//	key_rec_log = fopen("key_rec.log", "w");
//	if (!key_rec_log)
//		printf("failed to open key rec log\n");
//	setvbuf(key_rec_log, NULL, _IONBF, 0);
}

void shell_output_str(uint8_t *buff,uint16_t len)
{
		for(uint16_t i=0;i<len;i++)
		{
				shell_output(buff[i]);
		}
}
void shell_output(uint8_t ch)
{
	uint16_t ret=0;
	if(shell_out == 0)
	{
			putchar(ch);
	}
	else
	{
		tud_cdc_n_write(shell_id, &ch, 1);
		tud_cdc_n_write_flush(shell_id);
		while(1)
		{
			tud_task();
			ret = tud_cdc_n_write_available(shell_id);
			if(0!= ret)
			{
				break;
			}
		}
	}
}

void set_shell_output(uint8_t id)
{
	if(id == 0)
	{
			shell_out = 0;
	}
	else{shell_out =1;}
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {

	if (itf == 0) 
	{
		// CDC接口0状态变化
		if (dtr) 
		{
		// 终端已连接，初始化UART
				set_shell_output(1);
				shell_id = itf;
			shell_init_log();
		} 
		else 
		{
		// 终端已断开，关闭UART

		}
	}
	else if(itf == 1)
	{
		if(dtr)
		{
			set_shell_output(1);
			shell_id = itf;
			shell_init_log();
		}
	}

}

static void shell_tack(void *param) ;
#define STACK_SIZE 256
StaticTask_t xTaskShellBuffer;
StackType_t xStackShell[ STACK_SIZE ];
TaskHandle_t xTaskHandleShell;
void shell_tack_init(void)
{
    xTaskHandleShell = xTaskCreateStatic(shell_tack, "shell", STACK_SIZE, NULL, 2, xStackShell, &xTaskShellBuffer);
}

extern uint8_t uart_rx_tmp[];/*接收的备份数据*/
extern uint16_t rx_buffer_rx_tmp_len;
static void shell_tack(void *param) 
{
  (void) param;
    uint32_t ulNotifiedValue;
    BaseType_t xResult;
    
    shell_init();
    shell_init_log();
    while(1)
    {
                // 等待通知（阻塞等待）
        xResult = xTaskNotifyWait(0x00,         // 进入前清除所有位
                                 0xffffffffU,     // 退出时清除所有位
                                 &ulNotifiedValue, 
                                 portMAX_DELAY);
        if(xResult == pdPASS)
        {
            // 收到通知，任务就绪并执行处理
            for(uint16_t i=0;i<rx_buffer_rx_tmp_len;i++)
            {
                shell(uart_rx_tmp[i]);
            }
            //printf("shell \r\n");  // 你的任务处理函数
            // 处理完成后，任务再次进入等待状态（挂起）
        }
        
    }
}


/******************* (C) COPYRIGHT 2025 Ji Youzhou ****************************/
