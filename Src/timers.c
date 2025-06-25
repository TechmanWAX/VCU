/*
 * timers.c
 *
 *  Created on: Mar 30, 2024
 *      Author: Anton
 */
#include "main.h"

// Timer variables
uint32_t timer_count_CAN = 0;
uint32_t timer_count_GPIO = 0;

// Timer interrupt handler
//Timer for CAN1 Bus
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim_oc)
{
	//Timer for CAN1 bus
	//Tick every 1 mS
	if (htim_oc->Instance == TIM2) {
		timer_count_CAN++;
		// Send CAN messages every 10ms
		if (timer_count_CAN % CAN_TIM_PERIOD_10 == 0)
		{
			store_ADC1_data();
			Climate.regulator_position = adc1[3];
			Charger.proximity_pilot = adc1[2];
			MMC_Heartbeat();
		}
		// Send can messages every 100ms
		if (timer_count_CAN % CAN_TIM_PERIOD_100 == 0)
		{
			setup_ac(ACCommand.state, ACCommand.rpm);
			set_OBC_state();
		}
		// Send can messages every 1000ms
		if (timer_count_CAN % CAN_TIM_PERIOD_1000 == 0)
		{
			if (bms.connected == false){
				BMS_Connect();
			}
			while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) < 1)
				;
//			setup_CLUSTER_CAN_TEMPERATURES();																	// 0x150
			wake_Ic();
			debug_Message(bms.minimumVoltage, bms.maximumVoltage, Climate.regulator_position, Charger.proximity_pilot);
		}
	}
	//Timer for Inputs and Outputs
	//Tick every 100 mS
	if (htim_oc->Instance == TIM3) {
		if (timer_count_GPIO % GPIO_TIM_PERIOD_100 == 0)
		{
			;

		}
	}
	//Tick every 1000 mS
	if (htim_oc->Instance == TIM3) {
		if (timer_count_GPIO % GPIO_TIM_PERIOD_1000 == 0)
		{
			;
//			Climate.regulator_position = read_charger_PP();
//			Charger.proximity_pilot ++;

		}

		control.requestIgnition = HAL_GPIO_ReadPin(IN_IGNITION_GPIO_Port, IN_IGNITION_Pin);
		control.requestEco = HAL_GPIO_ReadPin(IN_BTN_ECO_GPIO_Port, IN_BTN_ECO_Pin);
		Climate.state = HAL_GPIO_ReadPin(BTN_CLIMATE_GPIO_Port, BTN_CLIMATE_Pin);
		control.RequestAC = HAL_GPIO_ReadPin(IN_BTN_AC_GPIO_Port, IN_BTN_AC_Pin);

		control_Modes();
	}

	// Timer for BMS alive
	// If in 5 seconds we do not receive CAN message from BMS
	// We will flag it
	if  (htim_oc->Instance == TIM4) {
		bms.connected = false;
	}
}
