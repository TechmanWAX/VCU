/*
 * CanRX.h
 *
 *  Created on: Apr 6, 2024
 *      Author: Anton
 */

#ifndef CAN_RX_H
#define CAN_RX_H

#include "stm32f4xx_hal.h"
#include <stdint.h>


extern void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif // CAN_RX_H
