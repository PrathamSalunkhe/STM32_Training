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
#include "string.h"
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
#define IDLE      &protimer_state_handler_IDLE
#define TIME_SET  &protimer_state_handler_TIME_SET
#define COUNTDOWN &protimer_state_handler_COUNTDOWN
#define PAUSE     &protimer_state_handler_PAUSE
#define STAT      &protimer_state_handler_STAT
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;

TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */

struct protimer_tag;
struct event_tag;


/* signals of the application */
typedef enum {
	INC_TIME,
	DEC_TIME,
	TIME_TICK,
	START_PAUSE,
	ABRT,
	/* Internal activity signals */
	ENTRY,
	EXIT
}protimer_signal_t;

typedef enum{
	EVENT_HANDLED,
	EVENT_IGNORED,
	EVENT_TRANSITION
}event_status_t;

typedef event_status_t (*protimer_state_t)(struct protimer_tag  *const, struct event_tag const *const);

/* main application struct */
typedef struct protimer_tag{
	uint32_t curr_time;
	uint32_t elapsed_time;
	uint32_t pro_time;
	protimer_state_t active_state;
}protimer_t;

/* generic(super) event structure */
typedef struct event_tag{
	uint8_t sig;
}event_t;

/* for user generated events */
typedef struct{
	event_t super;
}protimer_user_event_t;

/* for tick event */
typedef struct{
	event_t super;
	uint8_t ss;
}protimer_tick_event_t;



/* main application object */
protimer_t protimer;

volatile uint32_t millis_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */

void protimer_init(protimer_t *mobj);
event_status_t protimer_state_machine(protimer_t *const mobj, event_t const *const e);
//prototypes of helper functions
static void display_clear(void);
static void display_message(char *string);
static void display_time(uint32_t time);

//prototypes of state handlers
static event_status_t protimer_state_handler_IDLE(protimer_t  *const mobj, event_t const *const e);
static event_status_t protimer_state_handler_TIME_SET(protimer_t  *const mobj, event_t const *const e);
static event_status_t protimer_state_handler_COUNTDOWN(protimer_t  *const mobj, event_t const *const e);
static event_status_t protimer_state_handler_PAUSE(protimer_t  *const mobj, event_t const *const e);
static event_status_t protimer_state_handler_STAT(protimer_t  *const mobj, event_t const *const e);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void protimer_init(protimer_t *mobj)
{
//	mobj->active_state = IDLE;
//	mobj->pro_time = 0;
    event_t ee;
    ee.sig = ENTRY;
    mobj->active_state = IDLE;
    mobj->pro_time = 0;
    (*mobj->active_state)(mobj, &ee);	//jump to idle handler
}

static void protimer_event_dispatcher(protimer_t *const mobj,event_t const *const e)
{

  event_status_t status;
  protimer_state_t source, target;

  source = mobj->active_state;
  status = (*mobj->active_state)(mobj,e);

  if(status == EVENT_TRANSITION){
    target = mobj->active_state;
    event_t ee;
    //1. run the exit action for the source state
    ee.sig = EXIT;
    (*source)(mobj,&ee);

    //2. run the entry action for the target state
    ee.sig = ENTRY;
    (*target)(mobj,&ee);
  }

}

