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


#include "stm32f1xx.h"//requied library
#include "stm32f1xx_nucleo.h"//requied library
			
#define ADC_CHANNELS 4 //numbers of ADC channels, 2x2 for joysticks

#define LL_ADC_RESOLUTION_8B (ADC_CR1_RES_1)//setting an 8 bit resolution of ADC

//handlers
DMA_HandleTypeDef dma;
ADC_HandleTypeDef adc;
TIM_HandleTypeDef tim;
GPIO_InitTypeDef gpio;

#ifdef DEBUG
UART_HandleTypeDef uart;//UART for debuging

#endif

//function declaration
long map(long x, long in_min, long in_max, long out_min, long out_max);//map function declaration -> i used function for clearance in code
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void TIM2_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

//global variables
uint16_t adc_value[ADC_CHANNELS];//table for ADC values used with DMA
int counter = 0;//counter used in TIMER_3 interrupt
int joystick_counter = 64-1;//counter used to switch beetween buttons and joystcks
uint16_t buttons[12];//gamecube use 12 buttons:
/*
 * A B X Y
 * UP DOWN LEFT RIGHT
 * R L R_analog L_analog Z
 * START
 */


/*	additional info:
*
*	I used ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_4,ADC_CHANNEL_8
*	Because i wanted to use UART
*
*/
int main(void)
{
    SystemCoreClock = 8000000; //frequency -> 8MHz
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE();//macro for enabling A port
    __HAL_RCC_GPIOB_CLK_ENABLE();//macro for enabling B port
    __HAL_RCC_GPIOC_CLK_ENABLE();//macro for enabling C port

	__HAL_RCC_TIM3_CLK_ENABLE();//macro for enabling TIMER_3

#ifdef DEBUG
    __HAL_RCC_USART2_CLK_ENABLE();//macro for enabling UART port
#endif

    __HAL_RCC_DMA1_CLK_ENABLE();//macro for enabling DMA channel
    __HAL_RCC_ADC1_CLK_ENABLE();//macro for enabling ADC feature



#ifdef DEBUG
    //all configuration stuff for UART

    //UART -> A2 (TX) A3 (RX)
    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_AF_PP;//alternative mode push-pull mode for TX / output line pin
    gpio.Pin = GPIO_PIN_2;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Mode = GPIO_MODE_AF_INPUT;//alternative mode input mode for RX / input line pin
    gpio.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &gpio);


    // UART conviguration
    uart.Instance = USART2;
    uart.Init.BaudRate = 115200;// baudrate/speed -> 115200 (standard speed)
    uart.Init.WordLength = UART_WORDLENGTH_8B;
    uart.Init.Parity = UART_PARITY_NONE;
    uart.Init.StopBits = UART_STOPBITS_1;
    uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart.Init.OverSampling = UART_OVERSAMPLING_16;
    uart.Init.Mode = UART_MODE_TX_RX;// receive (RX) and send/transmit (TX) mode
    HAL_UART_Init(&uart);


    // A5 as output (built in LED)
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOA, &gpio);

