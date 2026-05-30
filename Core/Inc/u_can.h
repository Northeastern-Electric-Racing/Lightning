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
#define CERBERUS_MSG_ID           0x0CA
#define IMU_ACCEL_MSG_ID          0xAAA
#define IMU_GYRO_MSG_ID           0xAAB
#define LIGHTNING_SENSOR_MSG_ID   0xAAC
#define MAGNOMETER_MSG_ID         0xAAD
#define IMD_GENERAL_MSG_ID        0x37
#define BMS_LIGHTNING_OKAY_MSG_ID 0x01E
#define RESET_LATCHING_MSG_ID     0x510

#endif /* u_can.h */