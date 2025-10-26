#ifndef __U_SENSORS_H
#define __U_SENSORS_H

#include <stdint.h>
#include "u_can.h"
#include "lsm6dso.h"

/**
 * @brief initializes lightning sensor struct used for reading values
 */
void init_lightning_sensor(SPI_HandleTypeDef *hspi);

/**
 * @brief reads and returns the information from the lighnting sensor
 * @return returns the information from the lighnting sensor
 */
can_msg_t *read_lightning_sensor();

/**
 * @brief reads and returns the information from imu
 * @return returns the information from imu
 */
can_msg_t *read_imu();

/**
 * @brief reads and returns the information from magnetometer
 * @return returns the information from magnetometer
 */
can_msg_t *read_magnetometer();

uint32_t imu_getAccelerometerData(LSM6DSO_Axes_t *axes); /* Gets the accelerometer axes (x, y, and z). */
uint32_t imu_getGyroscopeData(LSM6DSO_Axes_t *axes);     /* Gets the gyroscope axes (x, y, and z). */

#endif