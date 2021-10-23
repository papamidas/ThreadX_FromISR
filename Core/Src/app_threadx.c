/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THREAD_STACK_SIZE 1024
#define TRACEX_BUFFER_SIZE 64000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t thread_stack[THREAD_STACK_SIZE];
TX_THREAD thread_ptr;
uint8_t tracex_buffer[TRACEX_BUFFER_SIZE] __attribute__ ((section (".trace")));
extern TIM_HandleTypeDef htim6;
HAL_StatusTypeDef  halstatus;

#ifdef USE_SEMAPHORE_FROM_ISR
  TX_SEMAPHORE my_semaphore_ptr;
#endif
#ifdef USE_EVENTFLAGS_FROM_ISR
  TX_EVENT_FLAGS_GROUP event_ptr;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
VOID my_thread_entry(ULONG initial_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  halstatus = HAL_TIM_Base_Start_IT(&htim6);
  if(halstatus != HAL_OK)
  {
	  while(1);
  }
  tx_thread_create( &thread_ptr, "my_thread", my_thread_entry,
		  0x1234, thread_stack, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START);
#ifdef USE_SEMAPHORE_FROM_ISR
  tx_semaphore_create(&my_semaphore_ptr, "my_semaphore", 1);
#endif
#ifdef USE_EVENTFLAGS_FROM_ISR
  tx_event_flags_create(&event_ptr, "my_event");
#endif
  tx_trace_enable(&tracex_buffer,TRACEX_BUFFER_SIZE,30);
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
VOID my_thread_entry (ULONG initial_input)
{
    while(1){
#ifdef USE_SEMAPHORE_FROM_ISR
    	tx_semaphore_get(&my_semaphore_ptr,TX_WAIT_FOREVER);
#endif
#ifdef USE_EVENTFLAGS_FROM_ISR
    	uint32_t events;
    	tx_event_flags_get(&event_ptr,0x1,TX_AND_CLEAR ,&events,TX_WAIT_FOREVER );
#endif
    	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        //tx_thread_sleep(20);
    }
}
/* USER CODE END 1 */
