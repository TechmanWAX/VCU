/*
 * bms.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Anton
 */

#ifndef INC_BMS_H_
#define INC_BMS_H_

#define MAX_CELL_COUNT 192
#define MAX_MODULE_COUNT 16

#define BMS_TX_MSG_ID	0x000000F4
#define BMS_RX_MSG_ID	0x000000F5

#define BMS_GROUP_ID_MIN	0x01
#define BMS_GROUP_ID_MAX	0x54

// Hysteresis for charge after charged battery was discharged
#define BMS_HYSTERESIS		100

typedef struct {
    bool hostTemperatureBelowZero;
    bool channelStatus;
    bool currentPolarity;
    bool balancing;
    bool overDischarge;
    bool overCurrent;
    bool batteryStringError;
    bool overCharging;
    bool overTemperature;
} StatusAccounting_t;

typedef struct {
    bool overcurrentProtection;
    bool overDischargeProtection;
    bool overchargeProtection;
    bool overTemperatureProtection;
    bool batteryStringErrorProtection;
    bool damagedChargingRelay;
    bool damagedDischargeRelay;
    bool lowVoltagePowerOutageProtection;
    bool voltageDifferenceProtection;
    bool lowTemperatureProtection;
} ProtectingHistoricalLogs_t;

typedef struct {
    // Group 1-2
    uint16_t dischargeProtectionVoltage;
    uint16_t protectiveCurrent;
    uint16_t batteryPackCapacity;
    uint16_t numberOfBatteryStrings;
    uint16_t chargingProtectionVoltage;
    uint16_t protectionTemperature;

    // Group 3
    uint16_t totalVoltage;
    int16_t totalCurrent;
    uint16_t totalPower;

    // Group 4
    uint16_t batteryUsageCapacity;
    uint16_t batteryCapacityPercentage;
    uint16_t chargingCapacity;

    // Group 5
    uint16_t chargingRecoveryVoltage;
    uint16_t dischargeRecoveryVoltage;
    uint16_t remainingCapacity;

    // Group 6
    int16_t hostTemperature;
    StatusAccounting_t statusAccounting;
    uint16_t balancingStartingVoltage;

    // Groups 7-70
    uint16_t cellVoltages[MAX_CELL_COUNT];

    // Groups 71-79
    // 0x08 - третий модуль отрицательная температура
    // 0x20 - второй модуль отрицательная температура
    // 0x80 - первый модуль отрицательная температура
    uint8_t moduleTemperaturePolarity[MAX_MODULE_COUNT];
    int16_t moduleTemperatures[MAX_MODULE_COUNT];

    // Group 80
    uint16_t lowVoltagePowerOutageProtection;
    uint16_t lowVoltagePowerOutageDelayed;
    uint16_t numOfTriggeringProtectionCells;

    // Group 81
    uint16_t balancedReferenceVoltage;
    uint16_t minimumVoltage;
    uint16_t maximumVoltage;
    uint8_t chargingDischaringMOSStatus;

    // Group 82
    uint32_t accumulatedTotalCapacity;
    uint16_t preChargeDelayTime;
    uint8_t lcdStatus;

    // Group 83
    uint16_t differentialPressureSettingValue;
    uint16_t useCapacityToAutomaticallyReset;
    int16_t lowTempProtectionSettingValue;
    ProtectingHistoricalLogs_t protectingHistoricalLogs;

    // Group 84
    uint16_t hallSensorType;
    uint16_t fanStartSettingValue;
    uint16_t ptcHeatingStartSettingValue;
    uint8_t defaultChannelState;

    // Connection flag
    bool connected;

    uint16_t minimumTemp;
    uint16_t maximumTemp;

} BatteryManagementSystem_t;

extern BatteryManagementSystem_t bms;

void BMS_Connect(void);
void process_BMS_Data(uint8_t groupId, uint8_t *data);
void processStatusAccounting(uint16_t statusData);
void processProtectingHistoricalLogs(uint8_t logsData);
extern void initializeBMS(BatteryManagementSystem_t *bms);

#endif /* INC_BMS_H_ */
