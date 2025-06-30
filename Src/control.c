/*
 * control.c
 *
 *  Created on: Aug 9, 2024
 *      Author: Anton
 */

#include "main.h"
#include "stdint.h"

Control_t control;

void control_Modes (void){
	  // Check and On/Off Eco mode
	  //For Eco mode off need be unchecked button and SOC > ECO_MIN_CHARGE and Battery temp < 40C
	  // Also use hysteresis for temperature and voltage
	  if (control.requestEco == false &&
			  ((bms.maximumTemp < ECO_MODE_TEMP && control.EcoState == ECO_MODE_DISABLE) ||
					  (bms.maximumTemp < ECO_MODE_TEMP - ECO_MODE_HYST && control.EcoState == ECO_MODE_ENABLE)) &&
					  ((bms.batteryCapacityPercentage > ECO_MIN_CHARGE && control.EcoState == ECO_MODE_DISABLE) ||
							  (bms.batteryCapacityPercentage > ECO_MIN_CHARGE + ECO_MODE_HYST && control.EcoState == ECO_MODE_ENABLE)))
	  {
		  HAL_GPIO_WritePin(OUT_ECO_GPIO_Port, OUT_ECO_Pin, GPIO_PIN_SET);
		  control.EcoState = ECO_MODE_DISABLE;
	  }
	  else
	  {
		  HAL_GPIO_WritePin(OUT_ECO_GPIO_Port, OUT_ECO_Pin, GPIO_PIN_RESET);
		  control.EcoState = ECO_MODE_ENABLE;
	  }

	  // Check Climate state allowed
	if (Climate.state == CLIMATE_STATE_ALLOWED) {
//		set_climate_power(Climate.regulator_position);
		HeaterCommand.pwm = 66;
		HAL_GPIO_WritePin(OUT_PUMP_GPIO_Port, OUT_PUMP_Pin, GPIO_PIN_SET);
	}
	else {
		HeaterCommand.pwm = HEATER_MIN_PWM;
		HAL_GPIO_WritePin(GPIOD, OUT_PUMP_Pin, GPIO_PIN_RESET);
//		ACCommand.state = AC_STATUS_STANDBY;
	}
	if (control.RequestAC == true) {
		ACCommand.state = AC_STATUS_START;
	} else {
		ACCommand.state = AC_STATUS_STANDBY;
	}

	set_heater_PWM(HeaterCommand.pwm);
}

uint16_t store_ADC1_data(void) {
	if (adc1_flag == true){
		adc1_flag = false;
//		uint16_t adc = adc1[3];
		// Start ADC conversion
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc1, 4);
		// PA4 used as 4 channel of ADC1
		return true;
	}
	return false;
}


// Get input_value from potentiometer of car climate control to set AC and heater power
void  set_climate_power(uint16_t input_value) {
	if (input_value >= 1216)
		ACCommand.rpm = AC_MIN_RPM;
	if (input_value < 1344)
		HeaterCommand.pwm = HEATER_MIN_PWM;

	if (input_value < 128)
		ACCommand.rpm = AC_MAX_RPM;
	else if (input_value < 256)
		ACCommand.rpm = 77;
	else if (input_value < 384)
		ACCommand.rpm = 69;
	else if (input_value < 512)
		ACCommand.rpm = 61;
	else if (input_value < 640)
		ACCommand.rpm = 53;
	else if (input_value < 768)
		ACCommand.rpm = 45;
	else if (input_value < 896)
		ACCommand.rpm = 37;
	else if (input_value < 1024)
		ACCommand.rpm = 31;
	else if (input_value < 1152)
		ACCommand.rpm = 24;
	else if (input_value < 1216)
		ACCommand.rpm = 16;
	// 1280 середина шкалы резистора. Делаем гистерезис +-64 (1216 - 1344)
	// чтобы в этом диапазоне были минимальные значения печки и компрессора
	else if (input_value < 1344)
		HeaterCommand.pwm = HEATER_MIN_PWM;
	else if (input_value < 1408)
		HeaterCommand.pwm = 9;
	else if (input_value < 1536)
		HeaterCommand.pwm = 18;
	else if (input_value < 1664)
		HeaterCommand.pwm = 27;
	else if (input_value < 1792)
		HeaterCommand.pwm = 36;
	else if (input_value < 1920)
		HeaterCommand.pwm = 45;
	else if (input_value < 2048)
		HeaterCommand.pwm = 54;
	else if (input_value < 2176)
		HeaterCommand.pwm = 63;
	else if (input_value < 2304)
		HeaterCommand.pwm = 72;
	else if (input_value < 2431)
		HeaterCommand.pwm = 81;
	else
		HeaterCommand.pwm = HEATER_MAX_PWM;
}

