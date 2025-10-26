#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "as3935.h"
#include "lsm6dso.h"
#include "lis2mdl_reg.h"
#include "u_can.h"
#include "u_tx_debug.h"

static SPI_HandleTypeDef *imu_spi;
static SPI_HandleTypeDef *compass_spi;

static LSM6DSO_Object_t imu;
static as3935_t *as3935;
static stmdev_ctx_t *lis2mdl_ctx;


/** IMU STUFF */

/* Wrapper for lsm6dso SPI reading. */
static int32_t _lsm6dso_read(uint16_t device_address, uint16_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI reads, set MSB = 1 for read operation. */
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;
    
    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit(imu_spi, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Recieve the data. */
    status = HAL_SPI_Receive(imu_spi, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to read from the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    return LSM6DSO_OK;
}

static int32_t _lsm6dso_write(uint16_t device_address, uint16_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI writes, clear MSB = 0 for write operation. */
    uint8_t spi_reg = (uint8_t)(register_address & 0x7F);
    HAL_StatusTypeDef status;
    
    /* Send register address. */
    status = HAL_SPI_Transmit(imu_spi, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Send data. */
    status = HAL_SPI_Transmit(imu_spi, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to write to the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
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

/* Gets the accelerometer axes (x, y, and z). */
int imu_getAccelerometerData(LSM6DSO_Axes_t *axes) {
    int status = LSM6DSO_ACC_GetAxes(&imu, axes);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to call LSM6DSO_ACC_GetAxes() (Status: %d).", status);
        return U_ERROR;
    }
    return U_SUCCESS;
}

/* Gets the gyroscope axes (x, y, and z). */
int imu_getGyroscopeData(LSM6DSO_Axes_t *axes) {
    int status = LSM6DSO_GYRO_GetAxes(&imu, axes);
    if(status != LSM6DSO_OK) {
        DEBUG_PRINTLN("ERROR: Failed to call LSM6DSO_GYRO_GetAxes() (Status: %d).", status);
        return U_ERROR;
    }
    return U_SUCCESS;
}

/**
 * @brief initializes imu for reading
 * @return returns tx status for errors
 */
int init_imu(SPI_HandleTypeDef *given_imu_spi) {
    imu_spi = given_imu_spi;

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


/** LIGHTNING BOARD STUFF */

/**
 * @brief initializes lightning sensor struct used for reading values
 */
void init_lightning_sensor(SPI_HandleTypeDef *hspi) {
    as3935 = malloc(sizeof(as3935_t));
    as3935_init(as3935, hspi);

    // calibrate
    as3935_calibrate_RCO(as3935);

    // outdoor detection by default
    as3935_set_AFE(as3935, AS3935_AFE_OUTDOOR);
}

/** COMPASS STUFF */
static int32_t _lis2mdl_read(void *handle, uint8_t register_address, uint8_t *data, uint16_t length) {
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;
    
    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit((SPI_HandleTypeDef *) handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    /* Recieve the data. */
    status = HAL_SPI_Receive((SPI_HandleTypeDef *) handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to read from the lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return LSM6DSO_ERROR;
    }
    
    return LSM6DSO_OK;
}


static int32_t _lismdl_write(void *handle, uint8_t register_address, uint8_t *data, uint16_t length){
    HAL_StatusTypeDef status;

    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, &register_address, sizeof(register_address), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("ERROR: Failed to write to the lismdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    return 0;
}

int init_magnetometer(SPI_HandleTypeDef *given_compass_spi) {
    uint8_t status;

    lis2mdl_ctx = malloc(sizeof(stmdev_ctx_t));

    lis2mdl_ctx->handle = &given_compass_spi;
    lis2mdl_ctx->read_reg = _lis2mdl_read;
    lis2mdl_ctx->write_reg = _lismdl_write;
    
    lis2mdl_device_id_get(&lis2mdl_ctx, &status);

    if (status != LIS2MDL_ID) {
        DEBUG_PRINTLN("Device ID is not for LIS2MDL", status, hal_status_toString(status));
        return -1;
    }

    lis2mdl_reset_set(&lis2mdl_ctx, 1);
    lis2mdl_operating_mode_set(&lis2mdl_ctx, LIS2MDL_CONTINUOUS_MODE);
    lis2mdl_data_rate_set(&lis2mdl_ctx, LIS2MDL_ODR_50Hz);
    lis2mdl_offset_temp_comp_set(&lis2mdl_ctx, 1);
    lis2mdl_block_data_update_set(&lis2mdl_ctx, 1);

    return 0;
}

can_msg_t *read_imu() {
    LSM6DSO_Axes_t *accel_axes = malloc(sizeof(LSM6DSO_Axes_t));
    LSM6DSO_Axes_t *gyro_axes = malloc(sizeof(LSM6DSO_Axes_t));

    imu_getAccelerometerData(accel_axes);
    imu_getGyroscopeData(gyro_axes);
    
    can_msg_t message = { .id = 0xAA, .len = 128, .data = { 0 } };
    
    int32_t x = accel_axes->x;
    int32_t y = accel_axes->y;
    int32_t z = accel_axes->z;

    message.data[0] = accel_axes->x;

    free(accel_axes);
    free(gyro_axes);

    /** TODO: How should I send information */

    return 0;
}

can_msg_t *read_lightning_sensor() {
    uint8_t interrupt = as3935_get_interrupt(as3935);

    can_msg_t message = { .id = 0xAB, .len = 128, .data = { 0 } };

    if (interrupt == AS3935_INT_L) {
        uint8_t distance = as3935_get_distance(as3935);
        uint32_t energy = as3935_get_energy(as3935);

        message.data[1] = distance;
        message.data[2] = (energy >> 24) & 0xFF;
        message.data[3] = (energy >> 16) & 0xFF;
        message.data[4] = (energy >> 8) & 0xFF;
        message.data[5] = (energy >> 0) & 0xFF;
        message.data[6] = 0;
        message.data[7] = 0;
    }

    /** TODO: How should I send information */

    return &message;
}

can_msg_t *read_magnetometer() {
    int axes[3];
    lis2mdl_magnetic_raw_get(lis2mdl_ctx, axes);

    int temp;
    lis2mdl_temperature_raw_get(lis2mdl_ctx, &temp);

    can_msg_t message = { .id = 0xAC, .len = 8, .data = { 0 } };

    /** TODO: How should I send information */

    return 0;
}