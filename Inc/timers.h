/*
 * timers.h
 *
 *  Created on: Mar 30, 2024
 *      Author: Anton
 */

#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

// Timer period in milliseconds
#define CAN_TIM_PERIOD_10 1		//10 mS
#define CAN_TIM_PERIOD_20 2		//20 mS
#define CAN_TIM_PERIOD_100 10	//100 mS
#define CAN_TIM_PERIOD_500 50	//500 mS
#define CAN_TIM_PERIOD_1000 100	//1000 mS
#define GPIO_TIM_PERIOD_100 1	//100 mS
#define GPIO_TIM_PERIOD_500 5	//500 mS
#define GPIO_TIM_PERIOD_1000 10	//1000 mS

// Timer variables
extern uint32_t timer_count;
extern uint32_t timer_period;
extern uint8_t hundred_ms_counter;

// Function prototypes
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2);

#endif // TIMERS_H
