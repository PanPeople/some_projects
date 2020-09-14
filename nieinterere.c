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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/*
 ******************************************************************************
 * @file    main.c
 * @author  Mikołaj Czerniak
 * @version V0.1
 * @date    26.06.2020
 * @brief   Simple "SMT32 to GameCube Controller"
 ******************************************************************************
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * mikolajek26@gmail.com wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Mikołaj Czerniak
 * ----------------------------------------------------------------------------
 *
 * This code was my first attemt to make semi-proffesional piece of software
 * for uC so please be kind in reporting all bugs.
 *
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
//GCC standard commands
#define SI_CMD_ID       0x00
#define SI_CMD_POLL     0x40
#define SI_CMD_ORIGINS  0x41
#define SI_CMD_CALIB    0x42
#define SI_CMD_RESET    0xFF
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t adc_value[4];//table for ADC values used with DMA
uint16_t receivedMessage[8];
uint16_t receivedMessageLength = 0;
uint16_t bite_counter = 0;
uint16_t byte_counter = 0;

uint16_t buttons[14];//gamecube use 12 buttons + "analog" R and L:
uint8_t GC_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//buttons state request
uint8_t GC_data_message[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//messages request
uint8_t DMA_data[82]= { 0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0};

uint8_t justZero =0;

uint16_t IC_Val1 = 0;
uint16_t IC_Val2 = 0;
uint16_t received_value = 0;
uint16_t bit_value = 0 ;


uint8_t RxData[3];

uint8_t counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */


uint8_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint8_t out_min, uint8_t out_max);//map function declaration -> i used function for clearance in code
void adcConversion();
void buttonsCheck();
void bitsArray();

void sendData(uint8_t lenght, uint8_t* msg);//conversion of bytes array into

void buttonMessageChange(); //depending on 2 byte from RxData, it is RxData[1], we have to make conversion of last 4 bytes

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


// write the input capture callback

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // falling edge interrupt
	{
		// read captured value
		IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // first value

		if (IC_Val1 != 0)  // if the value is not 0
		{
			// read second value
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // rising edge value

			// calculate what kind of bit was received

			received_value = (IC_Val2*100/IC_Val1);

			if (received_value == 75  ){
				bit_value = 0;
			}

			else{ // received_value < 75
				bit_value = 1;
			}

			bite_counter ++;
			if (bite_counter == 8){
				byte_counter++;
			}

			//putting bit_value into bytes array
			receivedMessage[byte_counter] += bit_value;
			receivedMessage[byte_counter] <<= 1;
			receivedMessageLength = byte_counter + 1;



		}

		else
		{
			bit_value = 0;
		}
	}
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	 switch (htim->Channel) {

		 case HAL_TIM_ACTIVE_CHANNEL_1:{
			 // tu nadamy sygnał
			 HAL_TIM_Base_Stop_IT(&htim3);//stop timer interupts to not reading data from MOSI

			 switch(receivedMessage[0]){
			 	 case SI_CMD_ID :
			 	 case SI_CMD_RESET :
			 		 GC_data_message[0] = 0x09;
			 		 GC_data_message[1] = 0x00;
			 		 GC_data_message[2] = 0x03;

			 		 sendData(3, GC_data_message);
			 		 //send_data


			 	break;

			 	 case SI_CMD_POLL :{
			 		buttonMessageChange();
			 		sendData(8, GC_data);

			 		 break;
			 	 }
			 	 case SI_CMD_ORIGINS :{

			 		 break;
			 	 }
			 	 case SI_CMD_CALIB :{
			 		 GC_data_message[0] = 0x00;
			 		 GC_data_message[1] = 0x80;
			 		 GC_data_message[2] = 0x80;
			 		 GC_data_message[3] = 0x80;
			 		 GC_data_message[4] = 0x80;
			 		 GC_data_message[5] = 0x80;
			 		 GC_data_message[6] = 0x00;
			 		 GC_data_message[7] = 0x00;
			 		 GC_data_message[8] = 0x00;
			 		 GC_data_message[9] = 0x00;
			 		 sendData(10, GC_data_message)
			 		 //send_data
			 		 break;
			 	 }

			 	 default:{

			 		 break;
			 	 }


			 }

			 break;
		 }

		 default:{
			 break;
		 }

	 }
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
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3); //do wylacanie: HAL_TIM_Base_Stop_IT
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 6);//if nothing happened after 6us it means there is no received data and start sending data
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, justZero, 1);//sending only 0
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	adcConversion();
	buttonsCheck();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 64-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4-3;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 64-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xffff;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 PC6 PC8
                           PC9 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB13 PB14 PB15
                           PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void adcConversion(){
	// conversion for Joystick X  Joystick Y  C-Stick X  C-Stick Y

	GC_data[2] = map(adc_value[0],0,4095,0,255);
	GC_data[3] = map(adc_value[1],0,4095,0,255);
	GC_data[4] = map(adc_value[2],0,4095,0,255);
	GC_data[5] = map(adc_value[3],0,4095,0,255);

}

