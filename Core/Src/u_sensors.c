#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "c_utils.h"
#include "as3935.h"
#include "lsm6dsv_reg.h"
#include "lis2mdl_reg.h"
#include "u_can.h"
#include "u_tx_debug.h"
#include "u_queues.h"

extern SPI_HandleTypeDef hspi1; // imu
extern SPI_HandleTypeDef hspi2; // lightning sensor
extern SPI_HandleTypeDef hspi3; // magnetometer

static stmdev_ctx_t imu;
static as3935_t *as3935 = NULL;
static stmdev_ctx_t *lis2mdl_ctx = NULL;

static int16_t _float_to_int16(float value) {
    if (value > 32767.0f) {
        return 32767;
    }

    if (value < -32768.0f) {
        return -32768;
    }

    return (int16_t) value;
}

/** 
 * IMU 
 */

 typedef struct {
    float x;
    float y;
    float z;
} LSM6DSV_Axes_t;

static int32_t _lsm6dsv_read(void *handle, uint8_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI reads, set MSB = 1 for read operation. */
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;
    SPI_HandleTypeDef *spi_handle = (SPI_HandleTypeDef *) handle;
    
    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit(spi_handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    /* Receive the data. */
    status = HAL_SPI_Receive(spi_handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to read from the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    return 0;
}

static int32_t _lsm6dsv_write(void *handle, uint8_t register_address, uint8_t *data, uint16_t length) {
    /* For SPI writes, clear MSB = 0 for write operation. */
    uint8_t spi_reg = (uint8_t)(register_address & 0x7F);
    HAL_StatusTypeDef status;
    SPI_HandleTypeDef *spi_handle = (SPI_HandleTypeDef *) handle;
    
    /* Send register address. */
    status = HAL_SPI_Transmit(spi_handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to send register address to lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    /* Send data. */
    status = HAL_SPI_Transmit(spi_handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to write to the lsm6dso over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    return 0;
}

void _delay(uint32_t delay) {
    return HAL_Delay(delay);
}

uint16_t imu_getAccelerometerData(LSM6DSV_Axes_t *axes) {
    int16_t buf[3];

    int status = lsm6dsv_acceleration_raw_get(&imu, buf);

    if(status != 0) {
        PRINTLN_INFO("ERROR: Failed to call lsm6dsv_acceleration_raw_get() (Status: %d).", status);
        return U_ERROR;
    }

    axes->x = lsm6dsv_from_fs2_to_mg(buf[0]);
    axes->y = lsm6dsv_from_fs2_to_mg(buf[1]);
    axes->z = lsm6dsv_from_fs2_to_mg(buf[2]);

    return U_SUCCESS;
}

uint16_t imu_getGyroscopeData(LSM6DSV_Axes_t *axes) {
    int16_t buf[3];

    int status = lsm6dsv_angular_rate_raw_get(&imu, buf);

    if(status != 0) {
        PRINTLN_INFO("ERROR: Failed to call lsm6dso_angular_rate_raw_get() (Status: %d).", status);
        return U_ERROR;
    }

    axes->x = lsm6dsv_from_fs250_to_mdps(buf[0]);
    axes->y = lsm6dsv_from_fs250_to_mdps(buf[1]);
    axes->z = lsm6dsv_from_fs250_to_mdps(buf[2]);

    return U_SUCCESS;
}

uint16_t init_imu() {
    uint8_t whoami;

    imu.read_reg = _lsm6dsv_read;
    imu.read_reg = _lsm6dsv_write;
    imu.mdelay = _delay;
    imu.handle = &hspi1;

    int status = lis2mdl_device_id_get(&imu, &whoami);
    if(status != 0) {
        PRINTLN_ERROR("ERROR: Failed to call lis2mdl_device_id_get (Status: %d).", status);
        return U_ERROR;
    }

    if (whoami != LIS2MDL_ID) {
        PRINTLN_ERROR("ERROR: Failed whoami (Status: %d).", status);
        return U_ERROR;
    }

    status = lsm6dsv_sw_reset(&imu);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_sw_reset (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_xl_mode_set(&imu, LSM6DSV_XL_HIGH_PERFORMANCE_MD);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_xl_mode_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_xl_data_rate_set(&imu, LSM6DSV_EIS_960Hz);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_xl_data_rate_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_xl_data_rate_set(&imu, LSM6DSV_EIS_960Hz);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_xl_data_rate_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_xl_full_scale_set(&imu, LSM6DSV_OIS_2g);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_xl_full_scale_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_filt_xl_lp2_set(&imu, 1);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_filt_xl_lp2_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_filt_xl_lp2_bandwidth_set(&imu, LSM6DSV_OIS_XL_LP_ULTRA_LIGHT);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_filt_xl_lp2_bandwidth_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_gy_mode_set(&imu, LSM6DSV_GY_HIGH_PERFORMANCE_MD);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_gy_mode_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_gy_data_rate_set(&imu, LSM6DSV_ODR_AT_120Hz);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_gy_data_rate_set (Status: %d).", status);
        return U_ERROR;
    }

    /** TODO: just picked one that sounds good. double check this */
    status = lsm6dsv_gy_full_scale_set(&imu, LSM6DSV_250dps);
    if (status != 0) {
        PRINTLN_ERROR("ERROR: failed lsm6dsv_gy_full_scale_set (Status: %d).", status);
        return U_ERROR;
    }

    return U_SUCCESS;
}

uint16_t read_imu() {
    LSM6DSV_Axes_t accel_axes;
    LSM6DSV_Axes_t gyro_axes;

    if (imu_getAccelerometerData(&accel_axes) != U_SUCCESS) {
        return U_ERROR;
    }

    if (imu_getGyroscopeData(&gyro_axes) != U_SUCCESS) {
        return U_ERROR;
    }

    struct __attribute__((__packed__)) {
		int16_t accel_x;
		int16_t accel_y;
		int16_t accel_z;
	} accel_data;

    accel_data.accel_x = _float_to_int16(accel_axes.x * 1000);
    accel_data.accel_y = _float_to_int16(accel_axes.y * 1000);
    accel_data.accel_z = _float_to_int16(accel_axes.z * 1000);

    struct __attribute__((__packed__)) {
		int16_t gyro_x;
		int16_t gyro_y;
		int16_t gyro_z;
	} gyro_data;

    gyro_data.gyro_x = _float_to_int16(gyro_axes.x * 1000);
    gyro_data.gyro_y = _float_to_int16(gyro_axes.y * 1000);
    gyro_data.gyro_z = _float_to_int16(gyro_axes.z * 1000);

    can_msg_t imu_accel_msg = { .id = 0xAA,
				    .len = 6,
				    .data = { 0 } };
    
	can_msg_t imu_gyro_msg = { .id = 0xAB,
				   .len = 6,
				   .data = { 0 } };

    memcpy(imu_accel_msg.data, &accel_data, sizeof(accel_data));
    memcpy(imu_gyro_msg.data, &gyro_data, sizeof(gyro_data));

    queue_send(&can_outgoing, &imu_accel_msg, TX_NO_WAIT);
    queue_send(&can_outgoing, &imu_gyro_msg, TX_NO_WAIT);

    return U_SUCCESS;
}



/** 
 * LIGHTNING SENSOR 
 */

uint16_t init_lightning_sensor() {
    as3935 = malloc(sizeof(as3935_t));
    if (as3935 == NULL) {
        PRINTLN_INFO("as3935 struct malloc failed.");
        return U_ERROR;
    }

    as3935_init(as3935, &hspi2);

    // calibrate
    as3935_calibrate_RCO(as3935);

    // outdoor detection by default
    as3935_set_AFE(as3935, AS3935_AFE_OUTDOOR);

    return U_SUCCESS;
}

uint16_t read_lightning_sensor() {
    if (as3935 == NULL) {
        return U_ERROR;
    }

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

    queue_send(&can_outgoing, &lightning_message, TX_NO_WAIT);

    return U_SUCCESS;
}



/** 
 * COMPASS STUFF 
 */

static int32_t _lis2mdl_read(void *handle, uint8_t register_address, uint8_t *data, uint16_t length) {
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;
    
    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit((SPI_HandleTypeDef *) handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    /* Receive the data. */
    status = HAL_SPI_Receive((SPI_HandleTypeDef *) handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to read from the lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    return 0;
}

static int32_t _lis2mdl_write(void *handle, uint8_t register_address, uint8_t *data, uint16_t length){
    HAL_StatusTypeDef status;

    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, &register_address, sizeof(register_address), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }
    
    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_INFO("ERROR: Failed to write to the lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    return 0;
}

uint16_t init_magnetometer() {
    uint8_t status;

    lis2mdl_ctx = malloc(sizeof(stmdev_ctx_t));
    if (lis2mdl_ctx == NULL) {
        PRINTLN_INFO("lis2mdl_ctx struct malloc failed.");
        return U_ERROR;
    }

    lis2mdl_ctx->handle = &hspi3;
    lis2mdl_ctx->read_reg = _lis2mdl_read;
    lis2mdl_ctx->write_reg = _lis2mdl_write;
    
    lis2mdl_device_id_get(lis2mdl_ctx, &status);

    if (status != LIS2MDL_ID) {
        PRINTLN_INFO("Device ID is not for LIS2MDL (Status %d/%s)", status, hal_status_toString(status));
        return U_ERROR;
    }

    lis2mdl_reset_set(lis2mdl_ctx, 1);
    lis2mdl_operating_mode_set(lis2mdl_ctx, LIS2MDL_CONTINUOUS_MODE);
    lis2mdl_data_rate_set(lis2mdl_ctx, LIS2MDL_ODR_50Hz);
    lis2mdl_offset_temp_comp_set(lis2mdl_ctx, 1);
    lis2mdl_block_data_update_set(lis2mdl_ctx, 1);

    return U_SUCCESS;
}

uint16_t read_magnetometer() {
    if (lis2mdl_ctx == NULL) {
        return U_ERROR;
    }

    int16_t raw_axes[3];    

    uint8_t data_ready;
    lis2mdl_mag_data_ready_get(lis2mdl_ctx, &data_ready);
    if (!data_ready) {
        return U_SUCCESS;
    }

    lis2mdl_magnetic_raw_get(lis2mdl_ctx, raw_axes);

    struct __attribute__((__packed__)) {
		int16_t axes_1;
		int16_t axes_2;
		int16_t axes_3;
	} axes_data;

    axes_data.axes_1 = _float_to_int16(lis2mdl_from_lsb_to_mgauss(raw_axes[0]) * 1000.0f);
    axes_data.axes_2 = _float_to_int16(lis2mdl_from_lsb_to_mgauss(raw_axes[1]) * 1000.0f);
    axes_data.axes_3 = _float_to_int16(lis2mdl_from_lsb_to_mgauss(raw_axes[2]) * 1000.0f);

    can_msg_t message = { .id = 0xAD, .len = 6, .data = { 0 } };

    memcpy(message.data, &axes_data, sizeof(axes_data));

    queue_send(&can_outgoing, &message, TX_NO_WAIT);

    return U_SUCCESS;
}