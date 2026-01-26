/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "usb.h"
#include "tusb.h"
#include "usart.h"
#include "board_api.h"
#include "DAP.h"
#include "nr_micro_shell.h"
#include "diskio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
//char SDPath[4]={"1:"}; /* SD card logical drive path */
char baes_path[4]={"0:"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void cdc_task(void *params);
static void usb_device_task(void *param);
//extern void msc_disk_init(void);
void hid_task(void* params);
void cdc_task_noneRtos(void) ;
void tiny_none_rtos_task(void *param);

// static task
#if configSUPPORT_STATIC_ALLOCATION
StackType_t  hid_stack[HID_STACK_SZIE];
StaticTask_t hid_taskdef;

StackType_t  usb_device_stack[USBD_STACK_SIZE];
StaticTask_t usb_device_taskdef;

StackType_t  cdc_stack[CDC_STACK_SIZE];
StaticTask_t cdc_taskdef;
#endif

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	FRESULT ret=FR_OK;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  /* USER CODE END 1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	MX_USART1_UART_Init();
	printf("uart init seccod!\r\n");
	MX_USB_PCD_Init();
  /* USER CODE BEGIN 2 */
	
	ret= f_mount(&SDFatFs, "0:", 1);
	if(ret ==FR_OK)
	{
		LOG("挂载成功 \r\n");
		GET_SDInfo(baes_path,&SDFatFs);
		FatFS_Test();
	}
	else
	{
		LOG("挂载失败 \r\n");
//		Get_FatFsInfo(ret);
	}
	Led_Task_Init();
	  /* 启动任务调度 */     
	  /* USER CODE END 2 */
	
	shell_init();
	DAP_Setup();
	
  // init device stack on configured roothub port
//  tusb_rhport_init_t dev_init = {
//    .role = TUSB_ROLE_DEVICE,
//    .speed = TUSB_SPEED_AUTO
//  };
//  tusb_init(BOARD_TUD_RHPORT, &dev_init);

//  board_init_after_tusb();
	

	  // Create task for: tinyusb, blinky, cdc
#if configSUPPORT_STATIC_ALLOCATION
  xTaskCreateStatic(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES-1, usb_device_stack, &usb_device_taskdef);
//  xTaskCreateStatic(cdc_task, "cdc", CDC_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, cdc_stack, &cdc_taskdef);
//	xTaskCreateStatic(hid_task, "hid", HID_STACK_SZIE, NULL, configMAX_PRIORITIES-2, hid_stack, &hid_taskdef);
#else
  xTaskCreate(usb_device_task, "usbd", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(cdc_task, "cdc", CDC_STACK_SZIE, NULL, configMAX_PRIORITIES - 2, NULL);
	xTaskCreate(hid_task, "hid", HID_STACK_SZIE, NULL, configMAX_PRIORITIES-2, NULL);
#endif

	  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */


	
	LOG("运行到此处时 表示失败 \r\n");
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		tud_task();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

// USB Device Driver task
// This top level thread process all usb events and invoke callbacks
static void usb_device_task(void *param) {
  (void) param;

  // init device stack on configured roothub port
  // This should be called after scheduler/kernel is started.
  // Otherwise it could cause kernel issue since USB IRQ handler does use RTOS queue API.
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  board_init_after_tusb();

//  msc_disk_init();
  // RTOS forever loop
  while (1) {
    // put this thread to waiting state until there is new events
    tud_task();

    // following code only run if tud_task() process at least 1 event
    tud_cdc_write_flush();
  }
}

void cdc_task_noneRtos(void) 
{


  if (tud_cdc_n_available(0)) 
	{
    uint8_t buf[64];
    uint32_t count = tud_cdc_n_read(0, buf, sizeof(buf));
		for(uint16_t i=0;i<count;i++ )
		{
			shell(buf[i]);
		}
  }
  

  if (tud_cdc_n_available(1)) {
    uint8_t buf[64];
    uint32_t count = tud_cdc_n_read(1, buf, sizeof(buf));
    

    tud_cdc_n_write(1, buf, count);
    tud_cdc_n_write_flush(1);
  }

}

void cdc_task(void *params) {
  (void) params;

  // RTOS forever loop
  while (1) {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_connected() )
    {
      // There are data available
      while (tud_cdc_available()) {
        uint8_t buf[64];

        // read and echo back
        uint32_t count = tud_cdc_read(buf, sizeof(buf));
        (void) count;

        // Echo back
        // Note: Skip echo by commenting out write() and write_flush()
        // for throughput test e.g
        //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
        tud_cdc_write(buf, count);
      }

      tud_cdc_write_flush();

      // Press on-board button to send Uart status notification
      static uint32_t btn_prev = 0;
      static cdc_notify_uart_state_t uart_state = { .value = 0 };
      const uint32_t btn = board_button_read();
      if (!btn_prev && btn) {
        uart_state.dsr ^= 1;
        tud_cdc_notify_uart_state(&uart_state);
      }
      btn_prev = btn;
    }

    // For ESP32-Sx this delay is essential to allow idle how to run and reset watchdog
    vTaskDelay(1);
  }
}





// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

}
// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
uint8_t hid_tx_data_buf[CFG_TUD_HID_EP_BUFSIZE];
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) report_type;

    uint32_t response_size = TU_MIN(CFG_TUD_HID_EP_BUFSIZE, bufsize);
    DAP_ExecuteCommand(buffer, hid_tx_data_buf);
    tud_hid_n_report(instance, report_id, hid_tx_data_buf, response_size);
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}


void hid_task(void* param)
{
  (void) param;

  while(1)
  {
    // Poll every 10ms
    vTaskDelay(pdMS_TO_TICKS(10));

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if ( tud_suspended() && btn )
    {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      tud_remote_wakeup();
    }
    else
    {
      // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
//      send_hid_report(REPORT_ID_KEYBOARD, btn);
    }
  }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
