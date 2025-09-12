#ifndef __U_SENSORS_H
#define __U_SENSORS_H

#include <stdint.h>

/**
 * @brief reads and returns the information from the lighnting sensor
 * @return returns the information from the lighnting sensor
 */
uint16_t read_lightning_sensor();

/**
 * @brief reads and returns the information from imu
 * @return returns the information from imu
 */
uint16_t read_imu();

/**
 * @brief reads and returns the information from magnetometer
 * @return returns the information from magnetometer
 */
uint16_t read_magnetometer();

#endif