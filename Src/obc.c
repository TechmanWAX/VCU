#include "main.h"


void process_dcdc_status(uint8_t *data, DCDCStatus_t *status) {
    status->battery_voltage = (float)((data[0] << 8 | data[1]) * 0.01);
    status->supply_current = (float)((data[2] << 8 | data[3]) * 0.1);
    status->temperature1 = (int8_t)(data[4] - 40);
    status->temperature2 = (int8_t)(data[5] - 40);
    status->temperature3 = (int8_t)(data[6] - 40);
    status->status_byte = data[7];
    status->error = (status->status_byte & 0x01) != 0;
    status->in_operation = (status->status_byte & 0x02) != 0;
    status->ready = (status->status_byte & 0x20) != 0;
}

void process_charger_status1(uint8_t *data, ChargerStatus1_t *status) {
    status->hv_battery_voltage = data[0] * 2;
    status->ac_mains_voltage = data[1];
    status->dc_charge_current1 = (float)(data[2] * 0.1);
    status->temperature1 = (int8_t)(data[3] - 40);
    status->temperature2 = (int8_t)(data[4] - 40);
    status->status_byte = data[5];
    status->mains_voltage_present = (status->status_byte & 0x02) != 0;
    status->charging = (status->status_byte & 0x08) != 0;
    status->error = (status->status_byte & 0x10) != 0;
    status->dc_dc_converter_request = (status->status_byte & 0x40) != 0;
    status->pilot_present = (status->status_byte & 0x80) != 0;
    status->ac_mains_current = (float)(data[6] * 0.1);
    status->dc_charge_current2 = (float)(data[7] * 0.1);
}

void process_charger_status2(uint8_t *data, ChargerStatus2_t *status) {
    status->temperature1 = (int8_t)(data[0] - 40) + 5; // +5deg offset
    status->temperature2 = (int8_t)(data[1] - 40);
    status->dc_bus_voltage = data[2] * 2;
    status->pwm_signal = data[3];
    status->status = data[4];
    status->wait_for_mains = (status->status & 0x04) != 0;
    status->ready_for_charging = (status->status & 0x08) != 0;
}

// Show status of charging process
void charger_led(void) {
	// light - Charged
	// meander - charging
	if (Charger.state == CHARGING && Charger.voltage > 0 && Charger.current > 0) {
		HAL_GPIO_TogglePin(OUT_CP_LED_GPIO_Port, OUT_CP_LED_Pin);
	}
	else if (Charger.state == CHARGED) {
		HAL_GPIO_WritePin(OUT_CP_LED_GPIO_Port, OUT_CP_LED_Pin, GPIO_PIN_SET);		// Open charging relay
	}
	else {
		HAL_GPIO_WritePin(OUT_CP_LED_GPIO_Port, OUT_CP_LED_Pin, GPIO_PIN_RESET);		// Open charging relay
	}
}
// 0ACA - Disconnected
// 095A - 09C2 - 480 Ohm Unlock Pressed
// 06EB - 0700 - 150 Ohm Inserted




CHARGER_t Charger;
DCDCStatus_t dcdc_status;
ChargerStatus1_t charger_status1;
ChargerStatus2_t charger_status2;
