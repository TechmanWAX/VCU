/*
 * bms.c
 *
 *  Created on) Aug 27, 2024
 *      Author) Anton
 */

#include "main.h"

BatteryManagementSystem_t bms;

// Connect to BMS and request for data. I have to send a request every time I want to get new data from the BMS.
void BMS_Connect(void) {
    TxHeader_2.IDE = CAN_ID_EXT;
    TxHeader_2.RTR = CAN_RTR_DATA;
    TxHeader_2.DLC = 3;
	TxHeader_2.ExtId = BMS_TX_MSG_ID;

	TxData_2[0] = 0x10;
	TxData_2[1] = 0x0;
	TxData_2[2] = 0x02;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan2, &TxHeader_2, TxData_2, &TxMailbox_2);

	TxData_2[0] = 0x1C;
	TxData_2[1] = 0x0;
	TxData_2[2] = 0x02;
	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) < 1)
		for (uint8_t j = 0; j < 255; j++)
			;
	HAL_CAN_AddTxMessage(&hcan2, &TxHeader_2, TxData_2, &TxMailbox_2);

}

// Get and store BMS main data
void process_BMS_Data(uint8_t groupId, uint8_t *data) {
	if (groupId >= BMS_GROUP_ID_MIN && groupId <= BMS_GROUP_ID_MAX) {
		if(groupId == 0x01) {

			// TODO Correct dataIds. Decrease it by one
			bms.dischargeProtectionVoltage = (data[2] << 8) | data[3];
			bms.protectiveCurrent = (data[4] << 8) | data[5];
			bms.batteryPackCapacity = (data[6] << 8) | data[7];
		}
		if (groupId == 0x02) {
			bms.numberOfBatteryStrings = (data[2] << 8) | data[3];
			bms.chargingProtectionVoltage = (data[4] << 8) | data[5];
			bms.protectionTemperature = (data[6] << 8) | data[7];
		}
		if (groupId == 0x03) {
		 	bms.totalVoltage = (data[2] << 8) | data[3];
			bms.totalCurrent = (int16_t)((data[4] << 8) | data[5]);
			bms.totalPower = (data[6] << 8) | data[7];
			}
		if (groupId == 0x04) {
			bms.batteryUsageCapacity = (data[2] << 8) | data[3];
			bms.batteryCapacityPercentage = (data[4] << 8) | data[5];
			bms.chargingCapacity = (data[6] << 8) | data[7];
			}
		if (groupId == 0x05) {
			bms.chargingRecoveryVoltage = (data[2] << 8) | data[3];
			bms.dischargeRecoveryVoltage = (data[4] << 8) | data[5];
			bms.remainingCapacity = (data[6] << 8) | data[7];
			}
		if (groupId == 0x06) {
            bms.hostTemperature = (int16_t)((data[2] << 8) | data[3]);
            processStatusAccounting((data[4] << 8) | data[5]);
            bms.balancingStartingVoltage = (data[6] << 8) | data[7];
			}
		if (groupId >= 0x07 && groupId <= 0x46) {
			int baseIndex = (groupId - 0x07) * 3;
			for (int i = 0; i < 3; i++) {
				if (baseIndex + i < MAX_CELL_COUNT) {
					bms.cellVoltages[baseIndex + i] = (data[1 + i*2] << 8) | data[2 + i*2];
				}
			}
		}
		// ... (cases 0x07 to 0x46 for cell voltages)
		if (groupId == 0x47) {
			bms.moduleTemperaturePolarity[0] = (data[3] & 0x01) != 0;
			bms.moduleTemperatures[0] = (int16_t)((data[4] << 8) | data[5]);
			}
		// ... (more cases for module temperatures)
		if (groupId == 0x50) {
			bms.lowVoltagePowerOutageProtection = (data[2] << 8) | data[3];
			bms.lowVoltagePowerOutageDelayed = (data[4] << 8) | data[5];
			bms.numOfTriggeringProtectionCells = (data[6] << 8) | data[7];
			}
		if (groupId == 0x51) {
			bms.balancedReferenceVoltage = (data[2] << 8) | data[3];
			bms.minimumVoltage = (data[4] << 8) | data[5];
			bms.maximumVoltage = (data[6] << 8) | data[7];
			bms.chargingDischaringMOSStatus = data[8];
			}
		if (groupId == 0x52) {
			bms.accumulatedTotalCapacity = ((uint32_t)data[2] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5];
			bms.preChargeDelayTime = (data[6] << 8) | data[7];
			bms.lcdStatus = data[8];
			}
		if (groupId == 0x53) {
			bms.differentialPressureSettingValue = (data[2] << 8) | data[3];
			bms.useCapacityToAutomaticallyReset = (data[4] << 8) | data[5];
			bms.lowTempProtectionSettingValue = (int16_t)((data[6] << 8) | data[7]);
            processProtectingHistoricalLogs(data[8]);			}
		if (groupId == 0x54) {
			bms.hallSensorType = (data[2] << 8) | data[3];
			bms.fanStartSettingValue = (data[4] << 8) | data[5];
			bms.ptcHeatingStartSettingValue = (data[6] << 8) | data[7];
			bms.defaultChannelState = data[8];
		}
	}
}

