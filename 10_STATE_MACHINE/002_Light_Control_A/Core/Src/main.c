/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stdio.h"
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
UART_HandleTypeDef hlpuart1;

TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */

enum event{
	BLINK,
	ON,
	OFF,
	SCROLL
};

typedef enum{
	LIGHTS_OFF,
	LIGHT_BLINK,
	LIGHTS_ON,
	LIGHTS_SCROLL
}light_state_t;

light_state_t curr_state = LIGHTS_OFF;

uint8_t led_index = 0;
uint8_t rx_buffer;
uint8_t rx_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */
void lights_on();
void lights_blink();
void lights_off();
void lights_scroll();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* state machine function
 *
 * outer switch case is for state transition
 * inner switch case is for event transition  */
void light_state_machine(uint8_t event)
{
	switch (curr_state) {
		case LIGHTS_OFF:{
			switch (event) {
				case BLINK:{
					curr_state = LIGHT_BLINK;			//change the current state
					HAL_TIM_Base_Start_IT(&htim16);		//start timer for led blinking
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case ON:{
					lights_on();
					curr_state = LIGHTS_ON;				//change the current state
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case SCROLL:{
					curr_state = LIGHTS_SCROLL;
					HAL_TIM_Base_Start_IT(&htim17);		//start the timer for led scrolling
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					break;
				}
			}
			break;
		}

		case LIGHT_BLINK:{
			switch (event) {
				case ON:{
					lights_on();
					curr_state = LIGHTS_ON;				//change the current state
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case OFF:{
					lights_off();
					curr_state = LIGHTS_OFF;			//change the current state
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case SCROLL:{
					curr_state = LIGHTS_SCROLL;
					HAL_TIM_Base_Start_IT(&htim17);		//start the timer for led scrolling
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					break;
				}
			}
			break;
		}

		case LIGHTS_ON:{
			switch (event) {
				case BLINK:{
					curr_state = LIGHT_BLINK;			//change the current state
					HAL_TIM_Base_Start_IT(&htim16);		//start timer for led blinking
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}

				case OFF:{
					lights_off();
					curr_state = LIGHTS_OFF;			//change the current state
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					break;
				}
				case SCROLL:{
					curr_state = LIGHTS_SCROLL;
					HAL_TIM_Base_Start_IT(&htim17);		//start the timer for led scrolling
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					break;
				}
			}
			break;
		}

		case LIGHTS_SCROLL:{
			switch (event) {
				case ON:{
					lights_on();
					curr_state = LIGHTS_ON;				//change the current state
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case OFF:{
					lights_off();
					curr_state = LIGHTS_OFF;			//change the current state
					HAL_TIM_Base_Stop_IT(&htim16);		//stop the timer
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
				case BLINK:{
					curr_state = LIGHT_BLINK;			//change the current state
					HAL_TIM_Base_Start_IT(&htim16);		//start timer for led blinking
					HAL_TIM_Base_Stop_IT(&htim17);		//stop the timer
					break;
				}
			}
			break;
		}
	}
}

/* led blinking function */
void lights_blink()
{
	HAL_GPIO_TogglePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin);
	HAL_GPIO_TogglePin(BSP_USER_LED_2_GPIO_Port, BSP_USER_LED_2_Pin);
	HAL_GPIO_TogglePin(BSP_USER_LED_3_GPIO_Port, BSP_USER_LED_3_Pin);
}

/* function for turning leds on */
void lights_on()
{
	HAL_GPIO_WritePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin,
			GPIO_PIN_SET);
	HAL_GPIO_WritePin(BSP_USER_LED_2_GPIO_Port, BSP_USER_LED_2_Pin,
			GPIO_PIN_SET);
	HAL_GPIO_WritePin(BSP_USER_LED_3_GPIO_Port, BSP_USER_LED_3_Pin,
			GPIO_PIN_SET);
}

/* function for turning leds off */
void lights_off()
{
	HAL_GPIO_WritePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BSP_USER_LED_2_GPIO_Port, BSP_USER_LED_2_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BSP_USER_LED_3_GPIO_Port, BSP_USER_LED_3_Pin,
			GPIO_PIN_RESET);
}

void lights_scroll()
{
	HAL_GPIO_WritePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BSP_USER_LED_2_GPIO_Port, BSP_USER_LED_2_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BSP_USER_LED_3_GPIO_Port, BSP_USER_LED_3_Pin,
			GPIO_PIN_RESET);

	switch (led_index) {
	case 0:
		HAL_GPIO_WritePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin,
				GPIO_PIN_SET);  // Turn on LED1
		break;
	case 1:
		HAL_GPIO_WritePin(BSP_USER_LED_2_GPIO_Port, BSP_USER_LED_2_Pin,
				GPIO_PIN_SET); // Turn on LED2
		break;
	case 2:
		HAL_GPIO_WritePin(BSP_USER_LED_3_GPIO_Port, BSP_USER_LED_3_Pin,
				GPIO_PIN_SET);  // Turn on LED3
		break;
	}

	led_index = (led_index + 1) % 3;  // Move to the next LED

}

/* this function wll call after every 1 second due to which leds can blink continuously */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM16)
	{
		if (curr_state == LIGHT_BLINK)
		{
			lights_blink();
		}

	}
	else if(htim->Instance == TIM17)
	{
		if(curr_state == LIGHTS_SCROLL)
		{
			lights_scroll();
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
  MX_LPUART1_UART_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim16);
  HAL_TIM_Base_Start_IT(&htim17);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /* event dispatcher using uart
	   * receive the command through uart due to which respective event will generate */
	  if(HAL_UART_Receive(&hlpuart1, &rx_buffer, 1, HAL_MAX_DELAY) == HAL_OK)
	  {
//		  rx_data = rx_buffer;
//		  printf("received data is: %d\r\n", rx_data);


		  if(rx_buffer == 'b')
		  {
			  light_state_machine(BLINK);
		  }
		  else if(rx_buffer == 'o')
		  {
			  light_state_machine(ON);
		  }
		  else if(rx_buffer == 's')
		  {
			  light_state_machine(SCROLL);
		  }
		  else if(rx_buffer == 'f')
		  {
			  light_state_machine(OFF);
		  }
	  }


//	  else if(HAL_GPIO_ReadPin(BSP_USER_BUTTON_GPIO_Port, BSP_USER_BUTTON_Pin)== GPIO_PIN_SET)
//	  {
//		  stopblink = 1;
//		  light_state_machine(OFF);
//	  }
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 64000-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 500-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 64000-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 200-1;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BSP_USER_LED_3_Pin|BSP_USER_LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BSP_USER_LED_1_GPIO_Port, BSP_USER_LED_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BSP_USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = BSP_USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BSP_USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BSP_USER_LED_3_Pin BSP_USER_LED_2_Pin */
  GPIO_InitStruct.Pin = BSP_USER_LED_3_Pin|BSP_USER_LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BSP_USER_LED_1_Pin */
  GPIO_InitStruct.Pin = BSP_USER_LED_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BSP_USER_LED_1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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