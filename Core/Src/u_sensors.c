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
#include "can_messages_tx.h"
#include "main.h"

#define IMU_NSS_PORT SPI1_NSS_GPIO_Port
#define IMU_NSS_PIN  SPI1_NSS_Pin

#define LIG_NSS_PORT SPI2_NSS_GPIO_Port
#define LIG_NSS_PIN  SPI2_NSS_Pin

#define MAG_NSS_PORT SPI3_NSS_GPIO_Port
#define MAG_NSS_PIN  SPI3_NSS_Pin

extern SPI_HandleTypeDef hspi1; // imu
extern SPI_HandleTypeDef hspi2; // lightning sensor
extern SPI_HandleTypeDef hspi3; // magnetometer

static stmdev_ctx_t imu;
static as3935_t *as3935 = NULL;
static stmdev_ctx_t *lis2mdl_ctx = NULL;

/**
 * IMU
 */

typedef struct {
    float x;
    float y;
    float z;
} LSM6DSV_Axes_t;

/* Wrapper for lsm6dsv SPI reading. */
static int32_t _lsm6dsv_read(void* spi_handle, uint8_t reg, uint8_t* buffer, uint16_t length) {
    
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)spi_handle;
    HAL_StatusTypeDef status;

    /* Select the IMU by setting its CS pin LOW. */
    HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_RESET);
    
    /* Tell the IMU you want to read from 'reg'. */
    uint8_t spi_reg = (uint8_t)(reg | 0b10000000); // Bits 0 through 6 store 'reg' (the register address), while Bit 7 lets you chose if it's a read or write operation (1=read, 0=write).
    status = HAL_SPI_Transmit(handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to call HAL_SPI_Transmit() to write the first SPI command (Status: %d/%s).", status, hal_status_toString(status));
        HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);
        return -1;
    }

    /* Read from 'reg'. */
    status = HAL_SPI_Receive(handle, buffer, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to call HAL_SPI_Receive() to read from 'reg' (Status: %d/%s).", status, hal_status_toString(status));
        HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);
        return -1;
    }

    /* Deselect the IMU by setting its CS pin HIGH. */
    HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);
    
    return 0;
}

/* Wrapper for lsm6dsv SPI writing. */
static int32_t _lsm6dsv_write(void* spi_handle, uint8_t reg, const uint8_t* data, uint16_t length) {
    SPI_HandleTypeDef *handle = (SPI_HandleTypeDef *)spi_handle;
    HAL_StatusTypeDef status;

    /* Select the IMU by setting its CS pin LOW. */
    HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_RESET);
    
    /* Tell the IMU you want to write to 'reg'. */
    uint8_t spi_reg = (uint8_t)(reg & 0b01111111); // Bits 0 through 6 store 'reg' (the register address), while Bit 7 lets you chose if it's a read or write operation (1=read, 0=write).
    status = HAL_SPI_Transmit(handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to call HAL_SPI_Transmit() to write the first SPI command (Status: %d/%s).", status, hal_status_toString(status));
        HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);
        return -1;
    }

    /* Write to 'reg'. */
    status = HAL_SPI_Transmit(handle, data, length, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to call HAL_SPI_Transmit() to write to 'reg' (Status: %d/%s).", status, hal_status_toString(status));
        HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);
        return -1;
    }

    HAL_GPIO_WritePin(IMU_NSS_PORT, IMU_NSS_PIN, GPIO_PIN_SET);

    return 0;
}

void _delay(uint32_t delay) {
    HAL_Delay(delay);
}

uint16_t imu_getAccelerometerData(LSM6DSV_Axes_t *axes) {
    int16_t buf[3];

    uint8_t status = lsm6dsv_acceleration_raw_get(&imu, buf);

    if (status != 0) {
        PRINTLN_ERROR("ERROR: Failed to call lsm6dsv_acceleration_raw_get() (Status: %d).", status);
        return U_ERROR;
    }

    axes->x = lsm6dsv_from_fs2_to_mg(buf[0]);
    axes->y = lsm6dsv_from_fs2_to_mg(buf[1]);
    axes->z = lsm6dsv_from_fs2_to_mg(buf[2]);

    return U_SUCCESS;
}

uint16_t imu_getGyroscopeData(LSM6DSV_Axes_t *axes) {
    int16_t buf[3];

    uint8_t status = lsm6dsv_angular_rate_raw_get(&imu, buf);

    if (status != 0) {
        PRINTLN_ERROR("ERROR: Failed to call lsm6dso_angular_rate_raw_get() (Status: %d).", status);
        return U_ERROR;
    }

    axes->x = lsm6dsv_from_fs250_to_mdps(buf[0]);
    axes->y = lsm6dsv_from_fs250_to_mdps(buf[1]);
    axes->z = lsm6dsv_from_fs250_to_mdps(buf[2]);

    return U_SUCCESS;
}

