/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*******************************************************************************
 * This file provides an example FreeRTOSConfig.h header file, inclusive of an
 * abbreviated explanation of each configuration item.  Online and reference
 * documentation provides more information.
 * https://www.freertos.org/a00110.html
 *
 * Constant values enclosed in square brackets ('[' and ']') must be completed
 * before this file will build.
 *
 * Use the FreeRTOSConfig.h supplied with the RTOS port in use rather than this
 * generic file, if one is available.
 ******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H



/* 头文件 */
#include "stm32f1xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/******************************************************************************/
/* Hardware description related definitions. **********************************/
/******************************************************************************/

/* In most cases, configCPU_CLOCK_HZ must be set to the frequency of the clock
 * that drives the peripheral used to generate the kernels periodic tick interrupt.
 * The default value is set to 20MHz and matches the QEMU demo settings.  Your
 * application will certainly need a different value so set this correctly.
 * This is very often, but not always, equal to the main system clock frequency. */
/* 在大多数情况下，configCPU_CLOCK_HZ 必须设置为驱动用于生成内核周期性时钟中断的外设的时钟频率。
* 默认值设为 20MHz，与 QEMU 演示设置相匹配。您的
* 应用程序肯定需要一个不同的数值，所以请正确设置这个数值。
* 这通常（但并非总是）与主系统时钟频率相等。*/
#define configCPU_CLOCK_HZ    ( ( unsigned long ) 72000000 )

/* configSYSTICK_CLOCK_HZ is an optional parameter for ARM Cortex-M ports only.
 *
 * By default ARM Cortex-M ports generate the RTOS tick interrupt from the
 * Cortex-M SysTick timer. Most Cortex-M MCUs run the SysTick timer at the same
 * frequency as the MCU itself - when that is the case configSYSTICK_CLOCK_HZ is
 * not needed and should be left undefined. If the SysTick timer is clocked at a
 * different frequency to the MCU core then set configCPU_CLOCK_HZ to the MCU clock
 * frequency, as normal, and configSYSTICK_CLOCK_HZ to the SysTick clock
 * frequency.  Not used if left undefined.
 * The default value is undefined (commented out).  If you need this value bring it
 * back and set it to a suitable value. */
/* configSYSTICK_CLOCK_HZ 是仅适用于 ARM Cortex-M 端口的一个可选参数。*
* 默认情况下，ARM Cortex-M 端口会从 Cortex-M 的 SysTick 定时器生成实时操作系统（RTOS）的计时器中断。
大多数 Cortex-M 单元以与单片机自身相同的频率运行 SysTick 定时器——在这种情况下，
无需使用 configSYSTICK_CLOCK_HZ，并且应将其留空。如果 SysTick 定时器的时钟频率与单片机核心不同，
则应按照常规设置 configCPU_CLOCK_HZ 为单片机的时钟频率，并将 configSYSTICK_CLOCK_HZ 设置为 SysTick 的时钟频率。
若未定义则不使用。
* 默认值为未定义（已注释掉）。如果您需要此值，请将其恢复并设置为合适的值。*/

/*
#define configSYSTICK_CLOCK_HZ		( ( TickType_t ) 1000)// 时钟节拍1ms（SysTick中断频率）
*/

/******************************************************************************/
/* Scheduling behaviour related definitions. **********************************/
/******************************************************************************/

/* configTICK_RATE_HZ sets frequency of the tick interrupt in Hz, normally
 * calculated from the configCPU_CLOCK_HZ value. */
 /* configTICK_RATE_HZ 用于设置时钟中断的频率（以赫兹为单位），通常该频率是根据 configCPU_CLOCK_HZ 值计算得出的。*/
#define configTICK_RATE_HZ                         ( ( TickType_t ) 1000)// 时钟节拍1ms（SysTick中断频率）

/* Set configUSE_PREEMPTION to 1 to use pre-emptive scheduling.  Set
 * configUSE_PREEMPTION to 0 to use co-operative scheduling.
 * See https://www.freertos.org/single-core-amp-smp-rtos-scheduling.html. */
/* 将 configUSE_PREEMPTION 设置为 1 可启用抢占式调度。设置
* 将 configUSE_PREEMPTION 设置为 0 以采用协作式调度方式。
* 详情请参阅 https://www.freertos.org/single-core-amp-smp-rtos-scheduling.html 。*/
#define configUSE_PREEMPTION                       1

/* Set configUSE_TIME_SLICING to 1 to have the scheduler switch between Ready
 * state tasks of equal priority on every tick interrupt.  Set
 * configUSE_TIME_SLICING to 0 to prevent the scheduler switching between Ready
 * state tasks just because there was a tick interrupt.  See
 * https://freertos.org/single-core-amp-smp-rtos-scheduling.html. */
 /* 将 configUSE_TIME_SLICING 设置为 1，即可使调度器在每次中断时在具有相同优先级的就绪状态任务之间进行切换。设置
* 将 configUSE_TIME_SLICING 设置为 0，以防止调度器仅仅因为有时钟中断而切换处于就绪状态的任务。见
* https://freertos.org/单核扩展-对称多处理-实时操作系统调度.html*/
#define configUSE_TIME_SLICING                     0

/* Set configUSE_PORT_OPTIMISED_TASK_SELECTION to 1 to select the next task to
 * run using an algorithm optimised to the instruction set of the target hardware -
 * normally using a count leading zeros assembly instruction.  Set to 0 to select
 * the next task to run using a generic C algorithm that works for all FreeRTOS
 * ports.  Not all FreeRTOS ports have this option.  Defaults to 0 if left
 * undefined. */
 /* 将 configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为 1，
即可使用针对目标硬件指令集优化的算法来选择下一项要运行的任务——通常会使用一个包含前导零的计数汇编指令。
将该值设置为 0 则会使用适用于所有 FreeRTOS 端口的通用 C 算法来选择下一项要运行的任务。
并非所有 FreeRTOS 端口都具备此选项。如果未定义则默认值为 0。 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0

/* Set configUSE_TICKLESS_IDLE to 1 to use the low power tickless mode.  Set to
 * 0 to keep the tick interrupt running at all times.  Not all FreeRTOS ports
 * support tickless mode. See https://www.freertos.org/low-power-tickless-rtos.html
 * Defaults to 0 if left undefined. */
 /* 将 configUSE_TICKLESS_IDLE 设置为 1 可启用低功耗无中断模式。
 将其设置为 0 则保持计时器中断始终运行。并非所有 FreeRTOS 端口都支持无中断模式。
 详情请参阅 https://www.freertos.org/low-power-tickless-rtos.html  。如果未定义则默认值为 0 。*/
