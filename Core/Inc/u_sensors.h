#ifndef __U_SENSORS_H
#define __U_SENSORS_H

#include <stdint.h>
#include "u_can.h"
#include "lsm6dso.h"

/**
 * @brief initializes IMU for reading
 * @return returns tx status for errors
 */
uint16_t init_imu(SPI_HandleTypeDef *given_imu_spi);

/**
 * @brief reads and sends over CAN information from the IMU
 * @return returns any tx error in reading from the IMU
 */
uint16_t read_imu();

/**
 * @brief initializes lightning sensor struct used for reading values
 * @return returns tx status for errors
 */
uint16_t init_lightning_sensor(SPI_HandleTypeDef *hspi);

/**
 * @brief reads and sends over CAN information from the lightning sensor
 * @return returns any tx error in reading or sending
 */
uint16_t read_lightning_sensor();

/**
 * @brief initializes Magnetometer for reading
 * @return returns tx status for errors
 */
uint16_t init_magnetometer(SPI_HandleTypeDef *given_compass_spi);

/**
 * @brief reads and returns the information from magnetometer
 * @return returns any tx errors in reading from the magnetometer
 */
uint16_t read_magnetometer();

#endif