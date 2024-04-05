/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "app.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 0
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN EV */
#endif
extern TIM_HandleTypeDef htim7;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */
	extern app_data_t app;

	CAN_HandleTypeDef *hcan1 = app.board.canbus_device.hcan;
#if 0
  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */
#endif
  HAL_CAN_IRQHandler(hcan1);
  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	extern app_data_t app;

	UART_HandleTypeDef *huart3 = app.board.cli.huart;
#if 0
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */
#endif
  HAL_UART_IRQHandler(huart3);
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
	extern app_data_t app;

	TIM_HandleTypeDef *htim5 = app.board.cool_flow.htim;
#if 0
  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */
#endif
  HAL_TIM_IRQHandler(htim5);
  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles UART7 global interrupt.
  */
void UART7_IRQHandler(void)
{
  /* USER CODE BEGIN UART7_IRQn 0 */
	extern app_data_t app;

	UART_HandleTypeDef *huart7 = &app.board.stm32f767.huart7;
#if 0
  /* USER CODE END UART7_IRQn 0 */
  HAL_UART_IRQHandler(&huart7);
  /* USER CODE BEGIN UART7_IRQn 1 */
#endif
  HAL_UART_IRQHandler(huart7);
  /* USER CODE END UART7_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	extern app_data_t app;
	cli_device_t *cli = &app.board.cli;
	char endl[] = "\r\n";
	HAL_StatusTypeDef ret = 0;

	if(cli->huart->Instance == huart->Instance)
	{
		if(cli->c == '\r')
		{
			ret = HAL_UART_Transmit_IT(cli->huart, (uint8_t *)endl, strlen(endl));
			cli->line[cli->index] = '\0';
			cli->index = 0;
			if(strlen(cli->line) > 0)
			{
				cli->msg_pending = true;
				cli->msg_count++;
			}
		}
		else if(cli->c == '\n')
		{
			// ignore \r
		}
		else if(cli->c == 127)
		{
			uint8_t del = 127;
			if(cli->index != 0)
			{
				cli->index--;
				cli->line[cli->index] = ' ';
				ret = HAL_UART_Transmit_IT(cli->huart, &cli->c, 1);
				ret = HAL_UART_Transmit_IT(cli->huart, (uint8_t *)&cli->line[cli->index], 1);
				ret = HAL_UART_Transmit_IT(cli->huart, &del, 1);
			}
		}
		else if(cli->c >= 32 && cli->c <= 126)
		{
			if(cli->index != CLI_LINE_SIZE - 1)
			{
				cli->line[cli->index++] = cli->c;
				ret = HAL_UART_Transmit_IT(cli->huart, &cli->c, 1);
			}
		}
		ret = HAL_UART_Receive_IT(cli->huart, &cli->c, 1);
		app.cli_fault = (ret != HAL_OK);
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	//BaseType_t task = 0;
	extern app_data_t app;
	canbus_device_t *canbus = &app.board.canbus_device;
	canbus_packet_t *rx_packet = &canbus->rx_packet;
    ams_t *ams = &app.board.ams;
	CAN_RxHeaderTypeDef rx_header;

    // See ECU_AMS_CANBUS_Protocol.xlsx
    ams_data_packet_t const ams_data_dest[] = {
    		{0,  &ams->state,             &ams->air_state,         &ams->current},
			{1,  &ams->imd_ok,            &ams->imd_status,        &ams->imd_duty},
			{2,  &ams->max_temp,          &ams->min_volt,          &ams->max_volt},
    		{3,  &ams->segs[0].volts[0],  &ams->segs[0].volts[1],  &ams->segs[0].volts[2]},
			{4,  &ams->segs[0].volts[3],  &ams->segs[0].volts[4],  &ams->segs[0].volts[5]},
			{5,  &ams->segs[0].volts[6],  &ams->segs[0].volts[7],  &ams->segs[0].volts[8]},
			{6,  &ams->segs[0].volts[9],  &ams->segs[0].volts[10], &ams->segs[0].volts[11]},
			{7,  &ams->segs[0].volts[12], &ams->segs[0].volts[13], NULL},
			{8,  &ams->segs[1].volts[0],  &ams->segs[1].volts[1],  &ams->segs[1].volts[2]},
			{9,  &ams->segs[1].volts[3],  &ams->segs[1].volts[4],  &ams->segs[1].volts[5]},
			{10, &ams->segs[1].volts[6],  &ams->segs[1].volts[7],  &ams->segs[1].volts[8]},
			{11, &ams->segs[1].volts[9],  &ams->segs[1].volts[10], &ams->segs[1].volts[11]},
			{12, &ams->segs[1].volts[12], &ams->segs[1].volts[13], NULL},
			{13, &ams->segs[2].volts[0],  &ams->segs[2].volts[1],  &ams->segs[2].volts[2]},
			{14, &ams->segs[2].volts[3],  &ams->segs[2].volts[4],  &ams->segs[2].volts[5]},
			{15, &ams->segs[2].volts[6],  &ams->segs[2].volts[7],  &ams->segs[2].volts[8]},
			{16, &ams->segs[2].volts[9],  &ams->segs[2].volts[10], &ams->segs[2].volts[11]},
			{17, &ams->segs[2].volts[12], &ams->segs[2].volts[13], NULL},
			{18, &ams->segs[3].volts[0],  &ams->segs[3].volts[1],  &ams->segs[3].volts[2]},
			{19, &ams->segs[3].volts[3],  &ams->segs[3].volts[4],  &ams->segs[3].volts[5]},
			{20, &ams->segs[3].volts[6],  &ams->segs[3].volts[7],  &ams->segs[3].volts[8]},
			{21, &ams->segs[3].volts[9],  &ams->segs[3].volts[10], &ams->segs[3].volts[11]},
			{22, &ams->segs[3].volts[12], &ams->segs[3].volts[13], NULL},
			{23, &ams->segs[4].volts[0],  &ams->segs[4].volts[1],  &ams->segs[4].volts[2]},
			{24, &ams->segs[4].volts[3],  &ams->segs[4].volts[4],  &ams->segs[4].volts[5]},
			{25, &ams->segs[4].volts[6],  &ams->segs[4].volts[7],  &ams->segs[4].volts[8]},
			{26, &ams->segs[4].volts[9],  &ams->segs[4].volts[10], &ams->segs[4].volts[11]},
			{27, &ams->segs[4].volts[12], &ams->segs[4].volts[13], NULL},
			{28, &ams->segs[0].temps[0],  &ams->segs[0].temps[1],  &ams->segs[0].temps[2]},
			{29, &ams->segs[0].temps[3],  &ams->segs[0].temps[4],  &ams->segs[0].temps[5]},
			{30, &ams->segs[0].temps[6],  &ams->segs[0].temps[7],  &ams->segs[0].temps[8]},
			{31, &ams->segs[0].temps[9],  &ams->segs[0].temps[10], &ams->segs[0].temps[11]},
			{32, &ams->segs[0].temps[12], &ams->segs[0].temps[13], &ams->segs[0].temps[14]},
			{33, &ams->segs[0].temps[15], &ams->segs[0].temps[16], NULL},
			{34, &ams->segs[1].temps[0],  &ams->segs[1].temps[1],  &ams->segs[1].temps[2]},
			{35, &ams->segs[1].temps[3],  &ams->segs[1].temps[4],  &ams->segs[1].temps[5]},
			{36, &ams->segs[1].temps[6],  &ams->segs[1].temps[7],  &ams->segs[1].temps[8]},
			{37, &ams->segs[1].temps[9],  &ams->segs[1].temps[10], &ams->segs[1].temps[11]},
			{38, &ams->segs[1].temps[12], &ams->segs[1].temps[13], &ams->segs[1].temps[14]},
			{39, &ams->segs[1].temps[15], &ams->segs[1].temps[16], NULL},
			{40, &ams->segs[2].temps[0],  &ams->segs[2].temps[1],  &ams->segs[2].temps[2]},
			{41, &ams->segs[2].temps[3],  &ams->segs[2].temps[4],  &ams->segs[2].temps[5]},
			{42, &ams->segs[2].temps[6],  &ams->segs[2].temps[7],  &ams->segs[2].temps[8]},
			{43, &ams->segs[2].temps[9],  &ams->segs[2].temps[10], &ams->segs[2].temps[11]},
			{44, &ams->segs[2].temps[12], &ams->segs[2].temps[13], &ams->segs[2].temps[14]},
			{45, &ams->segs[2].temps[15], &ams->segs[2].temps[16], NULL},
			{46, &ams->segs[3].temps[0],  &ams->segs[3].temps[1],  &ams->segs[3].temps[2]},
			{47, &ams->segs[3].temps[3],  &ams->segs[3].temps[4],  &ams->segs[3].temps[5]},
			{48, &ams->segs[3].temps[6],  &ams->segs[3].temps[7],  &ams->segs[3].temps[8]},
			{49, &ams->segs[3].temps[9],  &ams->segs[3].temps[10], &ams->segs[3].temps[11]},
			{50, &ams->segs[3].temps[12], &ams->segs[3].temps[13], &ams->segs[3].temps[14]},
			{51, &ams->segs[3].temps[15], &ams->segs[3].temps[16], NULL},
			{52, &ams->segs[4].temps[0],  &ams->segs[4].temps[1],  &ams->segs[4].temps[2]},
			{53, &ams->segs[4].temps[3],  &ams->segs[4].temps[4],  &ams->segs[4].temps[5]},
			{54, &ams->segs[4].temps[6],  &ams->segs[4].temps[7],  &ams->segs[4].temps[8]},
			{55, &ams->segs[4].temps[9],  &ams->segs[4].temps[10], &ams->segs[4].temps[11]},
			{56, &ams->segs[4].temps[12], &ams->segs[4].temps[13], &ams->segs[4].temps[14]},
			{57, &ams->segs[4].temps[15], &ams->segs[4].temps[16], NULL},
			{58, &ams->fans[0],           &ams->fans[1],           &ams->fans[2]},
			{59, &ams->fans[3],           &ams->fans[4],           &ams->fans[5]},
			{60, &ams->fans[6],           &ams->fans[7],           &ams->fans[8]},
			{61, &ams->fans[9],           NULL,                    NULL}
    };

	for (uint8_t i = 0; i < 8; i++) rx_packet->data[i] = 0x00;
	HAL_CAN_GetRxMessage(canbus->hcan, CAN_RX_FIFO0, &rx_header, rx_packet->data);
	rx_packet->id = rx_header.StdId;
	uint16_t header = ((uint16_t)rx_packet->data[0] << 8) | rx_packet->data[1];
	uint16_t data0  = ((uint16_t)rx_packet->data[2] << 8) | rx_packet->data[3];
	uint16_t data1  = ((uint16_t)rx_packet->data[4] << 8) | rx_packet->data[5];
	uint16_t data2  = ((uint16_t)rx_packet->data[6] << 8) | rx_packet->data[7];
	ams_data_packet_t packet_locs = ams_data_dest[header];
	uint16_t packet_header = packet_locs.header;
	if(packet_header == header)
	{
		if(packet_locs.d0) *packet_locs.d0 = data0;
		if(packet_locs.d1) *packet_locs.d1 = data1;
		if(packet_locs.d2) *packet_locs.d2 = data2;
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	extern app_data_t app;
	flow_sensor_t *cool_flow = &app.board.cool_flow;

	if(htim->Instance == cool_flow->htim->Instance && htim->Channel == cool_flow->total_channel) flow_sensor_read(cool_flow);
}
/* USER CODE END 1 */