// Get and store BMS actual alarms and some statuses
void processStatusAccounting(uint16_t statusData)
{
    bms.statusAccounting.hostTemperatureBelowZero = (statusData & 0x0100) != 0;	// Host temperature below zero
    bms.statusAccounting.channelStatus = (statusData & 0x0080) != 0;			// channel status
    bms.statusAccounting.currentPolarity = (statusData & 0x0040) != 0;			// Current polarity
    bms.statusAccounting.balancing = (statusData & 0x0020) != 0;				// Equilibrium (balancing)
    bms.statusAccounting.overDischarge = (statusData & 0x0010) != 0;			// Overdischarge
    bms.statusAccounting.overCurrent = (statusData & 0x0008) != 0;				// Overcurrent
    bms.statusAccounting.batteryStringError = (statusData & 0x0004) != 0;		// Battery string error
    bms.statusAccounting.overCharging = (statusData & 0x0002) != 0;				// Overcharging
    bms.statusAccounting.overTemperature = (statusData & 0x0001) != 0;			// Overtemperature
}

// get and store BMS historical logs
void processProtectingHistoricalLogs(uint8_t logsData)
{
    bms.protectingHistoricalLogs.overcurrentProtection = (logsData == 0x01);
    bms.protectingHistoricalLogs.overDischargeProtection = (logsData == 0x02);
    bms.protectingHistoricalLogs.overchargeProtection = (logsData == 0x03);
    bms.protectingHistoricalLogs.overTemperatureProtection = (logsData == 0x04);
    bms.protectingHistoricalLogs.batteryStringErrorProtection = (logsData == 0x05);
    bms.protectingHistoricalLogs.damagedChargingRelay = (logsData == 0x06);
    bms.protectingHistoricalLogs.damagedDischargeRelay = (logsData == 0x07);
    bms.protectingHistoricalLogs.lowVoltagePowerOutageProtection = (logsData == 0x08);
    bms.protectingHistoricalLogs.voltageDifferenceProtection = (logsData == 0x09);
    bms.protectingHistoricalLogs.lowTemperatureProtection = (logsData == 0x0A);
}

// It's just a dummy function, there is no use until there is no programming of the controller from the outside. All BMS settings are made via Bluetooth
void initializeBMS(BatteryManagementSystem_t *bms)
{
    // Initialize simple fields to 0
    bms->dischargeProtectionVoltage = 0;
    bms->protectiveCurrent = 0;
    bms->batteryPackCapacity = 0;
    bms->numberOfBatteryStrings = 0;
    bms->chargingProtectionVoltage = 0;
    bms->protectionTemperature = 0;
    bms->totalVoltage = 0;
    bms->totalCurrent = 0;
    bms->totalPower = 0;
    bms->batteryUsageCapacity = 0;
    bms->batteryCapacityPercentage = 0;
    bms->chargingCapacity = 0;
    bms->chargingRecoveryVoltage = 0;
    bms->dischargeRecoveryVoltage = 0;
    bms->remainingCapacity = 0;
    bms->hostTemperature = 0;
    bms->balancingStartingVoltage = 0;

    // Initialize StatusAccounting structure
    bms->statusAccounting = (StatusAccounting_t){
        .hostTemperatureBelowZero = false,
        .channelStatus = false,
        .currentPolarity = false,
        .balancing = false,
        .overDischarge = false,
        .overCurrent = false,
        .batteryStringError = false,
        .overCharging = false,
        .overTemperature = false
    };

    // Initialize cell voltages
    for (int i = 0; i < MAX_CELL_COUNT; i++) {
        bms->cellVoltages[i] = 0;
    }

    // Initialize module temperatures
    for (int i = 0; i < MAX_MODULE_COUNT; i++) {
        bms->moduleTemperaturePolarity[i] = false;
        bms->moduleTemperatures[i] = 0;
    }

    bms->lowVoltagePowerOutageProtection = 0;
    bms->lowVoltagePowerOutageDelayed = 0;
    bms->numOfTriggeringProtectionCells = 0;
    bms->balancedReferenceVoltage = 0;
    bms->minimumVoltage = 0;
    bms->maximumVoltage = 0;
    bms->chargingDischaringMOSStatus = 0;
    bms->accumulatedTotalCapacity = 0;
    bms->preChargeDelayTime = 0;
    bms->lcdStatus = 0;
    bms->differentialPressureSettingValue = 0;
    bms->useCapacityToAutomaticallyReset = 0;
    bms->lowTempProtectionSettingValue = 0;

    // Initialize ProtectingHistoricalLogs structure
    bms->protectingHistoricalLogs = (ProtectingHistoricalLogs_t){
        .overcurrentProtection = false,
        .overDischargeProtection = false,
        .overchargeProtection = false,
        .overTemperatureProtection = false,
        .batteryStringErrorProtection = false,
        .damagedChargingRelay = false,
        .damagedDischargeRelay = false,
        .lowVoltagePowerOutageProtection = false,
        .voltageDifferenceProtection = false,
        .lowTemperatureProtection = false
    };

    bms->hallSensorType = 0;
    bms->fanStartSettingValue = 0;
    bms->ptcHeatingStartSettingValue = 0;
    bms->defaultChannelState = 0;

    bms->connected = false;
}

void min_max_temp(uint16_t  *struc) {
	;
}