#endif

    // A0 , A1, A4 and B0 as "analog" pins -> two per stick
    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &gpio);//A0, A1, A4

    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &gpio);//B0

    // ADC clock configuration
    RCC_PeriphCLKInitTypeDef adc_clk;
    adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk);

    // ADC configuration
    //LL_ADC_RESOLUTION_12B
    adc.Instance = ADC1;
    adc.Init.ContinuousConvMode = ENABLE;//continous mode of mesaurement
    adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc.Init.ScanConvMode = ADC_SCAN_ENABLE;
    adc.Init.NbrOfConversion = ADC_CHANNELS;//in this case the amount is equal 4
    adc.Init.DiscontinuousConvMode = DISABLE;
    adc.Init.NbrOfDiscConversion = 1;
    HAL_ADC_Init(&adc);

    // ADC channels configurations
    ADC_ChannelConfTypeDef adc_ch;
    adc_ch.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    // channel 0 -> A0 <- (ADC12_IN0)
    adc_ch.Channel = ADC_CHANNEL_0;
    adc_ch.Rank = ADC_REGULAR_RANK_1;
    HAL_ADC_ConfigChannel(&adc, &adc_ch);
    // channel 1 -> A1 <- (ADC12_IN1)
    adc_ch.Channel = ADC_CHANNEL_1;
    adc_ch.Rank = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&adc, &adc_ch);
    // channel 4 -> A4 <- (ADC12_IN4)
    adc_ch.Channel = ADC_CHANNEL_4;
    adc_ch.Rank = ADC_REGULAR_RANK_3;
    HAL_ADC_ConfigChannel(&adc, &adc_ch);
    // channel 8 -> B0 <- (ADC12_IN8)
    adc_ch.Channel = ADC_CHANNEL_8;
    adc_ch.Rank = ADC_REGULAR_RANK_4;
    HAL_ADC_ConfigChannel(&adc, &adc_ch);

    HAL_ADCEx_Calibration_Start(&adc);//like the name of fuction says, we calibrate the ADC

    //DMA configuration staff
    dma.Instance = DMA1_Channel1;
    dma.Init.Direction = DMA_PERIPH_TO_MEMORY;//peripheries to memory mode
    dma.Init.PeriphInc = DMA_PINC_DISABLE;//we don't increment the adress of peripheries
    dma.Init.MemInc = DMA_MINC_ENABLE;//we increment adress od DMA memory because we have multiple variables to read
    dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    dma.Init.Mode = DMA_CIRCULAR;//CIRCULAR mode which allow us to read the data all the time, default: DMA_NORMAL
    dma.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&dma);//like the name aplies we initialize our DMA


    __HAL_LINKDMA(&adc, DMA_Handle, dma);//macro which "connect" DMA and ADC

    HAL_ADC_Start_DMA(&adc, (uint32_t*)adc_value, ADC_CHANNELS);//just starting DMA


    //TIMER_3 configuration staff -> now it is set for debug purpose for 1 second loop (target 1 us)
	tim.Instance = TIM3;
	tim.Init.Period = 1000 - 1;
	tim.Init.Prescaler = 8000 - 1;//now our TIMER_3 count to 1000
	tim.Init.ClockDivision = 0;
	tim.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim.Init.RepetitionCounter = 0;
	tim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim);
	HAL_TIM_Base_Start_IT(&tim);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);//enabling TIMER interumpt called NVIC (software interupt)


	//configuration of input interrupts
	gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpio.Pull = GPIO_PULLUP;

	//B1 -> interrupt 1
	gpio.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &gpio);
	//B2 -> interrupt 2
	gpio.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &gpio);
	//C3 -> interrupt 3
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC, &gpio);
	//C4 -> interrupt 4
	gpio.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOC, &gpio);
	//C5 -> interrupt 5
	gpio.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &gpio);
	//C6 -> interrupt 6
	gpio.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOC, &gpio);
	//B7 -> interrupt 7
	gpio.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &gpio);
	//C8 -> interrupt 8
	gpio.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &gpio);
	//C9 -> interrupt 9
	gpio.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &gpio);
	//C10 -> interrupt 10
	gpio.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOC, &gpio);
	//B11 -> interrupt 11
	gpio.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &gpio);
	//B12 -> interrupt 12
	gpio.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &gpio);
	//B13 -> interrupt 13
	gpio.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &gpio);
	//B14 -> interrupt 14
	gpio.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &gpio);
	//B15 -> interrupt 15
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &gpio);


	//enabling all pin interupts
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	//our output/sifnal pin configuraiton
	//note transalte polish coments
	gpio.Pin = GPIO_PIN_3; // konfigurujemy pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjście
	gpio.Pull = GPIO_NOPULL; // rezystory podciągające są wyłączone
	gpio.Speed = GPIO_SPEED_FREQ_HIGH; // wystarczą nieskie częstotliwości przełączania
	HAL_GPIO_Init(GPIOB, &gpio); // inicjalizacja modułu GPIOA



    while (1) {

#ifdef DEBUG

    	for (int i = 0; i < ADC_CHANNELS; i++)
    		printf("ADC%d = %d\n", i, adc_value[i]);//in debug mode we can see what the values are

#endif
    }







}




