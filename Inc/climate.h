/*
 * climate.h
 *
 *  Created on: Jul 26, 2024
 *      Author: Anton
 */

#ifndef INC_CLIMATE_H_
#define INC_CLIMATE_H_

// Message IDs
#define AC_COMMAND_MSG_ID 0x185					// Commands for Heater

#define AC_STATUS_START	0x0B					// Command to start compressor
#define AC_STATUS_STANDBY	0x08				// Command to standby compressor
#define AC_MIN_RPM			0x08				// Value to set minimum AC RPM
#define AC_MAX_RPM			0x54				// Value to set maximum AC RPM

#define HEATER_MIN_PWM		0
#define HEATER_MAX_PWM		95

#define CLIMATE_STATE_ALLOWED true				// Climate allowed to run
#define CLIMATE_STATE_BLOCKED false				// Climate blocked to run

//typedef struct {
//	uint8_t Allow_Operation_Flag;				// Heater operating permit
//	uint8_t Current_report;						// Current reported from Heater [Current = dec value/10]
//	uint8_t Power;								// Heater Power
//	uint8_t Inlet_Temp;							// Heater Inlet Temperature
//	uint8_t Outlet_Temp;						// Heater Outlet Temperature
//	uint8_t Low_Voltage;						// Heater Low voltage limit
//} HEATER_REPORT_t;

typedef struct {
	uint8_t pwm;
} HEATER_COMMAND_t;

typedef struct {
	uint8_t testvalue;
} AC_REPORT_t;

typedef struct {
	uint8_t state;
	uint8_t rpm;
} AC_COMMAND_t;

typedef struct {
	uint16_t regulator_position;
	bool state;
} CLIMATE_t;

//extern HEATER_REPORT_t HeaterReport;
extern HEATER_COMMAND_t HeaterCommand;
extern AC_REPORT_t ACReport;
extern AC_COMMAND_t ACCommand;
extern CLIMATE_t Climate;

extern void init_climate(void);
extern void setup_ac(uint8_t status, uint8_t rpm);
extern void set_heater_PWM(uint16_t value);

#endif /* INC_CLIMATE_H_ */
