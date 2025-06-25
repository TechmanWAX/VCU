/*
 * obc.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Anton
 */

#ifndef INC_OBC_H_
#define INC_OBC_H_

#define DCDC_STATUS_MSG_ID		0x377
#define MMC_HEARTBIT_MSG_ID 0x285				// heartbeat for MMC HV modules
#define OBC_COMMAND_MSG_ID	0x286				// Commands for OBC
#define OBC_STATUS1_MSG_ID	0x389				// State of OBC
#define OBC_STATUS2_MSG_ID	0x38A				// State of OBC

// Charger states
#define BLOCKED			0
#define CONNECTED		1
#define CHARGING		2
#define	CHARGED			3
#define DISCONNECTED	4

#define CHARGER_PP_EMPTY		0
#define CHARGER_PP_PLUGED		1
#define CHARGER_PP_BTN_PUSHED	2
#define CHARGER_MAX_CURRENT		120
#define CHARGER_MAX_VOLTAGE		370

#define CHARGER_EVSE_CONNECTED	0xb6
#define CHARGER_EVSE_DISCONNECTED	0x14

typedef struct {
	uint16_t proximity_pilot;
	uint8_t pp_state;
	uint8_t current;
	uint16_t voltage;
	uint8_t contactor_request;
	uint8_t state;
} CHARGER_t;

// Struct to store DC-DC converter status (0x377h) 887
typedef struct {
    float battery_voltage;
    float supply_current;
    int8_t temperature1;
    int8_t temperature2;
    int8_t temperature3;
    uint8_t status_byte;
    bool error;
    bool in_operation;
    bool ready;
} DCDCStatus_t;

// Struct to store Charger status (0x389h) 905
typedef struct {
    uint16_t hv_battery_voltage;
    uint8_t ac_mains_voltage;
    float dc_charge_current1;
    int8_t temperature1;
    int8_t temperature2;
    uint8_t status_byte;
    bool mains_voltage_present;
    bool charging;
    bool error;
    bool dc_dc_converter_request;
    bool pilot_present;
    float ac_mains_current;
    float dc_charge_current2;
} ChargerStatus1_t;

// Struct to store additional Charger status (0x38Ah) 906
typedef struct {
    int8_t temperature1;
    int8_t temperature2;
    uint16_t dc_bus_voltage;
    uint8_t pwm_signal;
    uint8_t status;
    bool wait_for_mains;
    bool ready_for_charging;
} ChargerStatus2_t;


extern DCDCStatus_t dcdc_status;
extern ChargerStatus1_t charger_status1;
extern ChargerStatus2_t charger_status2;
extern CHARGER_t Charger;

// Function prototypes
void process_dcdc_status(uint8_t *data, DCDCStatus_t *status);
void process_charger_status1(uint8_t *data, ChargerStatus1_t *status);
void process_charger_status2(uint8_t *data, ChargerStatus2_t *status);


#endif /* INC_OBC_H_ */