#define configUSE_TICKLESS_IDLE                    0

/* configMAX_PRIORITIES Sets the number of available task priorities.  Tasks can
 * be assigned priorities of 0 to (configMAX_PRIORITIES - 1).  Zero is the lowest
 * priority. */
 /* configMAX_PRIORITIES 用于设置可用的任务优先级数量。
 任务可被赋予 0 到 (configMAX_PRIORITIES - 1) 之间的优先级值。其中 0 为最低优先级。*/
#define configMAX_PRIORITIES                       10

/* configMINIMAL_STACK_SIZE defines the size of the stack used by the Idle task
 * (in words, not in bytes!).  The kernel does not use this constant for any other
 * purpose.  Demo applications use the constant to make the demos somewhat portable
 * across hardware architectures. */
 /* configMINIMAL_STACK_SIZE 定义了空闲任务所使用的栈的大小
* （以字节为单位，而非字为单位！）。内核不会将此常量用于任何其他用途。
演示应用程序使用该常量以使演示程序在不同硬件架构之间更具可移植性。*/
#define configMINIMAL_STACK_SIZE                   1024

/* configMAX_TASK_NAME_LEN sets the maximum length (in characters) of a task's
 * human readable name.  Includes the NULL terminator. */

 /* configMAX_TASK_NAME_LEN 用于设置任务的可读名称的最大长度（以字符为单位）。 该值包含空终止符。*/
#define configMAX_TASK_NAME_LEN                    16

/* Time is measured in 'ticks' - which is the number of times the tick interrupt
 * has executed since the RTOS kernel was started.
 * The tick count is held in a variable of type TickType_t.
 *
 * configTICK_TYPE_WIDTH_IN_BITS controls the type (and therefore bit-width) of TickType_t:
 *
 * Defining configTICK_TYPE_WIDTH_IN_BITS as TICK_TYPE_WIDTH_16_BITS causes
 * TickType_t to be defined (typedef'ed) as an unsigned 16-bit type.
 *
 * Defining configTICK_TYPE_WIDTH_IN_BITS as TICK_TYPE_WIDTH_32_BITS causes
 * TickType_t to be defined (typedef'ed) as an unsigned 32-bit type.
 *
 * Defining configTICK_TYPE_WIDTH_IN_BITS as TICK_TYPE_WIDTH_64_BITS causes
 * TickType_t to be defined (typedef'ed) as an unsigned 64-bit type. */
 
 /* 时间是以“计时单位”来计量的——即自实时操作系统内核启动以来，计时中断执行的次数。
* 计时计数值保存在类型为 TickType_t 的变量中。*
* “configTICK_TYPE_WIDTH_IN_BITS” 控制着“TickType_t”类型的设定（以及相应的位宽）：*
* 将 configTICK_TYPE_WIDTH_IN_BITS 定义为 TICK_TYPE_WIDTH_16_BITS 会导致
* TickType_t 被定义（通过类型定义的方式）为一个无符号的 16 位类型。*
* 将 configTICK_TYPE_WIDTH_IN_BITS 定义为 TICK_TYPE_WIDTH_32_BITS 会导致
* TickType_t 被定义（通过类型定义的方式）为一个无符号的 32 位类型。*
* 将 configTICK_TYPE_WIDTH_IN_BITS 定义为 TICK_TYPE_WIDTH_64_BITS 会导致
* TickType_t 被定义（通过类型定义的方式）为一个无符号的 64 位类型。*/
#define configTICK_TYPE_WIDTH_IN_BITS              TICK_TYPE_WIDTH_32_BITS

/* Set configIDLE_SHOULD_YIELD to 1 to have the Idle task yield to an
 * application task if there is an Idle priority (priority 0) application task that
 * can run.  Set to 0 to have the Idle task use all of its timeslice.  Default to 1
 * if left undefined. */
 /* 将 configIDLE_SHOULD_YIELD 设置为 1，即可使空闲任务在存在具有空闲优先级（优先级为 0）的应用任务能够运行的情况下，
 向该应用任务让出执行时间。将该值设置为 0 则意味着空闲任务将使用其全部的时间片。如果未定义该值，则默认设置为 1。*/
#define configIDLE_SHOULD_YIELD                    1

/* Each task has an array of task notifications.
 * configTASK_NOTIFICATION_ARRAY_ENTRIES sets the number of indexes in the array.
 * See https://www.freertos.org/RTOS-task-notifications.html  Defaults to 1 if
 * left undefined. */
 /* 每个任务都有一个任务通知数组。
* configTASK_NOTIFICATION_ARRAY_ENTRIES 用于设置数组中的索引数量。
* 详情请参阅 https://www.freertos.org/RTOS-task-notifications.html  如果未定义则默认值为 1 。*/
#define configTASK_NOTIFICATION_ARRAY_ENTRIES      1

/* configQUEUE_REGISTRY_SIZE sets the maximum number of queues and semaphores
 * that can be referenced from the queue registry.  Only required when using a
 * kernel aware debugger.  Defaults to 0 if left undefined. */
 /* configQUEUE_REGISTRY_SIZE 用于设定可以从队列注册表中引用的队列和信号量的最大数量。
 只有在使用支持内核的调试器时才需要此设置。若未定义则默认值为 0。 */
#define configQUEUE_REGISTRY_SIZE                  4

