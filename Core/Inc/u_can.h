#ifndef __U_CAN_H
#define __U_CAN_H

#include <stdint.h>
#include "fdcan.h"

/**
 * @brief Initializes CAN2 Line
 * @return Returns a tx error code if failed
 */
uint8_t can2_init(FDCAN_HandleTypeDef *hcan);

/* List of CAN interfaces */
extern can_t can2;

/* List of CAN IDs */
#define CERBERUS_MSG  0xCA


#endif /* u_can.h */