/*
 * outputs.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Anton
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

// Defines for Eco mode
#define ECO_MODE_ENABLE							1
#define ECO_MODE_DISABLE						0
#define ECO_MODE_HYST							5		// Hysteresis for Eco mode
#define ECO_MIN_CHARGE							20		// Minimum charge for automatic Eco mode on
#define ECO_MODE_TEMP							40		// Maximum temp of battery for power mode

#define HEATER_INTERPOLATE						0
#define AC_INTERPOLATE							1

typedef struct {
	bool requestEco;
	bool RequestAC;
	bool requestIgnition;
	bool EcoState;
	bool ACState;
	bool IgnitionState;
} Control_t;

extern Control_t control;

extern void control_Modes(void);
extern uint16_t store_ADC1_data(void);
extern void set_climate_power(uint16_t input_value);
extern void MMC_Heartbeat(void);
extern void wake_Ic(void);
extern void set_OBC_state(void);

extern void debug_Message(uint16_t minimumVoltage, uint16_t maximumVoltage, uint16_t regulator, uint16_t charger_pp);

#endif /* INC_CONTROL_H_ */