/* Set configENABLE_BACKWARD_COMPATIBILITY to 1 to map function names and
 * datatypes from old version of FreeRTOS to their latest equivalent.  Defaults to
 * 1 if left undefined. */
 /* 将 configENABLE_BACKWARD_COMPATIBILITY 设置为 1 可以实现将 FreeRTOS 旧版本中的函数名称和数据类型映射到其最新对应的版本。
 如果未定义该值，则默认值为 1。*/
#define configENABLE_BACKWARD_COMPATIBILITY        1

/* Each task has its own array of pointers that can be used as thread local
 * storage.  configNUM_THREAD_LOCAL_STORAGE_POINTERS set the number of indexes in
 * the array.  See https://www.freertos.org/thread-local-storage-pointers.html
 * Defaults to 0 if left undefined. */
 /* 每个任务都有其自身的指针数组，可作为线程局部存储使用。
 配置参数 `NUM_THREAD_LOCAL_STORAGE_POINTERS` 可设定数组中的索引数量。
 详情请参阅 https://www.freertos.org/thread-local-storage-pointers.html  。若未定义，则默认值为 0 。*/
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    0

/* When configUSE_MINI_LIST_ITEM is set to 0, MiniListItem_t and ListItem_t are
 * both the same. When configUSE_MINI_LIST_ITEM is set to 1, MiniListItem_t contains
 * 3 fewer fields than ListItem_t which saves some RAM at the cost of violating
 * strict aliasing rules which some compilers depend on for optimization. Defaults
 * to 1 if left undefined. */
 /* 若将 configUSE_MINI_LIST_ITEM 设置为 0，则 MiniListItem_t 和 ListItem_t 是相同的。若将该值设置为 1，
 则 MiniListItem_t 比 ListItem_t 少 3 个字段，这样能节省一些内存，但会违反某些编译器基于优化所需而依赖的严格别名规则。
 违约；拖欠（债务）若未作定义，则设为 1 。*/
#define configUSE_MINI_LIST_ITEM                   1

/* Sets the type used by the parameter to xTaskCreate() that specifies the stack
 * size of the task being created.  The same type is used to return information
 * about stack usage in various other API calls.  Defaults to size_t if left
 * undefined. */
 /* 设置用于在 xTaskCreate() 函数中指定参数所使用的类型，该类型用于指定正在创建的任务的栈大小。
 在其他各种 API 调用中，也会使用相同的类型来返回有关栈使用情况的信息。如果未定义，则默认值为 size_t 。*/
#define configSTACK_DEPTH_TYPE                     size_t

/* configMESSAGE_BUFFER_LENGTH_TYPE sets the type used to store the length of
 * each message written to a FreeRTOS message buffer (the length is also written to
 * the message buffer.  Defaults to size_t if left undefined - but that may waste
 * space if messages never go above a length that could be held in a uint8_t. */
 /* configMESSAGE_BUFFER_LENGTH_TYPE 用于设置用于存储写入到 FreeRTOS 消息缓冲区中的每个消息长度的类型
 （该长度也会写入到消息缓冲区中）。
 默认值为 size_t，如果未定义则会采用该值——但若消息的长度从未超过可由 uint8_t 类型表示的范围，则可能会造成空间浪费。*/
#define configMESSAGE_BUFFER_LENGTH_TYPE           size_t

/* If configHEAP_CLEAR_MEMORY_ON_FREE is set to 1, then blocks of memory allocated
 * using pvPortMalloc() will be cleared (i.e. set to zero) when freed using
 * vPortFree(). Defaults to 0 if left undefined. */
 /* 如果配置项“configHEAP_CLEAR_MEMORY_ON_FREE”被设置为 1，那么使用 pvPortMalloc() 分配的内存块在使用 vPortFree() 
 释放时将会被清零（即设置为零值）。如果未进行定义，则默认值为 0。*/
#define configHEAP_CLEAR_MEMORY_ON_FREE            1

/* vTaskList and vTaskGetRunTimeStats APIs take a buffer as a parameter and assume
 * that the length of the buffer is configSTATS_BUFFER_MAX_LENGTH. Defaults to
 * 0xFFFF if left undefined.
 * New applications are recommended to use vTaskListTasks and
 * vTaskGetRunTimeStatistics APIs instead and supply the length of the buffer
 * explicitly to avoid memory corruption. */
 /* vTaskList 和 vTaskGetRunTimeStats 这两个 API 会将一个缓冲区作为参数传入，
 并假定该缓冲区的长度为 configSTATS_BUFFER_MAX_LENGTH。如果未定义，则默认值为 0xFFFF。
* 推荐新应用使用 vTaskListTasks 和 vTaskGetRunTimeStatistics 这两个 API，并明确指定缓冲区的长度，以避免内存损坏。 */
#define configSTATS_BUFFER_MAX_LENGTH              0xFFFF

/* Set configUSE_NEWLIB_REENTRANT to 1 to have a newlib reent structure
 * allocated for each task.  Set to 0 to not support newlib reent structures.
 * Default to 0 if left undefined.
 *
 * Note Newlib support has been included by popular demand, but is not used or
 * tested by the FreeRTOS maintainers themselves. FreeRTOS is not responsible for
 * resulting newlib operation. User must be familiar with newlib and must provide
 * system-wide implementations of the necessary stubs. Note that (at the time of
 * writing) the current newlib design implements a system-wide malloc() that must
 * be provided with locks. */
 /* 将 configUSE_NEWLIB_REENTRANT 设置为 1 可以为每个任务分配一个新的lib reent 结构。
 将其设置为 0 则表示不支持新的lib reent 结构。如果未定义该值，则默认为 0。*
* 注意：新lib的支持是应广大用户的要求而加入的，但并未被 FreeRTOS 的维护人员实际使用或进行测试。
* FreeRTOS 不对由此产生的新lib 运行情况负责。用户必须熟悉新lib 并且必须提供系统级的必要补丁实现。
请注意（在撰写本文时），当前的新lib 设计实现了一个系统级的 malloc()，并且必须与锁一起提供。*/
#define configUSE_NEWLIB_REENTRANT                 0

/******************************************************************************/
/* Software timer related definitions. ****************************************/
/******************************************************************************/

