/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

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
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

osThreadId UltrasonicTaskHandle;
osThreadId Task2Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
static void MX_SPI1_Init(void);



/* USER CODE BEGIN PFP */
void EnableUltrasonic_1_TimerInterrutp (void);
void triggerUltrasonic_1_Task(void const * argument);
void indecateOnLED_Task(void const * argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile int distance_u1=0;
volatile int distance_u2=0;

volatile int flag_timer2=0,t1_timer2,t2_timer2,time_timer2;
volatile int flag_timer5=0,t1_timer5,t2_timer5,time_timer5;

#define TRIG_PIN_U1      			GPIO_PIN_1
#define TRIG_PIN_U2      			GPIO_PIN_4

#define TRIG_PORT_U1     			GPIOA
#define TRIG_PORT_U2     			GPIOA

#define LED_PIN_U1       			GPIO_PIN_2
#define LED_PIN_U2       			GPIO_PIN_0

#define LED_PORT_U1      			GPIOA
#define LED_PORT_U2      			GPIOB

#define NEAR_OBJECT_THRESHOLD   	20

#define FIRST_DELAY_U1    			50

//ISR for timer ICU
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// checking on timer ICU interrupt source if TIM2 or TIM 5
	if (htim->Instance == TIM2)
	{
		// Check if the callback is triggered by the correct timer (TIMx)
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if (flag_timer2==0)
			{
				t1_timer2=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

				flag_timer2++;
			}
			else if(flag_timer2==1)
			{
				t2_timer2=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				if(t2_timer2>t1_timer2)
				{
					time_timer2=t2_timer2-t1_timer2;
				}
				else if(t1_timer2>t2_timer2)
				{
					time_timer2 = ((0xffff-t1_timer2)+t2_timer2)+1;
				}
				distance_u1=(((int)time_timer2*340*100)/((int)2*1000000))+1;

				// Ensure that the distance is within the range (0 to 50 cm)
				if (distance_u1 > 500)
				{
					distance_u1 = 500;
				}
				else if (distance_u1<0)
				{
					distance_u1=0;
				}
				flag_timer2=0;

			}

		}
	}

	//	if (htim->Instance == TIM5)
	//	{
	//		// Check if the callback is triggered by the correct timer (TIMx)
	//		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	//		{
	//			if (flag_timer5==0)
	//			{
	//				t1_timer5=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
	//
	//				flag_timer5++;
	//			}
	//			else if(flag_timer5==1)
	//			{
	//				t2_timer5=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
	//				if(t2_timer5>t1_timer5)
	//				{
	//					time_timer5=t2_timer5-t1_timer5;
	//				}
	//				else if(t1_timer5>t2_timer5)
	//				{
	//					time_timer5 = ((0xffff-t1_timer5)+t2_timer5)+1;
	//				}
	//				distance_u2=(((int)time_timer5*340*100)/((int)2*1000000))+1;
	// Ensure distance is within the desired range (0 to 50 cm)
	//				if (distance_u2 > 500)
	//				{
	//						distance_u2 = 500;
	//				}
	//				else if (distance_u2<0)
	//				{
	//						distance_u2=0;
	//				}
	//				flag_timer5=0;
	//				HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_4); //disable timer 2
	//				HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // enable timer5
	//			}
	//
	//		}
	//	}
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
	MX_TIM2_Init();
	MX_TIM5_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */
	EnableUltrasonic_1_TimerInterrutp();

	/* USER CODE END 2 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of UltrasonicTask */
	osThreadDef(UltrasonicTask, triggerUltrasonic_1_Task, osPriorityNormal, 0, 128);
	UltrasonicTaskHandle = osThreadCreate(osThread(UltrasonicTask), NULL);

	/* definition and creation of Task2 */
	osThreadDef(Task2, indecateOnLED_Task, osPriorityNormal, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

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
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 15;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init(void)
{

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 15;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 65535;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 PA2 PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void EnableUltrasonic_1_TimerInterrutp (void)
{
	__enable_irq(); // enable global interrupt
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // enable timer2
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_UltrasonicTask_init */
/**
 * @brief  Function implementing the UltrasonicTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_UltrasonicTask_init */
void triggerUltrasonic_1_Task(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	while(1)
	{
		// Trigger ultrasonic sensor
		//enable trigger for ultrasonic_1
		HAL_GPIO_WritePin(TRIG_PORT_U1, TRIG_PIN_U1,GPIO_PIN_SET);
		for( int i=0;i<80;i++); //delay for 10 us //put no ass operation
		HAL_GPIO_WritePin(TRIG_PORT_U1, TRIG_PIN_U1,GPIO_PIN_RESET);
		osDelay(120);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Task2_init */
/**
 * @brief Function implementing the Task2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task2_init */
void indecateOnLED_Task(void const * argument)
{
	/* USER CODE BEGIN Task2_init */
	vTaskDelay(FIRST_DELAY_U1); // first delay : time between trigger and getting read
	/* Infinite loop */
	for(;;)
	{
		// Check if an object is near

		// if there is something near ultrasonic1 the led will be on
		if(distance_u1<NEAR_OBJECT_THRESHOLD)
		{
			HAL_GPIO_WritePin(LED_PORT_U1, LED_PIN_U1,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(LED_PORT_U1, LED_PIN_U1,GPIO_PIN_RESET);
		}


		//	// if there is something near ultrasonic2 the led will be on
		//	if(distance_u2<NEAR_OBJECT_THRESHOLD)
		//	{
		//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
		//	}
		//	else
		//	{
		//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
		//	}

		osDelay(60);// > 60 ms
	}
	/* USER CODE END Task2_init */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
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
	__disable_irq();
	while (1)
	{
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
