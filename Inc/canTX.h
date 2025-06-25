/*
 * canTX.h
 *
 *  Created on: Mar 30, 2024
 *      Author: Anton
 */

#ifndef CAN_TX_H
#define CAN_TX_H

#include "stm32f4xx_hal.h"
#include <stdint.h>



// Can frames functions
extern uint8_t calculateChecksum(uint8_t *message, uint8_t messageLength);

#endif // CAN_TX_H