/* Set configUSE_TIMERS to 1 to include software timer functionality in the
 * build.  Set to 0 to exclude software timer functionality from the build.  The
 * FreeRTOS/source/timers.c source file must be included in the build if
 * configUSE_TIMERS is set to 1.  Default to 0 if left undefined.  See
 * https://www.freertos.org/RTOS-software-timer.html. */
 /* 将 configUSE_TIMERS 设置为 1 可在构建过程中包含软件定时器功能。将其设置为 0 则可排除软件定时器功能的构建。
 最终的译文：这个 如果将 configUSE_TIMERS 设置为 1，则必须将 FreeRTOS/source/timers.c 这个源文件包含在构建过程中。
 如果未定义该参数，则默认值为 0。见 https://www.freertos.org/RTOS软件定时器.html*/
#define configUSE_TIMERS                1

/* configTIMER_TASK_PRIORITY sets the priority used by the timer task.  Only
 * used if configUSE_TIMERS is set to 1.  The timer task is a standard FreeRTOS
 * task, so its priority is set like any other task.  See
 * https://www.freertos.org/RTOS-software-timer-service-daemon-task.html  Only used
 * if configUSE_TIMERS is set to 1. */
 /* configTIMER_TASK_PRIORITY 用于设置定时任务所使用的优先级。仅仅
* 若将配置项“USE_TIMERS”设置为 1 时使用。该定时器任务是标准的 FreeRTOS 任务，因此其优先级的设置方式与任何其他任务相同。见
* https://www.freertos.org/RTOS软件定时器服务守护任务.html  仅在
* configUSE_TIMERS 设置为 1 时才会使用。*/
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 2 )

/* configTIMER_TASK_STACK_DEPTH sets the size of the stack allocated to the
 * timer task (in words, not in bytes!).  The timer task is a standard FreeRTOS
 * task.  See https://www.freertos.org/RTOS-software-timer-service-daemon-task.html
 * Only used if configUSE_TIMERS is set to 1. */
 /* configTIMER_TASK_STACK_DEPTH 用于设置分配给定时器任务的栈的大小（以字节为单位，而非字为单位）。
 定时器任务是标准的 FreeRTOS 任务。请参阅 https://www.freertos.org/RTOS-software-timer-service-daemon-task.html 以获取更多信息。
 只有在 configUSE_TIMERS 设置为 1 时才会使用此参数。*/
#define configTIMER_TASK_STACK_DEPTH    configMINIMAL_STACK_SIZE

/* configTIMER_QUEUE_LENGTH sets the length of the queue (the number of discrete
 * items the queue can hold) used to send commands to the timer task.  See
 * https://www.freertos.org/RTOS-software-timer-service-daemon-task.html  Only used
 * if configUSE_TIMERS is set to 1. */
 /* configTIMER_QUEUE_LENGTH 用于设置用于向定时任务发送命令的队列长度（即该队列能够容纳的独立项的数量）。见
* https://www.freertos.org/RTOS软件定时器服务守护任务.html  仅在
* configUSE_TIMERS 设置为 1 时才会使用。*/
#define configTIMER_QUEUE_LENGTH        32

/******************************************************************************/
/* Event Group related definitions. *******************************************/
/******************************************************************************/

/* Set configUSE_EVENT_GROUPS to 1 to include event group functionality in the
 * build. Set to 0 to exclude event group functionality from the build. The
 * FreeRTOS/source/event_groups.c source file must be included in the build if
 * configUSE_EVENT_GROUPS is set to 1. Defaults to 1 if left undefined. */
/* 将 configUSE_EVENT_GROUPS 设置为 1 可以在构建过程中包含事件组功能。将其设置为 0 则可排除事件组功能的构建。
最终的译文：这个
* 如果将 configUSE_EVENT_GROUPS 设置为 1，则必须将 FreeRTOS/source/event_groups.c 源文件包含在构建过程中。
* 若未定义该参数，则其默认值为 1。*/
#define configUSE_EVENT_GROUPS    1

/******************************************************************************/
/* Stream Buffer related definitions. *****************************************/
/******************************************************************************/

/* Set configUSE_STREAM_BUFFERS to 1 to include stream buffer functionality in
 * the build. Set to 0 to exclude event group functionality from the build. The
 * FreeRTOS/source/stream_buffer.c source file must be included in the build if
 * configUSE_STREAM_BUFFERS is set to 1. Defaults to 1 if left undefined. */
/* 将 configUSE_STREAM_BUFFERS 设置为 1 可在构建过程中包含流缓冲功能。
将其设置为 0 可排除事件组功能的构建。若将 configUSE_STREAM_BUFFERS 设置为 1，
则必须在构建中包含 FreeRTOS/source/stream_buffer.c 源文件。如果未定义该值，则默认值为 1。  */
#define configUSE_STREAM_BUFFERS    1

/******************************************************************************/
/* Memory allocation related definitions. *************************************/
/******************************************************************************/

/* Set configSUPPORT_STATIC_ALLOCATION to 1 to include FreeRTOS API functions
 * that create FreeRTOS objects (tasks, queues, etc.) using statically allocated
 * memory in the build.  Set to 0 to exclude the ability to create statically
 * allocated objects from the build.  Defaults to 0 if left undefined.  See
 * https://www.freertos.org/Static_Vs_Dynamic_Memory_Allocation.html. */
 /* 将 configSUPPORT_STATIC_ALLOCATION 设置为 1 可以在构建过程中包含使用静态分配内存来创建 FreeRTOS 对象（任务、队列等）的 FreeRTOS API 函数。
 将其设置为 0 可以排除在构建过程中使用静态分配对象的能力。默认值为 0，如果未定义则采用此默认值。见
* https://www.freertos.org/静态内存分配与动态内存分配.html*/
#define configSUPPORT_STATIC_ALLOCATION              1

