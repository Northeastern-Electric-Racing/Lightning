#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "c_utils.h"
#include "as3935.h"
#include "lsm6dso.h"
#include "lis2mdl_reg.h"
#include "u_can.h"
#include "u_tx_debug.h"
#include "u_queues.h"

static SPI_HandleTypeDef *imu_spi;

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


static int32_t _lis2mdl_write(void *handle, uint8_t register_address, uint8_t *data, uint16_t length){
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

int init_magnetometer(SPI_HandleTypeDef *given_magnetometer_spi) {
    uint8_t status;

    lis2mdl_ctx = malloc(sizeof(stmdev_ctx_t));

    lis2mdl_ctx->handle = given_magnetometer_spi;
    lis2mdl_ctx->read_reg = _lis2mdl_read;
    lis2mdl_ctx->write_reg = _lis2mdl_write;
    
    lis2mdl_device_id_get(lis2mdl_ctx, &status);

    if (status != LIS2MDL_ID) {
        DEBUG_PRINTLN("Device ID is not for LIS2MDL", status, hal_status_toString(status));
        return -1;
    }

    lis2mdl_reset_set(lis2mdl_ctx, 1);
    lis2mdl_operating_mode_set(lis2mdl_ctx, LIS2MDL_CONTINUOUS_MODE);
    lis2mdl_data_rate_set(lis2mdl_ctx, LIS2MDL_ODR_50Hz);
    lis2mdl_offset_temp_comp_set(lis2mdl_ctx, 1);
    lis2mdl_block_data_update_set(lis2mdl_ctx, 1);

    return 0;
}

void read_imu() {
    LSM6DSO_Axes_t accel_axes;
    LSM6DSO_Axes_t gyro_axes;

    imu_getAccelerometerData(&accel_axes);
    imu_getGyroscopeData(&gyro_axes);

    struct __attribute__((__packed__)) {
		int16_t accel_x;
		int16_t accel_y;
		int16_t accel_z;
	} accel_data;

    accel_data.accel_x = accel_axes.x;
    accel_data.accel_y = accel_axes.y;
    accel_data.accel_z = accel_axes.y;

    struct __attribute__((__packed__)) {
		int16_t gyro_x;
		int16_t gyro_y;
		int16_t gyro_z;
	} gyro_data;

    gyro_data.gyro_x = gyro_axes.x;
    gyro_data.gyro_y = gyro_axes.y;
    gyro_data.gyro_z = gyro_axes.y;

    can_msg_t imu_accel_msg = { .id = 0xAA,
				    .len = 6,
				    .data = { 0 } };
    
	can_msg_t imu_gyro_msg = { .id = 0xAB,
				   .len = 6,
				   .data = { 0 } };

    memcpy(imu_accel_msg.data, &accel_data, sizeof(accel_data));
    memcpy(imu_gyro_msg.data, &gyro_data, sizeof(gyro_data));

    queue_send(&can_outgoing, &imu_accel_msg);
    queue_send(&can_outgoing, &imu_gyro_msg);
}

void read_lightning_sensor() {
    uint8_t interrupt = as3935_get_interrupt(as3935);

    can_msg_t lightning_message = { .id = 0xAC, .len = 8, .data = { 0 } };

    struct __attribute__((__packed__)) {
		uint8_t interrupt;
		uint8_t distance;
		uint32_t energy;
	} lightning_data;

    lightning_data.interrupt = interrupt;
    lightning_data.distance = as3935_get_distance(as3935);
    lightning_data.energy = as3935_get_energy(as3935);

    memcpy(lightning_message.data, &lightning_data, sizeof(lightning_data));

    queue_send(&can_outgoing, &lightning_message);
}

void read_magnetometer() {
    int16_t raw_axes[3];    

    uint8_t data_ready;
    lis2mdl_mag_data_ready_get(lis2mdl_ctx, &data_ready);
    if (!data_ready) {
        return;
    }

    lis2mdl_magnetic_raw_get(lis2mdl_ctx, raw_axes);

    struct __attribute__((__packed__)) {
		int16_t axes_1;
		int16_t axes_2;
		int16_t axes_3;
	} axes_data;

    axes_data.axes_1 = lis2mdl_from_lsb_to_mgauss(raw_axes[0]) * 1000.0f;
    axes_data.axes_2 = lis2mdl_from_lsb_to_mgauss(raw_axes[1]) * 1000.0f;
    axes_data.axes_3 = lis2mdl_from_lsb_to_mgauss(raw_axes[2]) * 1000.0f;

    can_msg_t message = { .id = 0xAD, .len = 6, .data = { 0 } };

    memcpy(message.data, &axes_data, sizeof(axes_data));

    queue_send(&can_outgoing, &message);
}