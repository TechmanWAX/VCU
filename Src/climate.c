/*
 * climate.c
 *
 *  Created on: Jul 26, 2024
 *      Author: Anton
 */


#include "main.h"

AC_REPORT_t ACReport;
AC_COMMAND_t ACCommand;
HEATER_COMMAND_t HeaterCommand;
CLIMATE_t Climate;

void init_climate(void){
	HeaterCommand.pwm = HEATER_MIN_PWM;
	ACCommand.state = AC_STATUS_STANDBY;
	ACCommand.rpm = AC_MIN_RPM;
	Climate.state = CLIMATE_STATE_BLOCKED;
}

// Send every 100mS
void setup_ac(uint8_t status, uint8_t rpm) {
	if (rpm < 0x08)
		rpm = 0x08;
	else if (rpm > 0x54)
		rpm = 0x54;

    TxHeader_1.IDE = CAN_ID_STD;
    TxHeader_1.RTR = CAN_RTR_DATA;
    TxHeader_1.DLC = 8;
	TxHeader_1.StdId = AC_COMMAND_MSG_ID;

	TxData_1[0] = status;
	TxData_1[1] = 0;
	TxData_1[2] = 0x1D;
	TxData_1[3] = 0;
	TxData_1[4] = 0;
	TxData_1[5] = rpm;
	TxData_1[6] = 0;
	TxData_1[7] = 0;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader_1, TxData_1, &TxMailbox_1);
}

void set_heater_PWM(uint16_t value) {
    __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, value);// Timer 12, Channel 1, PB14 (OUT_HEATER_PWM)
//;
}

//void set_ACStatus (void) {
//	if (Ecomp_FaultCause_SC == ECOMP_FAULT_CAUSE_SC_FAULT || Ecomp_SleepCause_FS == ECOMP_SLEEP_CAUSE_FS_FAULT ||
//			Ecomp_SleepCause_CSE == ECOMP_SLEEP_CAUSE_CSE_FAULT || Ecomp_WaitCause_OC == ECOMP_WAIT_CAUSE_OC_FAULT ||
//			Ecomp_WaitCause_CAN == ECOMP_WAIT_CAUSE_CAN_FAULT || Ecomp_WaitCause_HT == ECOMP_WAIT_CAUSE_HT_FAULT ||
//			Ecomp_WaitCause_LT == ECOMP_WAIT_CAUSE_LT_FAULT || Ecomp_WaitCause_HV == ECOMP_WAIT_CAUSE_HV_FAULT ||
//			Ecomp_WaitCause_LV == ECOMP_WAIT_CAUSE_LV_FAULT)
//	{
//		AC_State = AC_State_FAULT;
//	}
//	else
//	{
//		AC_State = AC_State_NORMAL;
//	}
//}
//
//