/* Set configSUPPORT_DYNAMIC_ALLOCATION to 1 to include FreeRTOS API functions
 * that create FreeRTOS objects (tasks, queues, etc.) using dynamically allocated
 * memory in the build.  Set to 0 to exclude the ability to create dynamically
 * allocated objects from the build.  Defaults to 1 if left undefined.  See
 * https://www.freertos.org/Static_Vs_Dynamic_Memory_Allocation.html. */
 /* 将 configSUPPORT_DYNAMIC_ALLOCATION 设置为 1 可以在构建过程中包含 FreeRTOS API 函数，
 这些函数使用动态分配的内存来创建 FreeRTOS 对象（任务、队列等）。将该值设置为 0 可以排除在构建中使用动态分配对象的能力。
 默认值为 1，如果未定义则保持此值为 1。见
* https://www.freertos.org/静态内存分配与动态内存分配.html*/
#define configSUPPORT_DYNAMIC_ALLOCATION             0

/* Sets the total size of the FreeRTOS heap, in bytes, when heap_1.c, heap_2.c
 * or heap_4.c are included in the build.  This value is defaulted to 4096 bytes but
 * it must be tailored to each application.  Note the heap will appear in the .bss
 * section.  See https://www.freertos.org/a00111.html. */
 /* 设置 FreeRTOS 堆的总大小（以字节为单位），当将 heap_1.c、heap_2.c 或 heap_4.c 包含在构建过程中时生效。
 该值默认为 4096 字节，但必须根据每个应用程序进行调整。请注意，堆会出现在.bss 节中。
 请参阅 https://www.freertos.org/a00111.html 。*/
#define configTOTAL_HEAP_SIZE                        ( configSUPPORT_DYNAMIC_ALLOCATION*4*1024 )

#define configMINIMAL_SECURE_STACK_SIZE         configMINIMAL_STACK_SIZE
/* Set configAPPLICATION_ALLOCATED_HEAP to 1 to have the application allocate
 * the array used as the FreeRTOS heap.  Set to 0 to have the linker allocate the
 * array used as the FreeRTOS heap.  Defaults to 0 if left undefined. */
 /* 将 configAPPLICATION_ALLOCATED_HEAP 设置为 1，以便应用程序自行分配用作 FreeRTOS 堆的数组。将该值设置为 0，
 则由链接器分配用作 FreeRTOS 堆的数组。如果未定义则默认值为 0。*/
#define configAPPLICATION_ALLOCATED_HEAP             0

/* Set configSTACK_ALLOCATION_FROM_SEPARATE_HEAP to 1 to have task stacks
 * allocated from somewhere other than the FreeRTOS heap.  This is useful if you
 * want to ensure stacks are held in fast memory.  Set to 0 to have task stacks
 * come from the standard FreeRTOS heap.  The application writer must provide
 * implementations for pvPortMallocStack() and vPortFreeStack() if set to 1.
 * Defaults to 0 if left undefined. */
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP    0

/* Set configENABLE_HEAP_PROTECTOR to 1 to enable bounds checking and obfuscation
 * to internal heap block pointers in heap_4.c and heap_5.c to help catch pointer
 * corruptions. Defaults to 0 if left undefined. */
#define configENABLE_HEAP_PROTECTOR                  0

/******************************************************************************/
/* Interrupt nesting behaviour configuration. *********************************/
/******************************************************************************/

#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 


/* configKERNEL_INTERRUPT_PRIORITY sets the priority of the tick and context
 * switch performing interrupts.  Not supported by all FreeRTOS ports.  See
 * https://www.freertos.org/RTOS-Cortex-M3-M4.html for information specific to
 * ARM Cortex-M devices. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

/* configMAX_SYSCALL_INTERRUPT_PRIORITY sets the interrupt priority above which
 * FreeRTOS API calls must not be made.  Interrupts above this priority are never
 * disabled, so never delayed by RTOS activity.  The default value is set to the
 * highest interrupt priority (0).  Not supported by all FreeRTOS ports.
 * See https://www.freertos.org/RTOS-Cortex-M3-M4.html for information specific to
 * ARM Cortex-M devices. */
/* configMAX_SYSCALL_INTERRUPT_PRIORITY 用于设置超出此优先级范围的中断操作将被禁止的阈值。
高于此优先级的中断永远不会被禁用，因此不会因实时操作系统活动而延迟。默认值设为最高的中断优先级（0）。
并非所有 FreeRTOS 端口都支持此功能。有关特定于 ARM Cortex-M 设备的信息，
请参阅 https://www.freertos.org/RTOS-Cortex-M3-M4.html 。*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Another name for configMAX_SYSCALL_INTERRUPT_PRIORITY - the name used depends
 * on the FreeRTOS port. */
#define configMAX_API_CALL_INTERRUPT_PRIORITY    0

/******************************************************************************/
/* Hook and callback function related definitions. ****************************/
/******************************************************************************/

/* Set the following configUSE_* constants to 1 to include the named hook
 * functionality in the build.  Set to 0 to exclude the hook functionality from the
 * build.  The application writer is responsible for providing the hook function
 * for any set to 1.  See https://www.freertos.org/a00016.html. */
 /* 将以下名为“USE_”的配置常量设为 1 可以在构建过程中包含指定的钩子功能。
 将这些常量设为 0 则可排除该钩子功能的构建。应用程序编写者需为任何设为 1 的钩子提供相应的函数。
 详情请参阅 https://www.freertos.org/a00016.html 。*/
#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MALLOC_FAILED_HOOK          0
#define configUSE_DAEMON_TASK_STARTUP_HOOK    0

/* Set configUSE_SB_COMPLETED_CALLBACK to 1 to have send and receive completed
 * callbacks for each instance of a stream buffer or message buffer. When the
 * option is set to 1, APIs xStreamBufferCreateWithCallback() and
 * xStreamBufferCreateStaticWithCallback() (and likewise APIs for message
 * buffer) can be used to create a stream buffer or message buffer instance
 * with application provided callbacks. Defaults to 0 if left undefined. */
#define configUSE_SB_COMPLETED_CALLBACK       0