static event_status_t protimer_state_handler_IDLE(protimer_t *const mobj, event_t const *const e)
{
	switch (e->sig) {
		case ENTRY:{
			mobj->curr_time = 0;
			mobj->elapsed_time = 0;
			display_time(0);
			display_message("Set Time");
			return EVENT_HANDLED;
		}

		case EXIT:{
			display_clear();
			return EVENT_HANDLED;
		}

		case INC_TIME:{
			mobj->curr_time += 60;
			mobj->active_state = TIME_SET;
			return EVENT_TRANSITION;
		}

		case START_PAUSE:{
			mobj->active_state = STAT;
			return EVENT_TRANSITION;
		}

		case TIME_TICK:{
			if(((protimer_tick_event_t *)(e))->ss == 5)
			{
//				do_beep();
				return EVENT_HANDLED;
			}
			return EVENT_IGNORED;
		}
	}
	return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_TIME_SET(protimer_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:{
            display_time(mobj->curr_time);
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case INC_TIME:{
            mobj->curr_time += 60;
            display_time(mobj->curr_time);
            return EVENT_HANDLED;
        }
        case DEC_TIME:{
            if(mobj->curr_time >=60){
                mobj->curr_time -= 60;
                display_time(mobj->curr_time);
                return EVENT_HANDLED;
            }
            return EVENT_IGNORED;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
        case START_PAUSE:{
            if(mobj->curr_time >=60){
                mobj->active_state = COUNTDOWN;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
    }

    return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_COUNTDOWN(protimer_t *const mobj, event_t const *const e){
    switch(e->sig){
        case EXIT:{
            mobj->pro_time += mobj->elapsed_time;
            mobj->elapsed_time = 0;
            return EVENT_HANDLED;
        }

        case TIME_TICK:{
            if(((protimer_tick_event_t*)(e))->ss == 10){
                --mobj->curr_time;
                ++mobj->elapsed_time;
                display_time(mobj->curr_time);
                if(!mobj->curr_time){
                    mobj->active_state = IDLE;
                    return EVENT_TRANSITION;
                }
                return EVENT_HANDLED;
            }
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            mobj->active_state = PAUSE;
            return EVENT_TRANSITION;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
    }
    return EVENT_IGNORED;
}


static event_status_t protimer_state_handler_PAUSE(protimer_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:{
            display_message("Paused");
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case INC_TIME:{
            mobj->curr_time += 60;
            mobj->active_state = TIME_SET;
            return EVENT_TRANSITION;
        }
        case DEC_TIME:{
            if(mobj->curr_time >= 60){
                mobj->curr_time -= 60;
                mobj->active_state = TIME_SET;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            mobj->active_state = COUNTDOWN;
            return EVENT_TRANSITION;
        }
        case ABRT:{
            mobj->active_state = IDLE;
            return EVENT_TRANSITION;
        }
    }
    return EVENT_IGNORED;
}

static event_status_t protimer_state_handler_STAT(protimer_t *const mobj, event_t const *const e){
    static uint8_t tick_count;

    switch(e->sig){
        case ENTRY:{
            display_time(mobj->pro_time);
            display_message("Productive time");
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;
        }
        case TIME_TICK:{
            if(++tick_count == 30){
                tick_count = 0;
                mobj->active_state = IDLE;
                return EVENT_TRANSITION;
            }
        }
    }
    return EVENT_IGNORED;
}

//////////////////////////helper functions//////////////////////////
static void display_time(uint32_t time){
  char buf[7];
//  char *time_msg;

  uint16_t m = time / 60;
  uint8_t s = time % 60;
  sprintf(buf,"%03d:%02d",m,s);

//  time_msg = (String)buf;
//  lcd_set_cursor(5,0);
//  lcd_print_string(time_msg);

  printf("%s\r\n", buf);
}

static void display_message(char *string){
	printf("%s\r\n", string);
}

static void display_clear(void){
    printf("clear\r\n");
}





uint32_t millis(void)
{
	return millis_counter;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM17)
	{
		millis_counter++;
//		printf("%ld\r\n", millis_counter);
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
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

  protimer_init(&protimer);

  HAL_TIM_Base_Start_IT(&htim17);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   	  uint8_t btn_pad_value;
	  	  protimer_user_event_t ue;
	  	  static uint32_t current_time;

	  	  current_time = millis();

	  	  static protimer_tick_event_t te;

	  	  //2. make an event
	  	  if(HAL_UART_Receive(&hlpuart1, &btn_pad_value, 1, HAL_MAX_DELAY) == HAL_OK)
	  	  {
	  		  if(btn_pad_value == '1')
	  		  {
	  			  ue.super.sig = INC_TIME;
	  		  }
	  		  else if(btn_pad_value == '2')
	  		  {
	  			  ue.super.sig = DEC_TIME;
	  		  }
	  		  else if(btn_pad_value == '3')
	  		  {
	  			  ue.super.sig = START_PAUSE;
	  		  }
	  		  else if(btn_pad_value == '4')
	  		  {
	  			  ue.super.sig = ABRT;
	  		  }



	  	     //3. send it to event dispatcher
	  	    protimer_event_dispatcher(&protimer,&ue.super);
	  	  }

	  	  //4. dispatch the time tick event for every 100ms
	  	  if(millis() - current_time  >= 100){
	  	    //100ms has passed
	  	    current_time = millis();
	  	    te.super.sig = TIME_TICK;
	  	    if(++te.ss > 10) te.ss = 1;

	  	    protimer_event_dispatcher(&protimer,&te.super);
	  	  }

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
  htim17.Init.Period = 1000-1;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

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