long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//TIMER_3 interrupt
{
	counter++;//has to be at the begining

	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

	//sending 0 0 0 0
	if ((joystick_counter == 0) || (joystick_counter == 1) || (joystick_counter == 2) || (joystick_counter == 3)) {
		if (counter <= 3){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		}
		else if (counter == 4){//can be change to "else"
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		}

	}
	//for START Y X B A
	else if(joystick_counter>=4 && joystick_counter<=8){
		//button checking might be done using iterator from "joystick_counter"
		if (buttons[joystick_counter - 4]){
			if (counter == 1){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			else if (counter > 1){//can be change to "else"
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}

		}
		else{
			if (counter <= 3){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			else if (counter == 4){//can be change to "else"
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}
		}
	}
	//sending 1 1
	else if (joystick_counter == 9 || joystick_counter== 10 ){
		if (counter ==1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		}
		else if (counter > 1){//can be change to "else"
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		}
	}
	//for L R Z D-Up D-Down D-Right D-Left
	else if(joystick_counter>= 11 && joystick_counter<= 17){
		//button checking might be done using iterator from "joystick_counter"
		if (buttons[joystick_counter - 6]){
			if (counter == 1){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			else if (counter > 1){//can be change to "else"
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}
		}
		else{
			if (counter <= 3){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			else if (counter == 4){//can be change to "else"
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			}
		}
	}




	//has to be at the end
	if (counter == 4){
		counter = 0;
		joystick_counter++;
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//buttons interrupt
{
	//switch for handling all buttons
	switch (GPIO_Pin){

	case GPIO_PIN_0 :{
		if (buttons[0]==0){
			buttons[0]=1;
		}
		else if (buttons[0]==1){
			buttons[0]=0;
		}
	}
	break;

	case GPIO_PIN_1 :{
		if (buttons[1]==0){
			buttons[1]=1;
		}
		else if (buttons[1]==1){
			buttons[1]=0;
		}
	}
	break;

	case GPIO_PIN_2 :{
		if (buttons[2]==0){
			buttons[2]=1;
		}
		else if (buttons[2]==1){
			buttons[2]=0;
		}
	}
	break;

	case GPIO_PIN_3 :{
		if (buttons[3]==0){
			buttons[3]=1;
		}
		else if (buttons[3]==1){
			buttons[3]=0;
		}
	}
	break;

	case GPIO_PIN_4 :{
		if (buttons[4]==0){
			buttons[4]=1;
		}
		else if (buttons[4]==1){
			buttons[4]=0;
		}
	}
	break;

	case GPIO_PIN_5 :{
		if (buttons[5]==0){
			buttons[5]=1;
		}
		else if (buttons[5]==1){
			buttons[5]=0;
		}
	}
	break;

	case GPIO_PIN_6 :{
		if (buttons[6]==0){
			buttons[6]=1;
		}
		else if (buttons[6]==1){
			buttons[6]=0;
		}
	}
	break;

	case GPIO_PIN_7 :{
		if (buttons[7]==0){
			buttons[7]=1;
		}
		else if (buttons[7]==1){
			buttons[7]=0;
		}
	}
	break;

	case GPIO_PIN_8 :{
		if (buttons[8]==0){
			buttons[8]=1;
		}
		else if (buttons[8]==1){
			buttons[8]=0;
		}
	}
	break;

	case GPIO_PIN_9 :{
		if (buttons[9]==0){
			buttons[9]=1;
		}
		else if (buttons[9]==1){
			buttons[9]=0;
		}
	}
	break;

	case GPIO_PIN_10 :{
		if (buttons[10]==0){
			buttons[10]=1;
		}
		else if (buttons[10]==1){
			buttons[10]=0;
		}
	}
	break;

	case GPIO_PIN_11 :{
		if (buttons[11]==0){
			buttons[11]=1;
		}
		else if (buttons[11]==1){
			buttons[11]=0;
		}
	}
	break;

	case GPIO_PIN_12 :{
		if (buttons[12]==0){
			buttons[12]=1;
		}
		else if (buttons[12]==1){
			buttons[12]=0;
		}
	}
	break;

	case GPIO_PIN_13 :{
		if (buttons[13]==0){
			buttons[13]=1;
		}
		else if (buttons[13]==1){
			buttons[13]=0;
		}
	}
	break;

	case GPIO_PIN_14 :{
		if (buttons[14]==0){
			buttons[14]=1;
		}
		else if (buttons[14]==1){
			buttons[14]=0;
		}
	}
	break;

	case GPIO_PIN_15 :{
		if (buttons[15]==0){
			buttons[15]=1;
		}
		else if (buttons[15]==1){
			buttons[15]=0;
		}
	}
	break;

	default :{


	}
	break;


	}
}

void TIM2_IRQHandler(void)
{
 HAL_TIM_IRQHandler(&tim);
}