/* Set configCHECK_FOR_STACK_OVERFLOW to 1 or 2 for FreeRTOS to check for a
 * stack overflow at the time of a context switch.  Set to 0 to not look for a
 * stack overflow.  If configCHECK_FOR_STACK_OVERFLOW is 1 then the check only
 * looks for the stack pointer being out of bounds when a task's context is saved
 * to its stack - this is fast but somewhat ineffective.  If
 * configCHECK_FOR_STACK_OVERFLOW is 2 then the check looks for a pattern written
 * to the end of a task's stack having been overwritten.  This is slower, but will
 * catch most (but not all) stack overflows.  The application writer must provide
 * the stack overflow callback when configCHECK_FOR_STACK_OVERFLOW is set to 1.
 * See https://www.freertos.org/Stacks-and-stack-overflow-checking.html  Defaults
 * to 0 if left undefined. */
 /* 将 configCHECK_FOR_STACK_OVERFLOW 设置为 1 或 2，以便 FreeRTOS 在进行上下文切换时检查是否存在栈溢出情况。
 将其设置为 0 则不进行此检查。
 如果 configCHECK_FOR_STACK_OVERFLOW 的值为 1，
 则该检查仅在任务的上下文被保存到其栈中时检查栈指针是否超出界限——这种方式速度快，但效果稍差。如果
* 如果 configCHECK_FOR_STACK_OVERFLOW 的值为 2，那么该检查会查找任务栈末尾所写入的模式是否已被覆盖。
这种方式速度较慢，但能捕获大多数（但并非全部）的栈溢出情况。当将 configCHECK_FOR_STACK_OVERFLOW 设置为 1 时，
应用程序编写者必须提供栈溢出回调函数。
* 请参阅 https://www.freertos.org/Stacks-and-stack-overflow-checking.html  默认值为 0，若未定义则采用此默认值。*/
#define configCHECK_FOR_STACK_OVERFLOW        2

/******************************************************************************/
/* Run time and task stats gathering related definitions. *********************/
/******************************************************************************/

/* Set configGENERATE_RUN_TIME_STATS to 1 to have FreeRTOS collect data on the
 * processing time used by each task.  Set to 0 to not collect the data.  The
 * application writer needs to provide a clock source if set to 1.  Defaults to 0
 * if left undefined.  See https://www.freertos.org/rtos-run-time-stats.html. */
#define configGENERATE_RUN_TIME_STATS           0

/* Set configUSE_TRACE_FACILITY to include additional task structure members
 * are used by trace and visualisation functions and tools.  Set to 0 to exclude
 * the additional information from the structures. Defaults to 0 if left
 * undefined. */
 /* 将 configUSE_TRACE_FACILITY 设置为 1 可以包含由跟踪和可视化函数及工具所使用的额外任务结构成员。
 将其设置为 0 则可排除这些结构中的额外信息。默认值为 0，若未定义则采用此默认值。*/
#define configUSE_TRACE_FACILITY                1

/* Set to 1 to include the vTaskList() and vTaskGetRunTimeStats() functions in
 * the build.  Set to 0 to exclude these functions from the build.  These two
 * functions introduce a dependency on string formatting functions that would
 * otherwise not exist - hence they are kept separate.  Defaults to 0 if left
 * undefined. */
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/******************************************************************************/
/* Co-routine related definitions. ********************************************/
/******************************************************************************/

/* Set configUSE_CO_ROUTINES to 1 to include co-routine functionality in the
 * build, or 0 to omit co-routine functionality from the build. To include
 * co-routines, croutine.c must be included in the project. Defaults to 0 if left
 * undefined. */
#define configUSE_CO_ROUTINES              0

/* configMAX_CO_ROUTINE_PRIORITIES defines the number of priorities available
 * to the application co-routines. Any number of co-routines can share the same
 * priority. Defaults to 0 if left undefined. */
 /* configMAX_CO_ROUTINE_PRIORITIES 定义了应用程序协程可用的优先级数量。
* 任何数量的协程都可以使用相同的优先级。如果未定义则默认值为 0。*/
#define configMAX_CO_ROUTINE_PRIORITIES    2

/******************************************************************************/
/* Debugging assistance. ******************************************************/
/******************************************************************************/

/* configASSERT() has the same semantics as the standard C assert().  It can
 * either be defined to take an action when the assertion fails, or not defined
 * at all (i.e. comment out or delete the definitions) to completely remove
 * assertions.  configASSERT() can be defined to anything you want, for example
 * you can call a function if an assert fails that passes the filename and line
 * number of the failing assert (for example, "vAssertCalled( __FILE__, __LINE__ )"
 * or it can simple disable interrupts and sit in a loop to halt all execution
 * on the failing line for viewing in a debugger. */
 /* configASSERT() 的功能与标准 C 语言中的 assert() 函数具有相同的作用。它能够
* 或者被定义为在断言失败时执行某种操作，或者不进行定义
* 完全不进行（即注释掉或删除这些定义）以彻底移除断言。配置ASSERT() 可以被定义为您想要的任何内容，例如
* 如果断言失败，您可以调用一个函数，该函数会传递断言的文件名和行号（例如，“vAssertCalled( __FILE__， __LINE__ )”）
* 或者它可以简单地禁用中断并进入一个循环，在断言失败的行上暂停所有执行以便在调试器中查看。*/

//#define configASSERT( x )         \
//    if( ( x ) == 0 )              \
//    {                             \
//        taskDISABLE_INTERRUPTS(); \
//        for( ; ; )                \
//        ;                         \
//    }

/******************************************************************************/
/* FreeRTOS MPU specific definitions. *****************************************/
/******************************************************************************/

/* If configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS is set to 1 then
 * the application writer can provide functions that execute in privileged mode.
 * See: https://www.freertos.org/a00110.html#configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
 * Defaults to 0 if left undefined.  Only used by the FreeRTOS Cortex-M MPU ports,
 * not the standard ARMv7-M Cortex-M port. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0

/* Set configTOTAL_MPU_REGIONS to the number of MPU regions implemented on your
 * target hardware.  Normally 8 or 16.  Only used by the FreeRTOS Cortex-M MPU
 * ports, not the standard ARMv7-M Cortex-M port.  Defaults to 8 if left
 * undefined. */
