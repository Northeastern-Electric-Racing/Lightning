#ifndef __U_CAN_H
#define __U_CAN_H

#include <stdint.h>
#include "fdcan.h"

uint8_t can2_init(FDCAN_HandleTypeDef *hcan);

/* List of CAN interfaces */
extern can_t can2;

/* List of CAN IDs */
#define BMS_ALIVE_CAN_ID  0x1 /* TODO: Replace with actual */
#define IMD_ALIVE_CAN_ID  0x2 /* TODO: Replace with actual */
#define VCU_FAULTS_CAN_ID 0x3 /* TODO: Replace with actual */


#endif /* u_can.h */