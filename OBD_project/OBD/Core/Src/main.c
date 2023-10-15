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
#include "EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//Fuel level part
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



//timer and i2c part
#define PAGE_NUM_TIM_LOC        1
#define PAGE_NUM_FLAG_LOC       2

#define OFFSET_NUM_TIM          1
#define OFFSET_NUM_FLAG         3

#define TIM_SIZE_DATA           4
#define FLAG_SIZE_DATA          1

#define FIRST_TIME             0xFF


// temprature part

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_9

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint32_t adcValue ;


//timer and i2c part
uint32_t g_timeElapsedSec = 0;
uint8_t g_flag ;
uint8_t Done = 0x2;


// temprature part
uint8_t tx_buffer[2];
uint8_t RHI, RHD, TCI, TCD, SUM;
uint32_t pMillis, cMillis;
float tCelsius = 0;
float tFahrenheit = 0;
float RH = 0;
uint8_t TFI = 0;
uint8_t TFD = 0;
char strCopy[15];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

uint32_t convertDigitalToLevel(uint32_t digitalValue);
void mapDigitalvalueToLeds(uint32_t digitalValue);

//timer and i2c part
void EEPROM_voidGetStoredTime(void);
uint32_t GetElapsedTime(void);


// temprature part
void microDelay (uint16_t delay);
uint8_t DHT11_Start (void);
uint8_t DHT11_Read (void);
void DHT11_TempRead(void);
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	//timer and i2c part
	HAL_I2C_Init(&hi2c1);
	EEPROM_voidGetStoredTime();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start(&htim1);
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


		DHT11_TempRead();


		HAL_Delay(1000);

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000000;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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


//timer and i2c part
uint32_t GetElapsedTime(void){
	uint32_t ElapsedTime = 0 ;
	EEPROM_Read(PAGE_NUM_TIM_LOC, OFFSET_NUM_TIM, (uint8_t *)&ElapsedTime, TIM_SIZE_DATA);
	return ElapsedTime;
}

void EEPROM_voidGetStoredTime(void){

	//	for(uint8_t i = 0 ; i < 128 ; i++){
	//				EEPROM_PageErase(i);
	//			}
	//#ifdef	a

	EEPROM_Read(PAGE_NUM_FLAG_LOC, OFFSET_NUM_FLAG, &g_flag, FLAG_SIZE_DATA);
	//HAL_Delay(15);
	if(g_flag != FIRST_TIME){

		EEPROM_Read(PAGE_NUM_TIM_LOC, OFFSET_NUM_TIM, (uint8_t *)&g_timeElapsedSec, TIM_SIZE_DATA);
	}
	else{
		/* It's First time */
		/* Erase our used loc in EEPROM to make sure that is clear */
		for(uint8_t i = 0 ; i < 128 ; i++){
			EEPROM_PageErase(i);
		}
		EEPROM_Write(PAGE_NUM_FLAG_LOC, OFFSET_NUM_FLAG,&Done, FLAG_SIZE_DATA);
	}
	//#endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
		//Write Code here
		g_timeElapsedSec += 10;
		EEPROM_Write(PAGE_NUM_TIM_LOC, OFFSET_NUM_TIM,(uint8_t *)&g_timeElapsedSec,TIM_SIZE_DATA);
	}

}


// temprature part
void microDelay (uint16_t delay)
{

	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}



uint8_t DHT11_Start (void)
{
	uint8_t Response = 0;
	GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
	GPIO_InitStructPrivate.Pin = DHT11_PIN;
	GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as output

	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	HAL_Delay(20);   // wait for 20ms  ,To initiate communication, the microcontroller sends a start signal to the DHT11 sensor. This start signal consists of a LOW signal on the data pin for at least 18 milliseconds. This LOW signal informs the sensor that it should prepare to transmit data.
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
	microDelay (30);   // wait for 30us , After receiving the start signal, the DHT11 sensor responds by pulling the data line LOW for about 20-40 microseconds to acknowledge the start signal.
	GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as input
	microDelay (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		microDelay (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) {

			Response = 1;
		}
	}
	pMillis = HAL_GetTick();
	cMillis = HAL_GetTick();
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
	{
		cMillis = HAL_GetTick();
	}
	return Response;
}


uint8_t DHT11_Read (void)
{
	uint8_t a,b;
	for (a=0;a<8;a++)
	{
		pMillis = HAL_GetTick();
		cMillis = HAL_GetTick();
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
		{  // wait for the pin to go high
			cMillis = HAL_GetTick();
		}
		microDelay (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
			b&= ~(1<<(7-a));
		else
			b|= (1<<(7-a));
		pMillis = HAL_GetTick();
		cMillis = HAL_GetTick();
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
		{  // wait for the pin to go low
			cMillis = HAL_GetTick();
		}
	}

	return b;
}


void DHT11_TempRead(void)
{
	if(DHT11_Start())
	{
		RHI = DHT11_Read(); // Relative humidity integral
		RHD = DHT11_Read(); // Relative humidity decimal
		TCI = DHT11_Read(); // Celsius integral
		TCD = DHT11_Read(); // Celsius decimal
		SUM = DHT11_Read(); // Check sum
		if (RHI + RHD + TCI + TCD == SUM)
		{
			// Can use RHI and TCI for any purposes if whole number only needed
			tCelsius = (float)TCI + (float)(TCD/10.0);
			//tFahrenheit = tCelsius * 9/5 + 32;
			//RH = (float)RHI + (float)(RHD/10.0);
			// Can use tCelsius, tFahrenheit and RH for any purposes
			//TFI = tFahrenheit;  // Fahrenheit integral
			//TFD = tFahrenheit*10-TFI*10; // Fahrenheit decimal

			tx_buffer[0]=TCI/10+48;
			tx_buffer[1]=TCI%10+48;

			HAL_UART_Transmit(&huart1,& tx_buffer[0], 1, 10);
			HAL_UART_Transmit(&huart1,& tx_buffer[1], 1, 10);


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
