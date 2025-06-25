/*
 * canTX.c
 *
 *  Created on: Mar 30, 2024
 *      Author: Anton
 */
#include "main.h"

//BMS variables


//Calculate checksum of packet
uint8_t calculateChecksum(uint8_t *message, uint8_t messageLength) {
    uint8_t checksum = 0;

    // Ensure that the message length is at least 7 bytes
    if (messageLength < 7) {
        return 0; // Return 0 if the message is too short
    }

    // XOR the first 7 bytes of the message
    for (uint8_t i = 0; i < 7; i++) {
        checksum ^= message[i];
    }

    return checksum;
}
