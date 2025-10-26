#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "as3935.h"
#include "lsm6dso.h"
#include "u_can.h"

static LSM6DSO_Object_t imu;
static as3935_t *as3935;

/* IMU SPI Chip Select Macros. */
#define _SELECT_IMU   GPIO_PIN_RESET // Setting the CS pin LOW selects the IMU for SPI.
#define _DESELECT_IMU GPIO_PIN_SET   // Setting the CS pin HIGH deselects the IMU for SPI.

void init_lightning_sensor(SPI_HandleTypeDef *hspi) {
    as3935 = malloc(sizeof(as3935_t));
    as3935_init(as3935, hspi, AS3935_NSS_GPIO_Port, AS3935_NSS_Pin);

    // calibrate
    as3935_calibrate_RCO(as3935);

    // outdoor detection by default
    as3935_set_AFE(as3935, AS3935_AFE_OUTDOOR);
}

/* Wrapper for lsm6dso SPI reading. */
static int32_t _lsm6dso_read(uint16_t device_address, uint16_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI reads, set MSB = 1 for read operation. */
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;
    
    /* Select the IMU device. */
    HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _SELECT_IMU);
    
    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit(&hspi2, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _DESELECT_IMU);
        DEBUG_PRINTLN("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Recieve the data. */
    status = HAL_SPI_Receive(&hspi2, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to read from the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Deselect the IMU device. */
    HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _DESELECT_IMU);
    
    return LSM6DSO_OK;
}

static int32_t _lsm6dso_write(uint16_t device_address, uint16_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI writes, clear MSB = 0 for write operation. */
    uint8_t spi_reg = (uint8_t)(register_address & 0x7F);
    HAL_StatusTypeDef status;
    
    /* Select the device (CS low). */
    HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _SELECT_IMU);
    
    /* Send register address. */
    status = HAL_SPI_Transmit(&hspi1, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _DESELECT_IMU);
        DEBUG_PRINTLN("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Send data. */
    status = HAL_SPI_Transmit(&hspi2, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to write to the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Deselect the device (CS high). */
    HAL_GPIO_WritePin(IMU_NSS_GPIO_Port, IMU_NSS_Pin, _DESELECT_IMU);
    
    return LSM6DSO_OK;
}

/* Wrapper for HAL_GetTick. */
int32_t _get_tick(void) {
    return (int32_t)HAL_GetTick();
}

/* Wrapper for HAL_Delay. */
void _delay(uint32_t delay) {
    return HAL_Delay(delay);
}

int init_imu() {
    LSM6DSO_IO_t io_config = {
        .BusType = LSM6DSO_SPI_4WIRES_BUS,
        .WriteReg = _lsm6dso_write,
        .ReadReg = _lsm6dso_read,
        .GetTick = _get_tick,
        .Delay = _delay
    };

    int status = LSM6DSO_RegisterBusIO(&imu, &io_config);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to call LSM6DSO_RegisterBusIO (Status: %d).", status);
    }

    /* Initialize IMU. */
    status = LSM6DSO_Init(&imu);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DS0_Init() (Status: %d).", status);
        return U_ERROR;
    }

    /* Setup IMU Accelerometer - default 104Hz */
	status = LSM6DSO_ACC_SetOutputDataRate_With_Mode(&imu, 833.0f, LSM6DSO_ACC_HIGH_PERFORMANCE_MODE);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DSO_ACC_SetOutputDataRate_With_Mode (Status: %d).", status);
        return U_ERROR;
    }

    /* Enable Accelerometer. */
    status = LSM6DSO_ACC_Enable(&imu);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DSO_ACC_Enable() (Status: %d).", status);
        return U_ERROR;
    }
	
    /* Set Accelerometer Filter Mode. */
	status = LSM6DSO_ACC_Set_Filter_Mode(&imu, 0, 3); // 3 = 'LSM6DSO_LP_ODR_DIV_45'
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DSO_ACC_Set_Filter_Mode() (Status: %d).", status);
        return U_ERROR;
    }

	/* Setup IMU Gyroscope */
	status = LSM6DSO_GYRO_SetOutputDataRate_With_Mode(&imu, 104.0f, LSM6DSO_GYRO_HIGH_PERFORMANCE_MODE);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DSO_GYRO_SetOutputDataRate_With_Mode() (Status: %d).", status);
        return U_ERROR;
    }

    /* Enable IMU Gyroscope. */
	status = LSM6DSO_GYRO_Enable(&imu);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to run LSM6DSO_GYRO_Enable() (Status: %d).", status);
        return U_ERROR;
    }

    return 0;
}

can_msg_t *read_lightning_sensor() {
    uint8_t interrupt = as3935_get_interrupt(as3935);

    can_msg_t *message = malloc(sizeof(can_msg_t));

    message->id = 0xDA;
    message->data[0] = interrupt;

    if (interrupt == AS3935_INT_L) {
        uint8_t distance = as3935_get_distance(as3935);
        uint32_t energy = as3935_get_energy(as3935);

        message->data[1] = distance;
        message->data[2] = (energy >> 24) & 0xFF;
        message->data[3] = (energy >> 16) & 0xFF;
        message->data[4] = (energy >> 8) & 0xFF;
        message->data[5] = (energy >> 0) & 0xFF;
        message->data[6] = 0;
        message->data[7] = 0;
    }
    else {
        for (int i = 1; i < 8; i++) {
            message->data[i] = 0;
        }
    }

    message->id_is_extended = false;
    message->len = 8;

    return message;
}

can_msg_t *read_imu() {
    return 0;
}

uint32_t imu_getAccelerometerData(LSM6DSO_Axes_t *axes) {
    int status = LSM6DSO_ACC_GetAxes(&imu, axes);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to call LSM6DSO_ACC_GetAxes() (Status: %d).", status);
        return U_ERROR;
    }
    return U_SUCCESS;
}

/* Gets the gyroscope axes (x, y, and z). */
uint32_t imu_getGyroscopeData(LSM6DSO_Axes_t *axes) {
    int status = LSM6DSO_GYRO_GetAxes(&imu, axes);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to call LSM6DSO_GYRO_GetAxes() (Status: %d).", status);
        return U_ERROR;
    }
    return U_SUCCESS;
}

can_msg_t *read_magnetometer() {
    // TODO: implement later
    return 0;
}