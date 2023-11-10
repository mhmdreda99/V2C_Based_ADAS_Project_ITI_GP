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
#include "EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


//timer and eeprom
#define PAGE_NUM_TIM_LOC        	1
#define PAGE_NUM_FLAG_LOC       	100

#define OFFSET_NUM_TIM          	1
#define OFFSET_NUM_FLAG         	3

#define TIM_SIZE_DATA           	4
#define FLAG_SIZE_DATA          	1

#define FIRST_TIME             		0xFF

//ADC and fuel level
#define FUEL_SENSOR_PIN				GPIO_PIN_0
#define FUEL_SENSOR_PORT			GPIOA

#define FUEL_LED0_PIN				GPIO_PIN_1
#define FUEL_LED0_PORT				GPIOA

#define FUEL_LED1_PIN				GPIO_PIN_4
#define FUEL_LED1_PORT				GPIOA

#define FUEL_LED2_PIN				GPIO_PIN_6
#define FUEL_LED2_PORT				GPIOA

#define FUEL_LED3_PIN				GPIO_PIN_7
#define FUEL_LED3_PORT				GPIOA

#define FUEL_MAX_DIGITA_VALUE		1300
#define FUEL_MINIMUM_DIGITA_VALUE	180

#define FUEL_LEVEL_FULL_TANK		4
#define FUEL_LEVEL_THREE_Q_TANK		3
#define FUEL_LEVEL_HALF_TANK		2
#define FUEL_LEVEL_QUARTER_TANK		1
#define FUEL_LEVEL_EMPTY_TANK		0

//lm35 temprature part
#define LM35_SENSOR_PIN				GPIO_PIN_5
#define LM35_SENSOR_PORT			GPIOA

#define LM35_MAX_DIGITA_VALUE		4096
#define LM35_MINIMUM_DIGITA_VALUE	0

#define LM35_MAX_TEMPRATURE			150
#define LM35_MINIMUM_TEMPRATURE		2

#define LM35_MAX_OUT_VOLt			1480 	//in mv
#define LM35_MINIMUM_OUT_VOLT		0


// sending data to RPI
#define RPI_TRIG_PIN 				12
#define RPI_TRIG_PORT 				GPIOB

