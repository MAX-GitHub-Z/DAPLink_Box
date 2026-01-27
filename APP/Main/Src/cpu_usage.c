

#include "main.h"
#include "cpu_uasge.h"

#if configGENERATE_RUN_TIME_STATS
volatile configRUN_TIME_COUNTER_TYPE cpu_run_time=0;
TIM_HandleTypeDef        htim7;

void ConfigureTimerForRunTimeStats()
{
	cpu_run_time = 0;
//  HAL_StatusTypeDef     status = HAL_OK;

//  /* Enable TIM7 clock */
//  __HAL_RCC_TIM7_CLK_ENABLE();
//    /* Initialize TIM7 */
//  htim7.Instance = TIM7;

//  /* Initialize TIMx peripheral as follow:
//   * Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
//   * Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
//   * ClockDivision = 0
//   * Counter direction = Up
//   */
//  htim7.Init.Period = (1000-1);
//  htim7.Init.Prescaler = 71;
//  htim7.Init.ClockDivision = 0;
//  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

//  status = HAL_TIM_Base_Init(&htim7);
//  if (status == HAL_OK)
//  {
//    /* Start the TIM time Base generation in interrupt mode */
//    status = HAL_TIM_Base_Start_IT(&htim7);
//    if (status == HAL_OK)
//    {
//    /* Enable the TIM6 global Interrupt */
//        HAL_NVIC_EnableIRQ(TIM7_IRQn);
//        HAL_NVIC_SetPriority(TIM7_IRQn, 0x01, 0U);
//      /* Configure the SysTick IRQ priority */
//    }
//  }
}

//void TIM7_IRQHandler(void)
//{
//    cpu_run_time++;
//    HAL_TIM_IRQHandler(&htim7);
//}


configRUN_TIME_COUNTER_TYPE GetRunTimeCounterValue()
{
    return cpu_run_time;
}

int printCPUStats(uint8_t argc, char **argv) {
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;
    // 获取当前任务数量
    uxArraySize = uxTaskGetNumberOfTasks();
    // 分配内存存储任务状态
    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    if (pxTaskStatusArray != NULL) {
        // 获取任务状态信息
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
        // 归一化总运行时间（防止除零）
        if (ulTotalRunTime == 0) {
            ulTotalRunTime = 1;
        }
        printf("\r\n=== CPU Usage Statistics ===\r\n");
        printf("Task Name\t\tRun Time\t\t%%CPU\r\n");
        printf("---------\t\t--------\t\t----\r\n");
        // 打印每个任务的CPU使用率
        for (x = 0; x < uxArraySize; x++) {
            unsigned long ulTaskRunTime = pxTaskStatusArray[x].ulRunTimeCounter;
            float cpuUsage = (float)ulTaskRunTime * 100.0 / (float)ulTotalRunTime;
            
            printf("%-16s\t%lu\t\t\t%.2f%%\r\n", 
                       pxTaskStatusArray[x].pcTaskName,
                       ulTaskRunTime,
                       cpuUsage);
        }
        // 打印空闲任务信息（系统空闲率）
        for (x = 0; x < uxArraySize; x++) {
            if (strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE") == 0) {
                float idleUsage = (float)pxTaskStatusArray[x].ulRunTimeCounter * 100.0 / (float)ulTotalRunTime;
                printf("System Idle: %.2f%%\r\n", idleUsage);
                break;
            }
        }
        printf("Total Run Time: %lu\r\n", ulTotalRunTime);
        printf("=============================\r\n");
        vPortFree(pxTaskStatusArray);
    }
}
#endif
