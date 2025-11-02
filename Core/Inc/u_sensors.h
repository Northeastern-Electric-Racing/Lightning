#ifndef __U_SENSORS_H
#define __U_SENSORS_H

#include <stdint.h>
#include "u_can.h"
#include "lsm6dso.h"

/**
 * @brief initializes imu for reading
 * @return returns tx status for errors
 */
int init_imu(SPI_HandleTypeDef *given_imu_spi);

/**
 * @brief initializes lightning sensor struct used for reading values
 */
void init_lightning_sensor(SPI_HandleTypeDef *hspi);

/**
 * @brief initializes magnetometer for reading
 * @return returns tx status for errors
 */
int init_magnetometer(SPI_HandleTypeDef *given_compass_spi);

/**
 * @brief reads and returns the information from imu
 * @return returns the information from imu
 */
void read_imu();

/**
 * @brief reads and returns the information from the lighnting sensor
 */
void read_lightning_sensor();

/**
 * @brief reads and returns the information from magnetometer
 */
void read_magnetometer();

#endif