uint8_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint8_t out_min, uint8_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//please check
void buttonsCheck(){//ta funkcja ma mi wypełnić 2 bajty stanami przycisków oraz bajt "7 i 8" wypełniony 1 (no zapełniony bajt) dla wciśniętego

	GC_data[0] = 0x00;
	GC_data[1] = 0b10000000;
	GC_data[6] = 0x00;
	GC_data[7] = 0x00;

	uint8_t mask = 0b00100000;

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET){
		 GC_data[0] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET){
		 GC_data[0] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET){
		 GC_data[0] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET){
		 GC_data[0] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET){
		 GC_data[0] += (mask >>= 1) ;
	 }
	 else{

	 }
	 mask = 0b10000000;

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET){
		 GC_data[1] += (mask >>= 1) ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_RESET){
		 GC_data[6] = 0xff ;
	 }
	 else{

	 }

	 if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_RESET){
		 GC_data[7] = 0xff ;
	 }
	 else{

	 }


}

void buttonMessageChange(){

	uint8_t ra = GC_data[6];
	uint8_t la = GC_data[7];

	 switch (RxData[1]) {
	        case 0:
	        case 5:
	        case 6:
	        case 7:
	            //I swear Microchip, I hate you -> Aurelio words
	            GC_data[6] = (uint8_t)((uint8_t)(la & 0xF0U) | (uint8_t)(ra >> 4U));
	            GC_data[7] = 0x00; //Analog A/B
	        break;

	        case 1:
	        	GC_data[4] = (uint8_t)((uint8_t)(GC_data[4] & 0xF0U) | (uint8_t)(GC_data[5] >> 4U));
	        	GC_data[5] = la;
	        	GC_data[6] = ra;
	            GC_data[7] = 0x00; //Analog A/B
	        break;

	        case 2:
	        	GC_data[4] = (uint8_t)((uint8_t)(GC_data[4] & 0xF0U) | (uint8_t)(GC_data[5] >> 4U));
	        	GC_data[5] = (uint8_t)((uint8_t)(la & 0xF0U) | (uint8_t)(ra >> 4U));
	        	GC_data[6] = 0x00; //Analog A
	        	GC_data[7] = 0x00; //Analog B
	        break;

	        case 3:

	        	GC_data[6] = la;
	            GC_data[7] = ra;
	        break;

	        case 4:
	        	GC_data[4] = GC_data[4];
	        	GC_data[5] = GC_data[5];
	        	GC_data[6] = 0x00; //Analog A
	        	GC_data[7] = 0x00; //Analog B
	        break;
	    }
}

//please check
void sendData(uint8_t lenght, uint8_t* msg){//dostaje tablice jedno bajtowe i trzeba je pociąć na pojedyńcze bity jedne po drugim
	for (int i = 0 ; i< 66; i++){
		DMA_data[i] = 0;
	}
	uint8_t j =0;
	for (uint8_t i =0 ; i< lenght; i++){


		//j = 0;
		for(uint8_t bit_mask = 1 << (8-1); bit_mask > 0; bit_mask >>= 1){

			if (bit_mask & msg[i]){
				DMA_data[j] = 1;
			}
			else{
				DMA_data[j] = 3;
			}

			j++;
		}
	}

	DMA_data[j] = 1; //stop bit "1 " at the end
	DMA_data[j+1] = 0;//stan wysoki po skończonym wysyle
	//trza je wysłać
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, DMA_data, j+2);//j+2 or (length*2 +2)

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