//BSA
#define BSA_IS_OK					51
#define BSA_ONT_OK					58
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Definitions for tim_Task1 */
osThreadId_t tim_Task1Handle;
const osThreadAttr_t tim_Task1_attributes = {
  .name = "tim_Task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for timTask2 */
osThreadId_t timTask2Handle;
const osThreadAttr_t timTask2_attributes = {
  .name = "timTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for fuelLevelTask1 */
osThreadId_t fuelLevelTask1Handle;
const osThreadAttr_t fuelLevelTask1_attributes = {
  .name = "fuelLevelTask1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for fuelLevelTask2 */
osThreadId_t fuelLevelTask2Handle;
const osThreadAttr_t fuelLevelTask2_attributes = {
  .name = "fuelLevelTask2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LM35Task0 */
osThreadId_t LM35Task0Handle;
const osThreadAttr_t LM35Task0_attributes = {
  .name = "LM35Task0",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RPI_task1 */
osThreadId_t RPI_task1Handle;
const osThreadAttr_t RPI_task1_attributes = {
  .name = "RPI_task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BSA_task0 */
osThreadId_t BSA_task0Handle;
const osThreadAttr_t BSA_task0_attributes = {
  .name = "BSA_task0",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

//time and eeprom
uint32_t g_timeElapsedSec = 0;
uint8_t g_flag ;
uint8_t Done = 0x2;

// fuel level
uint32_t adcFuelValue ;
uint8_t curretFuelValue ;


// lm35 temprature
uint16_t ADC_TempratureDigitalRead ;
uint8_t  temperatureInCelsius ;
uint16_t  temperatureInMVolt ;

/*10 bit are sent to rpi
 * fule level 		> one bit 		0
 * BSA diagnlose	> two bits		1,2
 * temprature 		> three bits	3,4,5
 * time 			> four bits 	6,7,8,9
 *
 */
uint8_t rpi_data[9] = {FUEL_LEVEL_FULL_TANK,BSA_IS_OK,51,52,53,54,55,56,57};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void IncTimFunc(void *argument);
void EEPROM_saveTimVal(void *argument);
void getFuelLevelFromADC(void *argument);
void mapFuelLevelDigtalToLED(void *argument);
void LM35_getTemprature(void *argument);
void trigSendRPI(void *argument);
void renableUART_DMA(void *argument);

/* USER CODE BEGIN PFP */
// ADCs
// make sure that num of conversions is 1
void ADC_select_CH0();
void ADC_select_CH5();
//time and eeprom
void EEPROM_voidInitTime(void);
uint32_t GetElapsedTime(void);

//fuel level
void mapDigitalvalueToLeds(uint32_t digitalValue);

//lm35 temprature

void getDigitalTemperatureFromADC ();
void mapDigitalTemperatureToVolt();
void mapMVoltToCelsius();

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_I2C_Init(&hi2c1);
	EEPROM_voidInitTime();

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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
  /* creation of tim_Task1 */
  tim_Task1Handle = osThreadNew(IncTimFunc, NULL, &tim_Task1_attributes);

  /* creation of timTask2 */
  timTask2Handle = osThreadNew(EEPROM_saveTimVal, NULL, &timTask2_attributes);

  /* creation of fuelLevelTask1 */
  fuelLevelTask1Handle = osThreadNew(getFuelLevelFromADC, NULL, &fuelLevelTask1_attributes);

  /* creation of fuelLevelTask2 */
  fuelLevelTask2Handle = osThreadNew(mapFuelLevelDigtalToLED, NULL, &fuelLevelTask2_attributes);

  /* creation of LM35Task0 */
  LM35Task0Handle = osThreadNew(LM35_getTemprature, NULL, &LM35Task0_attributes);

  /* creation of RPI_task1 */
  RPI_task1Handle = osThreadNew(trigSendRPI, NULL, &RPI_task1_attributes);

  /* creation of BSA_task0 */
  BSA_task0Handle = osThreadNew(renableUART_DMA, NULL, &BSA_task0_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
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
  hadc1.Init.ScanConvMode = ENABLE;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

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
  huart1.Init.BaudRate = 9600;
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
  huart2.Init.BaudRate = 9600;
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pins : PA1 PA4 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//selecting adc chunnel

/*
 * steps
 * 1- comment in init
 * 2- set rank of all to 1
 * 3- set n of conversions = 1    << most impo
 * */
void ADC_select_CH0(void){
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}


}

void ADC_select_CH5(void){
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


//time and eeprom  no RTOS
uint32_t GetElapsedTime(void){
	uint32_t l_ElapsTim = g_timeElapsedSec;
	return l_ElapsTim;
}
/* Read Flag value */
void EEPROM_voidInitTime(void){
	EEPROM_Read(PAGE_NUM_FLAG_LOC, OFFSET_NUM_FLAG, &g_flag, FLAG_SIZE_DATA);

	HAL_Delay(5);
	if(g_flag != FIRST_TIME){

		EEPROM_Read(PAGE_NUM_TIM_LOC, OFFSET_NUM_TIM, (uint8_t *)&g_timeElapsedSec, TIM_SIZE_DATA);
	}
	else{
		/* It's First time */
		/* Erase our used loc in EEPROM to make sure that is clear */
		for(uint8_t i = 0 ; i < 7 ; i++){
			EEPROM_PageErase(i);
		}
		EEPROM_Write(PAGE_NUM_FLAG_LOC, OFFSET_NUM_FLAG,&Done, FLAG_SIZE_DATA);
	}
}


//fuel part no RTOS
void mapDigitalvalueToLeds(uint32_t digitalValue){
	//full tank
	if (digitalValue > FUEL_MAX_DIGITA_VALUE){

		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);

		curretFuelValue = FUEL_LEVEL_FULL_TANK;
	}
	//three quarter tank
	else if (digitalValue > (FUEL_MAX_DIGITA_VALUE*3)/4){

		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);

		curretFuelValue = FUEL_LEVEL_THREE_Q_TANK;
	}
	//Half tank
	else if (digitalValue > (FUEL_MAX_DIGITA_VALUE )/2){

		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);

		curretFuelValue = FUEL_LEVEL_HALF_TANK;
	}
	// quarter tank
	else if (digitalValue > (FUEL_MAX_DIGITA_VALUE /4)){

		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_RESET);

		curretFuelValue = FUEL_LEVEL_QUARTER_TANK;
	}
	//empty tank
	else {

		HAL_GPIO_WritePin(FUEL_LED0_PORT, FUEL_LED0_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED1_PORT, FUEL_LED1_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED2_PORT, FUEL_LED2_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(FUEL_LED3_PORT, FUEL_LED3_PIN,GPIO_PIN_SET);

		curretFuelValue = FUEL_LEVEL_EMPTY_TANK;
	}
}

//lm 35 temprater no RTOS
void getDigitalTemperatureFromADC (){

	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, 100);
	ADC_TempratureDigitalRead = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
}
void mapDigitalTemperatureToVolt() {
	temperatureInMVolt = (ADC_TempratureDigitalRead*(LM35_MAX_OUT_VOLt-LM35_MINIMUM_OUT_VOLT))/(LM35_MAX_DIGITA_VALUE)+LM35_MINIMUM_OUT_VOLT;
}

void mapMVoltToCelsius() {
	temperatureInCelsius = (temperatureInMVolt*(LM35_MAX_TEMPRATURE-LM35_MINIMUM_TEMPRATURE))/(LM35_MAX_OUT_VOLt-LM35_MINIMUM_OUT_VOLT) + LM35_MINIMUM_TEMPRATURE ;
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_IncTimFunc */
/**
 * @brief  Function implementing the Tim_Task1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_IncTimFunc */
void IncTimFunc(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		g_timeElapsedSec += 2;

		osDelay(2000);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_EEPROM_saveTimVal */
/**
 * @brief Function implementing the TimTask2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_EEPROM_saveTimVal */
void EEPROM_saveTimVal(void *argument)
{
  /* USER CODE BEGIN EEPROM_saveTimVal */
	/* Infinite loop */
	for(;;)
	{
		EEPROM_Write(PAGE_NUM_TIM_LOC, OFFSET_NUM_TIM,(uint8_t *)&g_timeElapsedSec,TIM_SIZE_DATA);
		//uint32_t timeInMins = g_timeElapsedSec/60;
		uint8_t tim0 = (g_timeElapsedSec%10 ) 		+ 48;
		uint8_t tim1 = (g_timeElapsedSec%100) /10 	+ 48;
		uint8_t tim2 = (g_timeElapsedSec%1000)/100 	+ 48;
		uint8_t tim3 = (g_timeElapsedSec/1000) 		+ 48;

		rpi_data[8] = tim0;
		rpi_data[7] = tim1;
		rpi_data[6] = tim2;
		rpi_data[5] = tim3;

		osDelay(5000);
	}
  /* USER CODE END EEPROM_saveTimVal */
}

/* USER CODE BEGIN Header_getFuelLevelFromADC */
/**
 * @brief Function implementing the fuelLevelTask1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_getFuelLevelFromADC */
void getFuelLevelFromADC(void *argument)
{
  /* USER CODE BEGIN getFuelLevelFromADC */
	osDelay(50);
	/* Infinite loop */
	for(;;)
	{
		ADC_select_CH0();
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		adcFuelValue = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		osDelay(2000);
	}
  /* USER CODE END getFuelLevelFromADC */
}

/* USER CODE BEGIN Header_mapFuelLevelDigtalToLED */
/**
 * @brief Function implementing the fuelLevelTask2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_mapFuelLevelDigtalToLED */
void mapFuelLevelDigtalToLED(void *argument)
{
  /* USER CODE BEGIN mapFuelLevelDigtalToLED */
	osDelay(100);

	/* Infinite loop */

	for(;;)
	{
		mapDigitalvalueToLeds(adcFuelValue);

		rpi_data[0] = curretFuelValue + 48;
		osDelay(2000);
	}
  /* USER CODE END mapFuelLevelDigtalToLED */
}

/* USER CODE BEGIN Header_LM35_getTemprature */
/**
 * @brief Function implementing the LM35Task0 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_LM35_getTemprature */
void LM35_getTemprature(void *argument)
{
  /* USER CODE BEGIN LM35_getTemprature */
	/* Infinite loop */
	for(;;)
	{
		ADC_select_CH5();
		//getting temperature digital value from adc
		getDigitalTemperatureFromADC();
		//mapping Digital temperature to volt
		mapDigitalTemperatureToVolt();
		//mapping volt to celsius
		mapMVoltToCelsius();

		// maping temp to 3 digits
		uint8_t temp0 = (temperatureInCelsius%10 ) 		+ 48;
		uint8_t temp1 = (temperatureInCelsius%100) /10 	+ 48;
		uint8_t temp2 = (temperatureInCelsius/100) 		+ 48;

		rpi_data[4]   = temp0;
		rpi_data[3]   = temp1;
		rpi_data[2]   = temp2;
		osDelay(2000);
	}
  /* USER CODE END LM35_getTemprature */
}

/* USER CODE BEGIN Header_trigSendRPI */
/**
 * @brief Function implementing the RPI_task1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_trigSendRPI */
void trigSendRPI(void *argument)
{
  /* USER CODE BEGIN trigSendRPI */
	/* Infinite loop */
	for(;;)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

		HAL_UART_Transmit(&huart1, &rpi_data[0], 9, 10);
		// uint8_t me = 'A';
		// HAL_UART_Transmit(&huart1, &me, 1, 100);
		osDelay(5000);
	}
  /* USER CODE END trigSendRPI */
}

/* USER CODE BEGIN Header_renableUART_DMA */
/**
* @brief Function implementing the BSA_task0 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_renableUART_DMA */
void renableUART_DMA(void *argument)
{
	vTaskDelay(240);
  /* USER CODE BEGIN renableUART_DMA */
  /* Infinite loop */
  for(;;)
  {

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, &rpi_data[1], 1);
    osDelay(2000);
  }
  /* USER CODE END renableUART_DMA */
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
