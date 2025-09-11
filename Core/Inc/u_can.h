#ifndef __U_CAN_H
#define __U_CAN_H

#include <stdint.h>
#include "fdcan.h"

uint8_t can1_init(FDCAN_HandleTypeDef *hcan);

/* List of CAN interfaces */
extern can_t can1;

/* List of CAN IDs */
uint16_t extended_ids[] = {0x00, 0x00};
uint16_t standard_ids[] = {0x00, 0x00};

#endif /* u_can.h */