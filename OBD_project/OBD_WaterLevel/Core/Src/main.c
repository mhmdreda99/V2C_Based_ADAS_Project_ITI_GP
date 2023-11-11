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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FUEL_SENSOR_PIN				GPIO_PIN_0
#define FUEL_SENSOR_PORT			GPIOA

#define FUEL_LED0_PIN				GPIO_PIN_1
#define FUEL_LED0_PORT				GPIOA

#define FUEL_LED1_PIN				GPIO_PIN_2
#define FUEL_LED1_PORT				GPIOA

#define FUEL_LED2_PIN				GPIO_PIN_3
#define FUEL_LED2_PORT				GPIOA

#define FUEL_LED3_PIN				GPIO_PIN_4
#define FUEL_LED3_PORT				GPIOA

#define FUEL_MAX_DIGITA_VALUE		2000
#define FUEL_MINIMUM_DIGITA_VALUE	180

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */
uint32_t adcValue ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
uint32_t convertDigitalToLevel(uint32_t digitalValue);
void mapDigitalvalueToLeds(uint32_t digitalValue);

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
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		adcValue = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		mapDigitalvalueToLeds(adcValue);
		HAL_Delay(500);
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
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

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

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 PA2 PA3 PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void mapDigitalvalueToLeds(uint32_t digitalValue){
	//empty tank
	if (digitalValue <= FUEL_MINIMUM_DIGITA_VALUE){
		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_SET);
	}
	//quarter tank
	else if (digitalValue <= (FUEL_MAX_DIGITA_VALUE - FUEL_MINIMUM_DIGITA_VALUE)/4){
		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);
	}
	//Half tank
	else if (digitalValue <= (FUEL_MAX_DIGITA_VALUE - FUEL_MINIMUM_DIGITA_VALUE)/2){
		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);
	}
	//3 quarters tank
	else if (digitalValue <= (FUEL_MAX_DIGITA_VALUE - FUEL_MINIMUM_DIGITA_VALUE)/4*3){


		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
				HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);
	}
	//Full tank
	else if (digitalValue <= (FUEL_MAX_DIGITA_VALUE - FUEL_MINIMUM_DIGITA_VALUE)){


		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);
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