#define configTOTAL_MPU_REGIONS                                   8

/* configTEX_S_C_B_FLASH allows application writers to override the default
 * values for the for TEX, Shareable (S), Cacheable (C) and Bufferable (B) bits for
 * the MPU region covering Flash.  Defaults to 0x07UL (which means TEX=000, S=1,
 * C=1, B=1) if left undefined.  Only used by the FreeRTOS Cortex-M MPU ports, not
 * the standard ARMv7-M Cortex-M port. */
#define configTEX_S_C_B_FLASH                                     0x07UL

/* configTEX_S_C_B_SRAM allows application writers to override the default
 * values for the for TEX, Shareable (S), Cacheable (C) and Bufferable (B) bits for
 * the MPU region covering RAM. Defaults to 0x07UL (which means TEX=000, S=1, C=1,
 * B=1) if left undefined.  Only used by the FreeRTOS Cortex-M MPU ports, not
 * the standard ARMv7-M Cortex-M port. */
#define configTEX_S_C_B_SRAM                                      0x07UL

/* Set configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY to 0 to prevent any privilege
 * escalations originating from outside of the kernel code itself.  Set to 1 to
 * allow application tasks to raise privilege.  Defaults to 1 if left undefined.
 * Only used by the FreeRTOS Cortex-M MPU ports, not the standard ARMv7-M Cortex-M
 * port. */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY               1

/* Set configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS to 1 to allow unprivileged
 * tasks enter critical sections (effectively mask interrupts). Set to 0 to
 * prevent unprivileged tasks entering critical sections.  Defaults to 1 if left
 * undefined.  Only used by the FreeRTOS Cortex-M MPU ports, not the standard
 * ARMv7-M Cortex-M port. */
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS                0

/* FreeRTOS Kernel version 10.6.0 introduced a new v2 MPU wrapper, namely
 * mpu_wrappers_v2.c. Set configUSE_MPU_WRAPPERS_V1 to 0 to use the new v2 MPU
 * wrapper. Set configUSE_MPU_WRAPPERS_V1 to 1 to use the old v1 MPU wrapper
 * (mpu_wrappers.c). Defaults to 0 if left undefined. */
#define configUSE_MPU_WRAPPERS_V1                                 0

/* When using the v2 MPU wrapper, set configPROTECTED_KERNEL_OBJECT_POOL_SIZE to
 * the total number of kernel objects, which includes tasks, queues, semaphores,
 * mutexes, event groups, timers, stream buffers and message buffers, in your
 * application. The application will not be able to have more than
 * configPROTECTED_KERNEL_OBJECT_POOL_SIZE kernel objects at any point of
 * time. */
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                   10

/* When using the v2 MPU wrapper, set configSYSTEM_CALL_STACK_SIZE to the size
 * of the system call stack in words. Each task has a statically allocated
 * memory buffer of this size which is used as the stack to execute system
 * calls. For example, if configSYSTEM_CALL_STACK_SIZE is defined as 128 and
 * there are 10 tasks in the application, the total amount of memory used for
 * system call stacks is 128 * 10 = 1280 words. */
#define configSYSTEM_CALL_STACK_SIZE                              128

/* When using the v2 MPU wrapper, set configENABLE_ACCESS_CONTROL_LIST to 1 to
 * enable Access Control List (ACL) feature. When ACL is enabled, an
 * unprivileged task by default does not have access to any kernel object other
 * than itself. The application writer needs to explicitly grant the
 * unprivileged task access to the kernel objects it needs using the APIs
 * provided for the same. Defaults to 0 if left undefined. */
#define configENABLE_ACCESS_CONTROL_LIST                          1

/******************************************************************************/
/* SMP( Symmetric MultiProcessing ) Specific Configuration definitions. *******/
/******************************************************************************/

/* Set configNUMBER_OF_CORES to the number of available processor cores. Defaults
 * to 1 if left undefined. */

/*
 #define configNUMBER_OF_CORES                     [Num of available cores]
 */

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configRUN_MULTIPLE_PRIORITIES to 0 to allow multiple tasks to run
 * simultaneously only if they do not have equal priority, thereby maintaining
 * the paradigm of a lower priority task never running if a higher priority task
 * is able to run. If configRUN_MULTIPLE_PRIORITIES is set to 1, multiple tasks
 * with different priorities may run simultaneously - so a higher and lower
 * priority task may run on different cores at the same time. */
#define configRUN_MULTIPLE_PRIORITIES             0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configUSE_CORE_AFFINITY to 1 to enable core affinity feature. When core
 * affinity feature is enabled, the vTaskCoreAffinitySet and vTaskCoreAffinityGet
 * APIs can be used to set and retrieve which cores a task can run on. If
 * configUSE_CORE_AFFINITY is set to 0 then the FreeRTOS scheduler is free to
 * run any task on any available core. */
#define configUSE_CORE_AFFINITY                   0

/* When using SMP with core affinity feature enabled, set
 * configTASK_DEFAULT_CORE_AFFINITY to change the default core affinity mask for
 * tasks created without an affinity mask specified. Setting the define to 1 would
 * make such tasks run on core 0 and setting it to (1 << portGET_CORE_ID()) would
 * make such tasks run on the current core. This config value is useful, if
 * swapping tasks between cores is not supported (e.g. Tricore) or if legacy code
 * should be controlled. Defaults to tskNO_AFFINITY if left undefined. */
#define configTASK_DEFAULT_CORE_AFFINITY          tskNO_AFFINITY

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), if
 * configUSE_TASK_PREEMPTION_DISABLE is set to 1, individual tasks can be set to
 * either pre-emptive or co-operative mode using the vTaskPreemptionDisable and
 * vTaskPreemptionEnable APIs. */
#define configUSE_TASK_PREEMPTION_DISABLE         0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configUSE_PASSIVE_IDLE_HOOK to 1 to allow the application writer to use
 * the passive idle task hook to add background functionality without the overhead
 * of a separate task. Defaults to 0 if left undefined. */
