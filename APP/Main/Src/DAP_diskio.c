


#include "stm32f1xx_hal.h"
#include "flash_blob.h"
#include "error.h"
__weak uint8_t swd_flash_syscall_exec(const program_syscall_t *sysCallParam,\
														uint32_t entry, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_TxMailbox2CompleteCallback could be implemented in the
            user file
   */
}



algo_info_t STM32_ALGO[6];
/*此处实现从fatfs中读取相关的参数 并进行加载*/