// Function for set charger and DC-DC state
void set_OBC_state(void) {
	// EVSE connector button pushed
//	if (Charger.proximity_pilot <= 0x0900) {
	if (Charger.proximity_pilot <= 0x0C80 && Charger.proximity_pilot > 0x0900) {
		Charger.pp_state = CHARGER_PP_BTN_PUSHED;
		Charger.current = 0;
		Charger.voltage = 0;
		Charger.state = BLOCKED;
	}
	// EVSE connector inserted
	else if (Charger.proximity_pilot <= 0x0900) {
		Charger.pp_state = CHARGER_PP_PLUGED;
		if (Charger.state == BLOCKED || Charger.state == DISCONNECTED){
			Charger.state = CONNECTED;
		}
		switch (Charger.state) {
		case CONNECTED:
			HAL_GPIO_WritePin(GPIOD, OUT_IGCT_Pin, GPIO_PIN_SET);	// Close charging relay
			Charger.voltage = CHARGER_MAX_VOLTAGE;					// Set Charging voltage
			Charger.contactor_request = CHARGER_EVSE_CONNECTED;
			Charger.state = CHARGING;
			break;
		case CHARGING:
			if (bms.maximumVoltage >= bms.chargingProtectionVoltage) {
				Charger.state = CHARGED;
				Charger.voltage = 0;
				Charger.current = 0;
			}
			else if (Charger.current < CHARGER_MAX_CURRENT)
				Charger.current ++;
			break;
		case CHARGED:
			if (bms.maximumVoltage < bms.chargingProtectionVoltage - BMS_HYSTERESIS) {
				Charger.state = CHARGING;
			}
			break;
		default:
			break;
		}
	}
	// EVSE disconnected
	else {
		HAL_GPIO_WritePin(GPIOD, OUT_IGCT_Pin, GPIO_PIN_RESET);		// Open charging relay
		Charger.pp_state = CHARGER_PP_EMPTY;
		Charger.current = 0;
		Charger.voltage = 0;
		Charger.contactor_request = CHARGER_EVSE_DISCONNECTED;
		Charger.state = DISCONNECTED;
	}


    TxHeader_1.IDE = CAN_ID_STD;
    TxHeader_1.RTR = CAN_RTR_DATA;
    TxHeader_1.DLC = 8;
	TxHeader_1.StdId = OBC_COMMAND_MSG_ID;

	TxData_1[0] = Charger.voltage >> 8;
	TxData_1[1] = Charger.voltage &= 0xFF;
	TxData_1[2] = Charger.current;
	TxData_1[3] = 55;
	TxData_1[4] = 0;
	TxData_1[5] = 0;
	TxData_1[6] = 0;
	TxData_1[7] = 0;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_1, TxData_1, &TxMailbox_1);

}

// Mitsubishi AC and Charger Heartbeat
void MMC_Heartbeat(void) {
    TxHeader_1.IDE = CAN_ID_STD;
    TxHeader_1.RTR = CAN_RTR_DATA;
    TxHeader_1.DLC = 8;
	TxHeader_1.StdId = MMC_HEARTBIT_MSG_ID;

	TxData_1[0] = 0;
	TxData_1[1] = 0;
	TxData_1[2] = Charger.contactor_request;
	TxData_1[3] = 0x39;
	TxData_1[4] = 0x91;
	TxData_1[5] = 0xFE;
	TxData_1[6] = 0x0C;
	TxData_1[7] = 0x10;
//	TxData_1[3] = 0x10;
//	TxData_1[4] = 0x78;
//	TxData_1[5] = 0x00;
//	TxData_1[6] = 0x00;
//	TxData_1[7] = 0x10;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_1, TxData_1, &TxMailbox_1);
}

void wake_Ic(void) {
    TxHeader_1.IDE = CAN_ID_STD;
    TxHeader_1.RTR = CAN_RTR_DATA;
    TxHeader_1.DLC = 8;
	TxHeader_1.StdId = 0x102;

	TxData_1[0] = 0x22;
	TxData_1[1] = 0xB3;
	TxData_1[2] = 0x88;
	TxData_1[3] = 0x04;
	TxData_1[4] = 0x92;
	TxData_1[5] = 0x30;
	TxData_1[6] = 0x11;
	TxData_1[7] = 0x0C;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_1, TxData_1, &TxMailbox_1);

}

void debug_Message(uint16_t minimumVoltage, uint16_t maximumVoltage, uint16_t regulator, uint16_t charger_pp) {
    TxHeader_1.IDE = CAN_ID_STD;
    TxHeader_1.RTR = CAN_RTR_DATA;
    TxHeader_1.DLC = 8;
	TxHeader_1.StdId = 0x777;

	TxData_1[0] = bms.minimumVoltage >> 8;
	TxData_1[1] = bms.minimumVoltage;
	TxData_1[2] = bms.maximumVoltage >> 8;
	TxData_1[3] = bms.maximumVoltage;
	TxData_1[4] = regulator >> 8;
	TxData_1[5] = regulator;
	TxData_1[6] = charger_pp >> 8;
	TxData_1[7] = charger_pp;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_1, TxData_1, &TxMailbox_1);
}