#define configUSE_PASSIVE_IDLE_HOOK               0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one),
 * configTIMER_SERVICE_TASK_CORE_AFFINITY allows the application writer to set
 * the core affinity of the RTOS Daemon/Timer Service task. Defaults to
 * tskNO_AFFINITY if left undefined. */
#define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY


/******************************************************************************/
/* ARMv8-M secure side port related definitions. ******************************/
/******************************************************************************/

/* secureconfigMAX_SECURE_CONTEXTS define the maximum number of tasks that can
 *  call into the secure side of an ARMv8-M chip.  Not used by any other ports. */
#define secureconfigMAX_SECURE_CONTEXTS        5

/* Defines the kernel provided implementation of
 * vApplicationGetIdleTaskMemory() and vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Idle task and Timer task respectively.
 * The application can provide it's own implementation of
 * vApplicationGetIdleTaskMemory() and vApplicationGetTimerTaskMemory() by
 * setting configKERNEL_PROVIDED_STATIC_MEMORY to 0 or leaving it undefined. */
 /* 定义了内核所提供的 vApplicationGetIdleTaskMemory() 和 vApplicationGetTimerTaskMemory() 函数的实现，
 以分别提供给定给空闲任务和定时任务使用的内存。* 应用程序可以通过将 configKERNEL_PROVIDED_STATIC_MEMORY 
 设置为 0 或不进行定义的方式来提供自己的 vApplicationGetIdleTaskMemory() 和 vApplicationGetTimerTaskMemory() 实现。 */
#define configKERNEL_PROVIDED_STATIC_MEMORY    0

/******************************************************************************/
/* ARMv8-M port Specific Configuration definitions. ***************************/
/******************************************************************************/

/* Set configENABLE_TRUSTZONE to 1 when running FreeRTOS on the non-secure side
 * to enable the TrustZone support in FreeRTOS ARMv8-M ports which allows the
 * non-secure FreeRTOS tasks to call the (non-secure callable) functions
 * exported from secure side. */
#define configENABLE_TRUSTZONE            1

/* If the application writer does not want to use TrustZone, but the hardware does
 * not support disabling TrustZone then the entire application (including the FreeRTOS
 * scheduler) can run on the secure side without ever branching to the non-secure side.
 * To do that, in addition to setting configENABLE_TRUSTZONE to 0, also set
 * configRUN_FREERTOS_SECURE_ONLY to 1. */
#define configRUN_FREERTOS_SECURE_ONLY    1

/* Set configENABLE_MPU to 1 to enable the Memory Protection Unit (MPU), or 0
 * to leave the Memory Protection Unit disabled. */
#define configENABLE_MPU                  1

/* Set configENABLE_FPU to 1 to enable the Floating Point Unit (FPU), or 0
 * to leave the Floating Point Unit disabled. */
#define configENABLE_FPU                  1

/* Set configENABLE_MVE to 1 to enable the M-Profile Vector Extension (MVE) support,
 * or 0 to leave the MVE support disabled. This option is only applicable to Cortex-M55
 * and Cortex-M85 ports as M-Profile Vector Extension (MVE) is available only on
 * these architectures. configENABLE_MVE must be left undefined, or defined to 0
 * for the Cortex-M23,Cortex-M33 and Cortex-M35P ports. */
#define configENABLE_MVE                  1

/******************************************************************************/
/* ARMv7-M and ARMv8-M port Specific Configuration definitions. ***************/
/******************************************************************************/

/* Set configCHECK_HANDLER_INSTALLATION to 1 to enable additional asserts to verify
 * that the application has correctly installed FreeRTOS interrupt handlers.
 *
 * An application can install FreeRTOS interrupt handlers in one of the following ways:
 *   1. Direct Routing  -  Install the functions vPortSVCHandler and xPortPendSVHandler
 *                         for SVC call and PendSV interrupts respectively.
 *   2. Indirect Routing - Install separate handlers for SVC call and PendSV
 *                         interrupts and route program control from those handlers
 *                         to vPortSVCHandler and xPortPendSVHandler functions.
 * The applications that use Indirect Routing must set configCHECK_HANDLER_INSTALLATION to 0.
 *
 * Defaults to 1 if left undefined. */
 /* 将 configCHECK_HANDLER_INSTALLATION 设置为 1 可以启用额外的断言，以验证应用程序是否已正确安装 FreeRTOS 中断处理程序。*
应用程序可以通过以下方式之一来安装 FreeRTOS 中断处理程序：
1. 直接路由 - 分别为 SVC 调用和 PendSV 中断安装函数 vPortSVCHandler 和 xPortPendSVHandler。
2. 间接路由 - 为 SVC 调用和 PendSV 中断分别安装单独的处理程序，并从这些处理程序将程序控制权路由至 vPortSVCHandler 和 xPortPendSVHandler 函数。
使用间接路由的程序必须将 configCHECK_HANDLER_INSTALLATION 设置为 0。*
如果未作定义，则默认值为 1 。*/
#define configCHECK_HANDLER_INSTALLATION    0

/******************************************************************************/
/* Definitions that include or exclude functionality. *************************/
/******************************************************************************/

/* Set the following configUSE_* constants to 1 to include the named feature in
 * the build, or 0 to exclude the named feature from the build. */
#define configUSE_TASK_NOTIFICATIONS           1
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES          1
#define configUSE_QUEUE_SETS                   0
#define configUSE_APPLICATION_TASK_TAG         0

/* Set the following INCLUDE_* constants to 1 to incldue the named API function,
 * or 0 to exclude the named API function.  Most linkers will remove unused
 * functions even when the constant is 1. */
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_xResumeFromISR                 1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_uxTaskGetStackHighWaterMark    0
#define INCLUDE_xTaskGetIdleTaskHandle         0
#define INCLUDE_eTaskGetState                  0
#define INCLUDE_xEventGroupSetBitFromISR       0
#define INCLUDE_xTimerPendFunctionCall         0
#define INCLUDE_xTaskAbortDelay                0
#define INCLUDE_xTaskGetHandle                 0
#define INCLUDE_xTaskResumeFromISR             1

#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
