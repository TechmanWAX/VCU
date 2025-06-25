/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdint.h"
#include "canRX.h"
#include "canTX.h"
#include "timers.h"

#include "bms.h"
#include "climate.h"
#include "control.h"
#include "obc.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern CAN_TxHeaderTypeDef TxHeader_1;
extern CAN_RxHeaderTypeDef RxHeader_1;
extern uint8_t             TxData_1[8];
extern uint8_t             RxData_1[8];
extern uint32_t            TxMailbox_1;

extern CAN_TxHeaderTypeDef TxHeader_2;
extern CAN_RxHeaderTypeDef RxHeader_2;
extern uint8_t             TxData_2[8];
extern uint8_t             RxData_2[8];
extern uint32_t            TxMailbox_2;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim12;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern volatile uint16_t adc1[4];
extern volatile bool adc1_flag;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
// Comment next line for production VCU build

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OUT_INT_LED_Pin GPIO_PIN_13
#define OUT_INT_LED_GPIO_Port GPIOC
#define BTN_CLIMATE_Pin GPIO_PIN_3
#define BTN_CLIMATE_GPIO_Port GPIOC
#define IN_TEMP_INV_Pin GPIO_PIN_1
#define IN_TEMP_INV_GPIO_Port GPIOA
#define IN_TEMP_EXT_Pin GPIO_PIN_2
#define IN_TEMP_EXT_GPIO_Port GPIOA
#define IN_CHARGER_PP_Pin GPIO_PIN_3
#define IN_CHARGER_PP_GPIO_Port GPIOA
#define IN_CLIMATE_ADC_Pin GPIO_PIN_4
#define IN_CLIMATE_ADC_GPIO_Port GPIOA
#define EXT_46_Pin GPIO_PIN_5
#define EXT_46_GPIO_Port GPIOA
#define IN_BRAKE_Pin GPIO_PIN_6
#define IN_BRAKE_GPIO_Port GPIOA
#define IN_BRAKE_FAULT_Pin GPIO_PIN_7
#define IN_BRAKE_FAULT_GPIO_Port GPIOA
#define IN_BTN_LOCKUP_Pin GPIO_PIN_4
#define IN_BTN_LOCKUP_GPIO_Port GPIOC
#define IN_LOW_BEAM_Pin GPIO_PIN_5
#define IN_LOW_BEAM_GPIO_Port GPIOC
#define IN_HIGH_BEAM_Pin GPIO_PIN_0
#define IN_HIGH_BEAM_GPIO_Port GPIOB
#define IN_TURN_RIGHT_Pin GPIO_PIN_1
#define IN_TURN_RIGHT_GPIO_Port GPIOB
#define IN_TURN_LEFT_Pin GPIO_PIN_2
#define IN_TURN_LEFT_GPIO_Port GPIOB
#define IN_POSITION_LIGHTS_Pin GPIO_PIN_7
#define IN_POSITION_LIGHTS_GPIO_Port GPIOE
#define IN_FRONT_FOG_Pin GPIO_PIN_8
#define IN_FRONT_FOG_GPIO_Port GPIOE
#define IN_REAR_FOG_Pin GPIO_PIN_9
#define IN_REAR_FOG_GPIO_Port GPIOE
#define IN_PARK_BRAKE_Pin GPIO_PIN_10
#define IN_PARK_BRAKE_GPIO_Port GPIOE
#define IN_RR_DOOR_Pin GPIO_PIN_11
#define IN_RR_DOOR_GPIO_Port GPIOE
#define IN_RL_DOOR_Pin GPIO_PIN_12
#define IN_RL_DOOR_GPIO_Port GPIOE
#define IN_FR_DOOR_Pin GPIO_PIN_13
#define IN_FR_DOOR_GPIO_Port GPIOE
#define IN_FL_DOOR_Pin GPIO_PIN_14
#define IN_FL_DOOR_GPIO_Port GPIOE
#define IN_IGNITION_Pin GPIO_PIN_15
#define IN_IGNITION_GPIO_Port GPIOE
#define IN_BTN_AC_Pin GPIO_PIN_10
#define IN_BTN_AC_GPIO_Port GPIOB
#define IN_BTN_ECO_Pin GPIO_PIN_11
#define IN_BTN_ECO_GPIO_Port GPIOB
#define OUT_PWM_HEATER_Pin GPIO_PIN_14
#define OUT_PWM_HEATER_GPIO_Port GPIOB
#define IN_FB_RESERVE3_4_Pin GPIO_PIN_15
#define IN_FB_RESERVE3_4_GPIO_Port GPIOB
#define OUT_PUMP_Pin GPIO_PIN_8
#define OUT_PUMP_GPIO_Port GPIOD
#define OUT_RESERVE2_Pin GPIO_PIN_9
#define OUT_RESERVE2_GPIO_Port GPIOD
#define IN_FB_RESERVE1_2_Pin GPIO_PIN_10
#define IN_FB_RESERVE1_2_GPIO_Port GPIOD
#define OUT_IGCT_Pin GPIO_PIN_11
#define OUT_IGCT_GPIO_Port GPIOD
#define OUT_FAN2_Pin GPIO_PIN_12
#define OUT_FAN2_GPIO_Port GPIOD
#define IN_FB_FANS_Pin GPIO_PIN_13
#define IN_FB_FANS_GPIO_Port GPIOD
#define OUT_FAN1_Pin GPIO_PIN_14
#define OUT_FAN1_GPIO_Port GPIOD
#define OUT_DIFFLOCK_Pin GPIO_PIN_15
#define OUT_DIFFLOCK_GPIO_Port GPIOD
#define IN_FB_ECO_DIFFLOCK_Pin GPIO_PIN_6
#define IN_FB_ECO_DIFFLOCK_GPIO_Port GPIOC
#define OUT_ECO_Pin GPIO_PIN_7
#define OUT_ECO_GPIO_Port GPIOC
#define OUT_PARK_Pin GPIO_PIN_8
#define OUT_PARK_GPIO_Port GPIOC
#define IN_FB_REV_PARK_Pin GPIO_PIN_9
#define IN_FB_REV_PARK_GPIO_Port GPIOC
#define OUT_REVERSE_Pin GPIO_PIN_8
#define OUT_REVERSE_GPIO_Port GPIOA
#define OUT_DRIVE_Pin GPIO_PIN_9
#define OUT_DRIVE_GPIO_Port GPIOA
#define IN_FB_DRIVE_IGN_Pin GPIO_PIN_10
#define IN_FB_DRIVE_IGN_GPIO_Port GPIOA
#define OUT_IGN_Pin GPIO_PIN_11
#define OUT_IGN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#ifdef DEBUG_PLATE
	//DEFINES FOR TEST BOARD ONLY
	// Ten Switches
	#define SWITCH_A_Pin GPIO_PIN_14			// Switch 1 (A)
	#define SWITCH_A_GPIO_Port GPIOE
	#define SWITCH_B_Pin GPIO_PIN_13			// Switch 2 (B)
	#define SWITCH_B_GPIO_Port GPIOE
	#define SWITCH_C_Pin GPIO_PIN_7				// Switch 3 (C)
	#define SWITCH_C_GPIO_Port GPIOA
	#define SWITCH_D_Pin GPIO_PIN_5				// Switch 4 (D)
	#define LOW_BEAM_GPIO_Port GPIOC
	#define SWITCH_E_Pin GPIO_PIN_10			// Switch 5 (E)
	#define SWITCH_E_GPIO_Port GPIOE
	#define SWITCH_F_Pin GPIO_PIN_8				// Switch 6 (F)
	#define SWITCH_F_GPIO_Port GPIOE
	#define SWITCH_G_Pin GPIO_PIN_2				// Switch 7 (G)
	#define SWITCH_G_GPIO_Port GPIOB
	#define SWITCH_H_Pin GPIO_PIN_10			// Switch 8 (H)
	#define SWITCH_H_GPIO_Port GPIOA
	#define SWITCH_I_Pin GPIO_PIN_12			// Switch 9 (I)
	#define SWITCH_I_GPIO_Port GPIOE
	#define SWITCH_J_Pin GPIO_PIN_6				// Switch 10 (J)
	#define SWITCH_J_GPIO_Port GPIOA
	// Three buttons
	#define BTN_L_Pin GPIO_PIN_4				// Button L
	#define BTN_L_GPIO_Port GPIOC
	#define BTN_N_Pin GPIO_PIN_11				// Button N
	#define BTN_N_GPIO_Port GPIOE
	#define BTN_P GPIO_PIN_9					// Button P
	#define BTN_P_GPIO_Port GPIOE
	// Eleven LEDs
	#define LED_A_Pin GPIO_PIN_11				// LED A
	#define LED_A_GPIO_Port GPIOA
	#define LED_B_Pin GPIO_PIN_9				// LED B
	#define LED_B_GPIO_Port GPIOA
	#define LED_C_Pin GPIO_PIN_8				// LED C
	#define LED_C_GPIO_Port GPIOA
	#define LED_D_Pin GPIO_PIN_8				// LED D
	#define LED_D_GPIO_Port GPIOC
	#define LED_F_Pin GPIO_PIN_7				// LED F
	#define LED_F_GPIO_Port GPIOC
	#define LED_G_Pin GPIO_PIN_15				// LED G
	#define LED_G_GPIO_Port GPIOD
	#define LED_H_Pin GPIO_PIN_14				// LED H
	#define LED_H_GPIO_Port GPIOD
	#define LED_I_Pin GPIO_PIN_12				// LED I
	#define LED_I_GPIO_Port GPIOD
	#define LED_K_Pin GPIO_PIN_11				// LED K
	#define LED_K_GPIO_Port GPIOD
	#define LED_L_Pin GPIO_PIN_9				// LED L
	#define LED_L_GPIO_Port GPIOD
	#define LED_M_Pin GPIO_PIN_8				// LED M
	#define LED_M_GPIO_Port GPIOD
#endif /* DEBUG_PLATE */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
