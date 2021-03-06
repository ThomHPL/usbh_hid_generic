/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "USBH_HID_generic/usbh_hid.h"
#include "stdio.h"
#include "Dualshock3_driver.h"

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
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
		.name = "defaultTask",
		.priority = (osPriority_t) osPriorityNormal,
		.stack_size = 128 * 4
};
/* Definitions for usbhTask */
osThreadId_t usbhTaskHandle;
const osThreadAttr_t usbhTask_attributes = {
		.name = "usbhTask",
		.priority = (osPriority_t) osPriorityNormal,
		.stack_size = 128 * 4
};
/* Definitions for processReport */
osThreadId_t processReportHandle;
const osThreadAttr_t processReport_attributes = {
		.name = "processReport",
		.priority = (osPriority_t) osPriorityNormal,
		.stack_size = 1024 * 4
};
/* Definitions for reportQueue */
osMessageQueueId_t reportQueueHandle;
const osMessageQueueAttr_t reportQueue_attributes = {
		.name = "reportQueue"
};
/* Definitions for usbhMutex */
osMutexId_t usbhMutexHandle;
const osMutexAttr_t usbhMutex_attributes = {
		.name = "usbhMutex"
};
/* Definitions for uart2Mutex */
osMutexId_t uart2MutexHandle;
const osMutexAttr_t uart2Mutex_attributes = {
		.name = "uart2Mutex"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void usbhTaskFunction(void *argument);
void processReportFunction(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Dualshock3_newReport_CB(DS3_report* report)
{
	if(osMessageQueueGetSpace(reportQueueHandle) == 0)
	{
		osMessageQueueReset(reportQueueHandle);
	}
	osMessageQueuePut(reportQueueHandle, report, 0, 0);
}

void Dualshock3_connected_CB()
{
	// use semaphore to start report task
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

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
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	MX_USB_HOST_Init();

	// stops the broken auto generated thread for the usbh process
	MX_USB_HOST_SuspendThread();

	// loads the dualshock 3 driver
	USBH_HID_GenericRegisterDriver(dualshock3_Driver);

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();
	/* Create the mutex(es) */
	/* creation of usbhMutex */
	usbhMutexHandle = osMutexNew(&usbhMutex_attributes);

	/* creation of uart2Mutex */
	uart2MutexHandle = osMutexNew(&uart2Mutex_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */

	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of reportQueue */
	reportQueueHandle = osMessageQueueNew (2, sizeof(DS3_report), &reportQueue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* creation of usbhTask */
	usbhTaskHandle = osThreadNew(usbhTaskFunction, NULL, &usbhTask_attributes);

	/* creation of processReport */
	processReportHandle = osThreadNew(processReportFunction, NULL, &processReport_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */

	osThreadSuspend(defaultTaskHandle);
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* init code for USB_HOST */
	MX_USB_HOST_Init();
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_usbhTaskFunction */
/**
 * @brief Function implementing the usbhTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usbhTaskFunction */
void usbhTaskFunction(void *argument)
{
	/* USER CODE BEGIN usbhTaskFunction */
	/* Infinite loop */
	for(;;)
	{
		osMutexAcquire(usbhMutexHandle, osWaitForever);
		MX_USB_HOST_Process();
		osMutexRelease(usbhMutexHandle);

		osDelay(1);
	}
	/* USER CODE END usbhTaskFunction */
}

/* USER CODE BEGIN Header_processReportFunction */
/**
 * @brief Function implementing the processReport thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_processReportFunction */
void processReportFunction(void *argument)
{
	/* USER CODE BEGIN processReportFunction */
	/* Infinite loop */

	char Uart_Buf[192] = "";

	for(;;)
	{
		DS3_report report;
		osMessageQueueGet(reportQueueHandle, &report, NULL, osWaitForever);

		sprintf(Uart_Buf, "Buttons: %2X %2X %2X\r\n", ((uint8_t*)&report)[2], ((uint8_t*)&report)[3], ((uint8_t*)&report)[4]);

		HAL_UART_Transmit(&huart2, (uint8_t *)Uart_Buf, strlen(Uart_Buf), 100);

		sprintf(Uart_Buf, "X: %4d, Y: %4d, Z: %4d, G: %4d\r\n",
				decodeRawData(report.AccX_Raw_HI, report.AccX_Raw_LO),
				decodeRawData(report.AccY_Raw_HI, report.AccY_Raw_LO),
				decodeRawData(report.AccZ_Raw_HI, report.AccZ_Raw_LO),
				decodeRawData(report.Gyro_Raw_HI, report.Gyro_Raw_LO));


		HAL_UART_Transmit(&huart2, (uint8_t *)Uart_Buf, strlen(Uart_Buf), 100);
		HAL_UART_Transmit(&huart2, (uint8_t *)"\033[F\033[F", strlen("\033[F\033[F"), 100);

	}
	/* USER CODE END processReportFunction */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
