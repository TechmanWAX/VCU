/*
 * CanRX.c
 *
 *  Created on: Apr 6, 2024
 *      Author: Anton
 */
#include "main.h"


//Rx for can1
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader_1, RxData_1) == HAL_OK)
    {
		switch (RxHeader_1.StdId) {
		case DCDC_STATUS_MSG_ID: {
			process_dcdc_status(RxData_1, &dcdc_status);
			break;
		}
		case OBC_STATUS1_MSG_ID: {
			process_charger_status1(RxData_1, &charger_status1);
			break;
		}
		case OBC_STATUS2_MSG_ID: {
			process_charger_status2(RxData_1, &charger_status2);
			break;
		}

    	//		case ESHIFT_PANEL_MSG_ID:
    	//			decompose_CANEShifter(RxData);
    	//			break;
    	//			decompose_AC2Message(RxData);
    				break;
    	//			decompose_AC3Message(RxData);
    		}
    }
    if(HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader_2, RxData_2) == HAL_OK)
    {
        if (RxHeader_2.ExtId == BMS_RX_MSG_ID)
        {
        	bms.connected = true;
    		__HAL_TIM_SET_COUNTER(&htim4, 0x0000);
            uint8_t groupId = RxData_2[0];
            process_BMS_Data(groupId, RxData_2);
        }
//    		get_BMS_state(RxHeader_2.StdId, RxData_2, &system_data);

    }

}

//Rx for can2
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &RxHeader_2, RxData_2);
//	get_BMS_state(RxHeader_2.StdId, RxData_2, &system_data);
}

// Error handle for can bus
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
        if(hcan->Instance == CAN1) {
//        	HAL_GPIO_TogglePin(LED_L_GPIO_Port, LED_L_Pin);
;
        }
        else if(hcan->Instance == CAN2) {
//        	HAL_GPIO_TogglePin(LED_K_GPIO_Port, LED_K_Pin);
;
        }
}