uint16_t init_imu() {
    imu.read_reg = _lsm6dsv_read;
    imu.write_reg = _lsm6dsv_write;
    imu.mdelay = _delay;
    imu.handle = &hspi1;

    uint8_t id;
    uint8_t status = lsm6dsv_device_id_get(&imu, &id);

    if (status != 0) {
        PRINTLN_ERROR("Failed to call lsm6dsv_device_id_get() (Status: %d).", status);
        return U_ERROR;
    }

    if (id != LSM6DSV_ID) {
        PRINTLN_ERROR("lsm6dsv_device_id_get() returned an unexpected ID (id=%d, expected=%d). This means that the IMU is not configured correctly.", id, LSM6DSV_ID);
        return U_ERROR;
    }

    /* Reset IMU. */
    status = lsm6dsv_reset_set(&imu, LSM6DSV_GLOBAL_RST);
    if (status != 0) {
        PRINTLN_ERROR("Failed to reset the IMU via lsm6dsv_reset_set() (Status: %d).", status);
        return U_ERROR;
    }
    HAL_Delay(30); // This is probably overkill, but the datasheet lists the gyroscope's "Turn-on time" as 30ms, and I can't find anything else that specifies how long resets take.

    /* Enable Block Data Update. */
    status = lsm6dsv_block_data_update_set(&imu, PROPERTY_ENABLE); // Makes it so "output registers are not updated until LSB and MSB have been read". Datasheet says this is enabled by default but figured it was better to be explicit.
    if (status != 0) {
        PRINTLN_ERROR("Failed to enable Block Data Update via lsm6dsv_block_data_update_set() (Status: %d).", status);
        return U_ERROR;
    }

    /* Set Accelerometer Full Scale. */
    status = lsm6dsv_xl_full_scale_set(&imu, LSM6DSV_2g);
    if (status != 0) {
        PRINTLN_ERROR("Failed to set IMU Accelerometer Full Scale via lsm6dsv_xl_full_scale_set() (Status: %d).", status);
        return U_ERROR;
    }

    /* Set gyroscope full scale. */
    status = lsm6dsv_gy_full_scale_set(&imu, LSM6DSV_2000dps);
    if (status != 0) {
        PRINTLN_ERROR("Failed to set IMU Gyroscope Full Scale via lsm6dsv_gy_full_scale_set() (Status: %d).", status);
        return U_ERROR;
    }

    /* Set accelerometer output data rate. */
    status = lsm6dsv_xl_data_rate_set(&imu, LSM6DSV_ODR_AT_120Hz);
    if (status != 0) {
        PRINTLN_ERROR("Failed to set IMU Accelerometer Datarate via lsm6dsv_xl_data_rate_set() (Status: %d).", status);
        return U_ERROR;
    }

    /* Set gyroscope output data rate. */
    status = lsm6dsv_gy_data_rate_set(&imu, LSM6DSV_ODR_AT_120Hz);
    if (status != 0) {
        PRINTLN_ERROR("Failed to set IMU Gyroscope Datarate via lsm6dsv_gy_data_rate_set() (Status: %d).", status);
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

    send_lightning_board_imu_acceleration_data(accel_axes.x, accel_axes.y, accel_axes.z);
    send_lightning_board_imu_gyro_data(gyro_axes.x, gyro_axes.y, gyro_axes.z);

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
    uint8_t distance = as3935_get_distance(as3935);
    uint32_t energy = as3935_get_energy(as3935);

    printf("Lightning: %d, %d, %ld\n", interrupt, distance, energy);

    send_lightning_board_lightning_sensor_information(interrupt,distance, energy);

    return U_SUCCESS;
}



/**
 * COMPASS STUFF
 */

static int32_t _lis2mdl_read(void *handle, uint8_t register_address, uint8_t *data, uint16_t length) {
    uint8_t spi_reg = (uint8_t)(register_address | 0x80);
    HAL_StatusTypeDef status;

    printf("Before data: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", data[i]);
    }
    printf("\n\n");
    
    HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_RESET);

    /* Send the register address we're trying to read from. */
    status = HAL_SPI_Transmit((SPI_HandleTypeDef *) handle, &spi_reg, sizeof(spi_reg), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);
        PRINTLN_ERROR("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    /* Receive the data. */
    status = HAL_SPI_Receive((SPI_HandleTypeDef *) handle, data, length, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);
        PRINTLN_ERROR("ERROR: Failed to read from the lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);

    printf("After data: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", data[i]);
    }
    printf("\n\n");

    return 0;
}

static int32_t _lis2mdl_write(void *handle, uint8_t register_address, const uint8_t *data, uint16_t length){
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_RESET);

    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, &register_address, sizeof(register_address), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);
        PRINTLN_ERROR("ERROR: Failed to send register address to lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    status = HAL_SPI_Transmit((SPI_HandleTypeDef *)handle, data, length, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);
        PRINTLN_ERROR("ERROR: Failed to write to the lis2mdl over SPI (Status: %d/%s).", status, hal_status_toString(status));
        return -1;
    }

    HAL_GPIO_WritePin(MAG_NSS_PORT, MAG_NSS_PIN, GPIO_PIN_SET);

    return 0;
}

uint16_t init_magnetometer() {
    uint8_t status;
    uint8_t whoami;

    lis2mdl_ctx = malloc(sizeof(stmdev_ctx_t));
    if (lis2mdl_ctx == NULL) {
        PRINTLN_ERROR("lis2mdl_ctx struct malloc failed.");
        return U_ERROR;
    }

    lis2mdl_ctx->handle = &hspi3;
    lis2mdl_ctx->read_reg = _lis2mdl_read;
    lis2mdl_ctx->write_reg = _lis2mdl_write;

    lis2mdl_device_id_get(lis2mdl_ctx, &whoami);

    /*if (status != HAL_OK) {
        PRINTLN_ERROR("Failed to read ID (Status %d/%s)", status, hal_status_toString(status));
        return U_ERROR;
    }

    if (whoami != LIS2MDL_ID) {
        PRINTLN_ERROR("Device ID is not for LIS2MDL (Status %d/%s)", status, hal_status_toString(status));
        return U_ERROR;
    }*/

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

    printf("mag: %d, %d, %d\n", raw_axes[0], raw_axes[1], raw_axes[2]);

    send_lightning_board_magnometer_sensor_information(lis2mdl_from_lsb_to_mgauss(raw_axes[0]), lis2mdl_from_lsb_to_mgauss(raw_axes[1]), lis2mdl_from_lsb_to_mgauss(raw_axes[2]));

    return U_SUCCESS;
}
