#include "can_messages_rx.h"

void receive_wheel_buttons(const can_msg_t *message, wheel_buttons_t *wheel_buttons) {
    
    uint8_t data = message->data[0];
    uint64_t button_id_mask = (1ULL << 8) - 1ULL;
    uint64_t button_id_raw = (data >> 0) & button_id_mask;
    wheel_buttons->button_id = (uint8_t)button_id_raw;
}

void receive_front_msb_env(const can_msg_t *message, front_msb_env_t *front_msb_env) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t temp_mask = (1ULL << 16) - 1ULL;
    uint64_t temp_raw = (data >> 16) & temp_mask;
    front_msb_env->temp = (float)(temp_raw / 10);
    uint64_t humidity_mask = (1ULL << 16) - 1ULL;
    uint64_t humidity_raw = (data >> 0) & humidity_mask;
    front_msb_env->humidity = (float)(humidity_raw / 10);
}

void receive_front_msb_accel(const can_msg_t *message, front_msb_accel_t *front_msb_accel) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_force_mask = (1ULL << 16) - 1ULL;
    uint64_t x_force_bits = (data >> 48) & x_force_mask;
    int64_t x_force_raw = (x_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_force_bits | ~x_force_mask)
        : (int64_t)x_force_bits;
    front_msb_accel->x_force = (float)x_force_raw;
    uint64_t y_force_mask = (1ULL << 16) - 1ULL;
    uint64_t y_force_bits = (data >> 32) & y_force_mask;
    int64_t y_force_raw = (y_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_force_bits | ~y_force_mask)
        : (int64_t)y_force_bits;
    front_msb_accel->y_force = (float)y_force_raw;
    uint64_t z_force_mask = (1ULL << 16) - 1ULL;
    uint64_t z_force_bits = (data >> 16) & z_force_mask;
    int64_t z_force_raw = (z_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_force_bits | ~z_force_mask)
        : (int64_t)z_force_bits;
    front_msb_accel->z_force = (float)z_force_raw;
}

void receive_front_msb_gyro(const can_msg_t *message, front_msb_gyro_t *front_msb_gyro) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t x_deg_bits = (data >> 48) & x_deg_mask;
    int64_t x_deg_raw = (x_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_deg_bits | ~x_deg_mask)
        : (int64_t)x_deg_bits;
    front_msb_gyro->x_deg = (float)x_deg_raw;
    uint64_t y_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t y_deg_bits = (data >> 32) & y_deg_mask;
    int64_t y_deg_raw = (y_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_deg_bits | ~y_deg_mask)
        : (int64_t)y_deg_bits;
    front_msb_gyro->y_deg = (float)y_deg_raw;
    uint64_t z_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t z_deg_bits = (data >> 16) & z_deg_mask;
    int64_t z_deg_raw = (z_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_deg_bits | ~z_deg_mask)
        : (int64_t)z_deg_bits;
    front_msb_gyro->z_deg = (float)z_deg_raw;
}

void receive_front_msb_strain(const can_msg_t *message, front_msb_strain_t *front_msb_strain) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t strain1_mask = (1ULL << 32) - 1ULL;
    uint64_t strain1_raw = (data >> 32) & strain1_mask;
    front_msb_strain->strain1 = (uint32_t)strain1_raw;
    uint64_t strain2_mask = (1ULL << 32) - 1ULL;
    uint64_t strain2_raw = (data >> 0) & strain2_mask;
    front_msb_strain->strain2 = (uint32_t)strain2_raw;
}

void receive_front_shockpot(const can_msg_t *message, front_shockpot_t *front_shockpot) {
    
    struct __attribute__((__packed__)) {
        uint32_t shock1;
        uint16_t shock1_raw;
        
    } bitstream_data;

    memcpy(&bitstream_data, message->data, sizeof(bitstream_data));

    
    
    
    front_shockpot->shock1 = (float)bitstream_data.shock1;
    
    
    
    
    
    front_shockpot->shock1_raw = (uint16_t)bitstream_data.shock1_raw;
    
    
    
}

void receive_front_ride_height(const can_msg_t *message, front_ride_height_t *front_ride_height) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t rh_mask = (1ULL << 16) - 1ULL;
    uint64_t rh_bits = (data >> 0) & rh_mask;
    int64_t rh_raw = (rh_bits & (1ULL << (16 - 1)))
        ? (int64_t)(rh_bits | ~rh_mask)
        : (int64_t)rh_bits;
    front_ride_height->rh = (float)rh_raw;
}

void receive_front_wheel_temp(const can_msg_t *message, front_wheel_temp_t *front_wheel_temp) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t wheel_temp_mask = (1ULL << 16) - 1ULL;
    uint64_t wheel_temp_raw = (data >> 0) & wheel_temp_mask;
    front_wheel_temp->wheel_temp = (float)wheel_temp_raw;
}

void receive_front_msb_orientation(const can_msg_t *message, front_msb_orientation_t *front_msb_orientation) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t x_fdeg_bits = (data >> 48) & x_fdeg_mask;
    int64_t x_fdeg_raw = (x_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_fdeg_bits | ~x_fdeg_mask)
        : (int64_t)x_fdeg_bits;
    front_msb_orientation->x_fdeg = (float)x_fdeg_raw;
    uint64_t y_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t y_fdeg_bits = (data >> 32) & y_fdeg_mask;
    int64_t y_fdeg_raw = (y_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_fdeg_bits | ~y_fdeg_mask)
        : (int64_t)y_fdeg_bits;
    front_msb_orientation->y_fdeg = (float)y_fdeg_raw;
    uint64_t z_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t z_fdeg_bits = (data >> 16) & z_fdeg_mask;
    int64_t z_fdeg_raw = (z_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_fdeg_bits | ~z_fdeg_mask)
        : (int64_t)z_fdeg_bits;
    front_msb_orientation->z_fdeg = (float)z_fdeg_raw;
}

void receive_back_msb_env(const can_msg_t *message, back_msb_env_t *back_msb_env) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t temp_mask = (1ULL << 16) - 1ULL;
    uint64_t temp_raw = (data >> 16) & temp_mask;
    back_msb_env->temp = (float)(temp_raw / 10);
    uint64_t humidity_mask = (1ULL << 16) - 1ULL;
    uint64_t humidity_raw = (data >> 0) & humidity_mask;
    back_msb_env->humidity = (float)(humidity_raw / 10);
}

void receive_back_msb_accel(const can_msg_t *message, back_msb_accel_t *back_msb_accel) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_force_mask = (1ULL << 16) - 1ULL;
    uint64_t x_force_bits = (data >> 48) & x_force_mask;
    int64_t x_force_raw = (x_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_force_bits | ~x_force_mask)
        : (int64_t)x_force_bits;
    back_msb_accel->x_force = (float)x_force_raw;
    uint64_t y_force_mask = (1ULL << 16) - 1ULL;
    uint64_t y_force_bits = (data >> 32) & y_force_mask;
    int64_t y_force_raw = (y_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_force_bits | ~y_force_mask)
        : (int64_t)y_force_bits;
    back_msb_accel->y_force = (float)y_force_raw;
    uint64_t z_force_mask = (1ULL << 16) - 1ULL;
    uint64_t z_force_bits = (data >> 16) & z_force_mask;
    int64_t z_force_raw = (z_force_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_force_bits | ~z_force_mask)
        : (int64_t)z_force_bits;
    back_msb_accel->z_force = (float)z_force_raw;
}

void receive_back_msb_gyro(const can_msg_t *message, back_msb_gyro_t *back_msb_gyro) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t x_deg_bits = (data >> 48) & x_deg_mask;
    int64_t x_deg_raw = (x_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_deg_bits | ~x_deg_mask)
        : (int64_t)x_deg_bits;
    back_msb_gyro->x_deg = (float)x_deg_raw;
    uint64_t y_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t y_deg_bits = (data >> 32) & y_deg_mask;
    int64_t y_deg_raw = (y_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_deg_bits | ~y_deg_mask)
        : (int64_t)y_deg_bits;
    back_msb_gyro->y_deg = (float)y_deg_raw;
    uint64_t z_deg_mask = (1ULL << 16) - 1ULL;
    uint64_t z_deg_bits = (data >> 16) & z_deg_mask;
    int64_t z_deg_raw = (z_deg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_deg_bits | ~z_deg_mask)
        : (int64_t)z_deg_bits;
    back_msb_gyro->z_deg = (float)z_deg_raw;
}

void receive_back_msb_strain(const can_msg_t *message, back_msb_strain_t *back_msb_strain) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t strain1_mask = (1ULL << 32) - 1ULL;
    uint64_t strain1_raw = (data >> 32) & strain1_mask;
    back_msb_strain->strain1 = (uint32_t)strain1_raw;
    uint64_t strain2_mask = (1ULL << 32) - 1ULL;
    uint64_t strain2_raw = (data >> 0) & strain2_mask;
    back_msb_strain->strain2 = (uint32_t)strain2_raw;
}

void receive_back_shockpot(const can_msg_t *message, back_shockpot_t *back_shockpot) {
    
    struct __attribute__((__packed__)) {
        uint32_t shock1;
        uint16_t shock1_raw;
        
    } bitstream_data;

    memcpy(&bitstream_data, message->data, sizeof(bitstream_data));

    
    
    
    back_shockpot->shock1 = (float)bitstream_data.shock1;
    
    
    
    
    
    back_shockpot->shock1_raw = (uint16_t)bitstream_data.shock1_raw;
    
    
    
}

void receive_back_ride_height(const can_msg_t *message, back_ride_height_t *back_ride_height) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t rh_mask = (1ULL << 16) - 1ULL;
    uint64_t rh_bits = (data >> 0) & rh_mask;
    int64_t rh_raw = (rh_bits & (1ULL << (16 - 1)))
        ? (int64_t)(rh_bits | ~rh_mask)
        : (int64_t)rh_bits;
    back_ride_height->rh = (float)rh_raw;
}

void receive_back_wheel_temp(const can_msg_t *message, back_wheel_temp_t *back_wheel_temp) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t wheel_temp_mask = (1ULL << 16) - 1ULL;
    uint64_t wheel_temp_raw = (data >> 0) & wheel_temp_mask;
    back_wheel_temp->wheel_temp = (float)wheel_temp_raw;
}

void receive_back_msb_orientation(const can_msg_t *message, back_msb_orientation_t *back_msb_orientation) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t x_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t x_fdeg_bits = (data >> 48) & x_fdeg_mask;
    int64_t x_fdeg_raw = (x_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(x_fdeg_bits | ~x_fdeg_mask)
        : (int64_t)x_fdeg_bits;
    back_msb_orientation->x_fdeg = (float)x_fdeg_raw;
    uint64_t y_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t y_fdeg_bits = (data >> 32) & y_fdeg_mask;
    int64_t y_fdeg_raw = (y_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(y_fdeg_bits | ~y_fdeg_mask)
        : (int64_t)y_fdeg_bits;
    back_msb_orientation->y_fdeg = (float)y_fdeg_raw;
    uint64_t z_fdeg_mask = (1ULL << 16) - 1ULL;
    uint64_t z_fdeg_bits = (data >> 16) & z_fdeg_mask;
    int64_t z_fdeg_raw = (z_fdeg_bits & (1ULL << (16 - 1)))
        ? (int64_t)(z_fdeg_bits | ~z_fdeg_mask)
        : (int64_t)z_fdeg_bits;
    back_msb_orientation->z_fdeg = (float)z_fdeg_raw;
}

void receive_shepherd_bms_fan_percent(const can_msg_t *message, shepherd_bms_fan_percent_t *shepherd_bms_fan_percent) {
    
    uint8_t data = message->data[0];
    uint64_t pwm_duty_mask = (1ULL << 8) - 1ULL;
    uint64_t pwm_duty_raw = (data >> 0) & pwm_duty_mask;
    shepherd_bms_fan_percent->pwm_duty = (uint8_t)pwm_duty_raw;
}

void receive_dashboard_efuse_state(const can_msg_t *message, dashboard_efuse_state_t *dashboard_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    dashboard_efuse_state->state = (uint8_t)state_raw;
}

void receive_brake_efuse_state(const can_msg_t *message, brake_efuse_state_t *brake_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    brake_efuse_state->state = (uint8_t)state_raw;
}

void receive_shutdown_efuse_state(const can_msg_t *message, shutdown_efuse_state_t *shutdown_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    shutdown_efuse_state->state = (uint8_t)state_raw;
}

void receive_lv_efuse_state(const can_msg_t *message, lv_efuse_state_t *lv_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    lv_efuse_state->state = (uint8_t)state_raw;
}

void receive_radfan_efuse_state(const can_msg_t *message, radfan_efuse_state_t *radfan_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    radfan_efuse_state->state = (uint8_t)state_raw;
}

void receive_fanbatt_efuse_state(const can_msg_t *message, fanbatt_efuse_state_t *fanbatt_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    fanbatt_efuse_state->state = (uint8_t)state_raw;
}

void receive_pumpone_efuse_state(const can_msg_t *message, pumpone_efuse_state_t *pumpone_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    pumpone_efuse_state->state = (uint8_t)state_raw;
}

void receive_pumptwo_efuse_state(const can_msg_t *message, pumptwo_efuse_state_t *pumptwo_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    pumptwo_efuse_state->state = (uint8_t)state_raw;
}

void receive_battbox_efuse_state(const can_msg_t *message, battbox_efuse_state_t *battbox_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    battbox_efuse_state->state = (uint8_t)state_raw;
}

void receive_mc_efuse_state(const can_msg_t *message, mc_efuse_state_t *mc_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    mc_efuse_state->state = (uint8_t)state_raw;
}

void receive_spare_efuse_state(const can_msg_t *message, spare_efuse_state_t *spare_efuse_state) {
    
    uint8_t data = message->data[0];
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 0) & state_mask;
    spare_efuse_state->state = (uint8_t)state_raw;
}

void receive_rtds_command_message(const can_msg_t *message, rtds_command_message_t *rtds_command_message) {
    
    uint8_t data = message->data[0];
    uint64_t command_mask = (1ULL << 8) - 1ULL;
    uint64_t command_raw = (data >> 0) & command_mask;
    rtds_command_message->command = (uint8_t)command_raw;
}

void receive_dashboard_efuse(const can_msg_t *message, dashboard_efuse_t *dashboard_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    dashboard_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    dashboard_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    dashboard_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    dashboard_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    dashboard_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    dashboard_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_brake_efuse(const can_msg_t *message, brake_efuse_t *brake_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    brake_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    brake_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    brake_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    brake_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    brake_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    brake_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_shutdown_efuse(const can_msg_t *message, shutdown_efuse_t *shutdown_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    shutdown_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    shutdown_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    shutdown_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    shutdown_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    shutdown_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    shutdown_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_lv_efuse(const can_msg_t *message, lv_efuse_t *lv_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    lv_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    lv_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    lv_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    lv_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    lv_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    lv_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_radfan_efuse(const can_msg_t *message, radfan_efuse_t *radfan_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    radfan_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    radfan_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    radfan_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    radfan_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    radfan_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    radfan_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_fanbatt_efuse(const can_msg_t *message, fanbatt_efuse_t *fanbatt_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    fanbatt_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    fanbatt_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    fanbatt_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    fanbatt_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    fanbatt_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    fanbatt_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_pumpone_efuse(const can_msg_t *message, pumpone_efuse_t *pumpone_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    pumpone_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    pumpone_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    pumpone_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    pumpone_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    pumpone_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    pumpone_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_pumptwo_efuse(const can_msg_t *message, pumptwo_efuse_t *pumptwo_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    pumptwo_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    pumptwo_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    pumptwo_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    pumptwo_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    pumptwo_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    pumptwo_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_battbox_efuse(const can_msg_t *message, battbox_efuse_t *battbox_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    battbox_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    battbox_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    battbox_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    battbox_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    battbox_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    battbox_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_mc_efuse(const can_msg_t *message, mc_efuse_t *mc_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    mc_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    mc_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    mc_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    mc_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    mc_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    mc_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_spare_efuse(const can_msg_t *message, spare_efuse_t *spare_efuse) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    spare_efuse->ADC = (uint16_t)ADC_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    spare_efuse->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_raw = (data >> 16) & current_mask;
    spare_efuse->current = (float)(current_raw / 1000);
    uint64_t is_faulted_mask = (1ULL << 4) - 1ULL;
    uint64_t is_faulted_raw = (data >> 12) & is_faulted_mask;
    spare_efuse->is_faulted = (bool)is_faulted_raw;
    uint64_t is_enabled_mask = (1ULL << 4) - 1ULL;
    uint64_t is_enabled_raw = (data >> 8) & is_enabled_mask;
    spare_efuse->is_enabled = (bool)is_enabled_raw;
    uint64_t control_state_mask = (1ULL << 8) - 1ULL;
    uint64_t control_state_raw = (data >> 0) & control_state_mask;
    spare_efuse->control_state = (uint8_t)control_state_raw;
}

void receive_shutdown_pins(const can_msg_t *message, shutdown_pins_t *shutdown_pins) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t bms_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t bms_gpio_raw = (data >> 15) & bms_gpio_mask;
    shutdown_pins->bms_gpio = (bool)bms_gpio_raw;
    uint64_t bots_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t bots_gpio_raw = (data >> 14) & bots_gpio_mask;
    shutdown_pins->bots_gpio = (bool)bots_gpio_raw;
    uint64_t bspd_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t bspd_gpio_raw = (data >> 13) & bspd_gpio_mask;
    shutdown_pins->bspd_gpio = (bool)bspd_gpio_raw;
    uint64_t hv_c_mask = (1ULL << 1) - 1ULL;
    uint64_t hv_c_raw = (data >> 12) & hv_c_mask;
    shutdown_pins->hv_c = (bool)hv_c_raw;
    uint64_t hvd_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t hvd_gpio_raw = (data >> 11) & hvd_gpio_mask;
    shutdown_pins->hvd_gpio = (bool)hvd_gpio_raw;
    uint64_t imd_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t imd_gpio_raw = (data >> 10) & imd_gpio_mask;
    shutdown_pins->imd_gpio = (bool)imd_gpio_raw;
    uint64_t ckpt_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t ckpt_gpio_raw = (data >> 9) & ckpt_gpio_mask;
    shutdown_pins->ckpt_gpio = (bool)ckpt_gpio_raw;
    uint64_t inertia_sw_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t inertia_sw_gpio_raw = (data >> 8) & inertia_sw_gpio_mask;
    shutdown_pins->inertia_sw_gpio = (bool)inertia_sw_gpio_raw;
    uint64_t tsms_gpio_mask = (1ULL << 1) - 1ULL;
    uint64_t tsms_gpio_raw = (data >> 7) & tsms_gpio_mask;
    shutdown_pins->tsms_gpio = (bool)tsms_gpio_raw;
}

void receive_car_state(const can_msg_t *message, car_state_t *car_state) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t home_mode_mask = (1ULL << 4) - 1ULL;
    uint64_t home_mode_raw = (data >> 60) & home_mode_mask;
    car_state->home_mode = (bool)home_mode_raw;
    uint64_t nero_index_mask = (1ULL << 4) - 1ULL;
    uint64_t nero_index_raw = (data >> 56) & nero_index_mask;
    car_state->nero_index = (uint8_t)nero_index_raw;
    uint64_t car_speed_mask = (1ULL << 16) - 1ULL;
    uint64_t car_speed_bits = (data >> 40) & car_speed_mask;
    int64_t car_speed_raw = (car_speed_bits & (1ULL << (16 - 1)))
        ? (int64_t)(car_speed_bits | ~car_speed_mask)
        : (int64_t)car_speed_bits;
    car_state->car_speed = (float)(car_speed_raw / 10);
    uint64_t tsms_mask = (1ULL << 1) - 1ULL;
    uint64_t tsms_raw = (data >> 39) & tsms_mask;
    car_state->tsms = (bool)tsms_raw;
    uint64_t torque_limit_percentage_mask = (1ULL << 7) - 1ULL;
    uint64_t torque_limit_percentage_raw = (data >> 32) & torque_limit_percentage_mask;
    car_state->torque_limit_percentage = (float)(torque_limit_percentage_raw / 100);
    uint64_t reverse_mask = (1ULL << 1) - 1ULL;
    uint64_t reverse_raw = (data >> 31) & reverse_mask;
    car_state->reverse = (bool)reverse_raw;
    uint64_t regen_limit_mask = (1ULL << 10) - 1ULL;
    uint64_t regen_limit_raw = (data >> 21) & regen_limit_mask;
    car_state->regen_limit = (uint16_t)regen_limit_raw;
    uint64_t launch_control_mask = (1ULL << 1) - 1ULL;
    uint64_t launch_control_raw = (data >> 20) & launch_control_mask;
    car_state->launch_control = (bool)launch_control_raw;
    uint64_t functional_state_mask = (1ULL << 3) - 1ULL;
    uint64_t functional_state_raw = (data >> 17) & functional_state_mask;
    car_state->functional_state = (uint8_t)functional_state_raw;
    uint64_t traction_control_mask = (1ULL << 1) - 1ULL;
    uint64_t traction_control_raw = (data >> 16) & traction_control_mask;
    car_state->traction_control = (bool)traction_control_raw;
}

void receive_pedal_percent_pressed_values(const can_msg_t *message, pedal_percent_pressed_values_t *pedal_percent_pressed_values) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t accel_norm_mask = (1ULL << 16) - 1ULL;
    uint64_t accel_norm_raw = (data >> 48) & accel_norm_mask;
    pedal_percent_pressed_values->accel_norm = (float)(accel_norm_raw / 100);
    uint64_t brake_norm_mask = (1ULL << 16) - 1ULL;
    uint64_t brake_norm_raw = (data >> 32) & brake_norm_mask;
    pedal_percent_pressed_values->brake_norm = (float)(brake_norm_raw / 100);
    uint64_t brake_psi_brake1_mask = (1ULL << 16) - 1ULL;
    uint64_t brake_psi_brake1_bits = (data >> 16) & brake_psi_brake1_mask;
    int64_t brake_psi_brake1_raw = (brake_psi_brake1_bits & (1ULL << (16 - 1)))
        ? (int64_t)(brake_psi_brake1_bits | ~brake_psi_brake1_mask)
        : (int64_t)brake_psi_brake1_bits;
    pedal_percent_pressed_values->brake_psi_brake1 = (float)(brake_psi_brake1_raw / 10);
    uint64_t brake_psi_brake2_mask = (1ULL << 16) - 1ULL;
    uint64_t brake_psi_brake2_bits = (data >> 0) & brake_psi_brake2_mask;
    int64_t brake_psi_brake2_raw = (brake_psi_brake2_bits & (1ULL << (16 - 1)))
        ? (int64_t)(brake_psi_brake2_bits | ~brake_psi_brake2_mask)
        : (int64_t)brake_psi_brake2_bits;
    pedal_percent_pressed_values->brake_psi_brake2 = (float)(brake_psi_brake2_raw / 10);
}

void receive_pedal_sensor_voltages(const can_msg_t *message, pedal_sensor_voltages_t *pedal_sensor_voltages) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t accel1_volts_mask = (1ULL << 16) - 1ULL;
    uint64_t accel1_volts_raw = (data >> 48) & accel1_volts_mask;
    pedal_sensor_voltages->accel1_volts = (float)(accel1_volts_raw / 100);
    uint64_t accel2_volts_mask = (1ULL << 16) - 1ULL;
    uint64_t accel2_volts_raw = (data >> 32) & accel2_volts_mask;
    pedal_sensor_voltages->accel2_volts = (float)(accel2_volts_raw / 100);
    uint64_t brake1_volts_mask = (1ULL << 16) - 1ULL;
    uint64_t brake1_volts_raw = (data >> 16) & brake1_volts_mask;
    pedal_sensor_voltages->brake1_volts = (float)(brake1_volts_raw / 100);
    uint64_t brake2_volts_mask = (1ULL << 16) - 1ULL;
    uint64_t brake2_volts_raw = (data >> 0) & brake2_volts_mask;
    pedal_sensor_voltages->brake2_volts = (float)(brake2_volts_raw / 100);
}

void receive_lightning_board_light_status(const can_msg_t *message, lightning_board_light_status_t *lightning_board_light_status) {
    
    uint8_t data = message->data[0];
    uint64_t status_mask = (1ULL << 2) - 1ULL;
    uint64_t status_raw = (data >> 6) & status_mask;
    lightning_board_light_status->status = (uint8_t)status_raw;
}

void receive_temperature_sensor(const can_msg_t *message, temperature_sensor_t *temperature_sensor) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t vcu_temperature_mask = (1ULL << 16) - 1ULL;
    uint64_t vcu_temperature_bits = (data >> 16) & vcu_temperature_mask;
    int64_t vcu_temperature_raw = (vcu_temperature_bits & (1ULL << (16 - 1)))
        ? (int64_t)(vcu_temperature_bits | ~vcu_temperature_mask)
        : (int64_t)vcu_temperature_bits;
    temperature_sensor->vcu_temperature = (float)(vcu_temperature_raw / 100);
    uint64_t vcu_humidity_mask = (1ULL << 16) - 1ULL;
    uint64_t vcu_humidity_raw = (data >> 0) & vcu_humidity_mask;
    temperature_sensor->vcu_humidity = (float)(vcu_humidity_raw / 100);
}

void receive_imu_accelerometer(const can_msg_t *message, imu_accelerometer_t *imu_accelerometer) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t imu_accelerometer_x_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_x_bits = (data >> 48) & imu_accelerometer_x_mask;
    int64_t imu_accelerometer_x_raw = (imu_accelerometer_x_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_x_bits | ~imu_accelerometer_x_mask)
        : (int64_t)imu_accelerometer_x_bits;
    imu_accelerometer->imu_accelerometer_x = (float)(imu_accelerometer_x_raw / 4);
    uint64_t imu_accelerometer_y_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_y_bits = (data >> 32) & imu_accelerometer_y_mask;
    int64_t imu_accelerometer_y_raw = (imu_accelerometer_y_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_y_bits | ~imu_accelerometer_y_mask)
        : (int64_t)imu_accelerometer_y_bits;
    imu_accelerometer->imu_accelerometer_y = (float)(imu_accelerometer_y_raw / 4);
    uint64_t imu_accelerometer_z_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_z_bits = (data >> 16) & imu_accelerometer_z_mask;
    int64_t imu_accelerometer_z_raw = (imu_accelerometer_z_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_z_bits | ~imu_accelerometer_z_mask)
        : (int64_t)imu_accelerometer_z_bits;
    imu_accelerometer->imu_accelerometer_z = (float)(imu_accelerometer_z_raw / 4);
}

void receive_imu_gyro(const can_msg_t *message, imu_gyro_t *imu_gyro) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t imu_gyro_x_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_x_bits = (data >> 48) & imu_gyro_x_mask;
    int64_t imu_gyro_x_raw = (imu_gyro_x_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_x_bits | ~imu_gyro_x_mask)
        : (int64_t)imu_gyro_x_bits;
    imu_gyro->imu_gyro_x = (float)(imu_gyro_x_raw / 4);
    uint64_t imu_gyro_y_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_y_bits = (data >> 32) & imu_gyro_y_mask;
    int64_t imu_gyro_y_raw = (imu_gyro_y_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_y_bits | ~imu_gyro_y_mask)
        : (int64_t)imu_gyro_y_bits;
    imu_gyro->imu_gyro_y = (float)(imu_gyro_y_raw / 4);
    uint64_t imu_gyro_z_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_z_bits = (data >> 16) & imu_gyro_z_mask;
    int64_t imu_gyro_z_raw = (imu_gyro_z_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_z_bits | ~imu_gyro_z_mask)
        : (int64_t)imu_gyro_z_bits;
    imu_gyro->imu_gyro_z = (float)(imu_gyro_z_raw / 4);
}

void receive_faults(const can_msg_t *message, faults_t *faults) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t CAN_OUTGOING_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t CAN_OUTGOING_FAULT_raw = (data >> 31) & CAN_OUTGOING_FAULT_mask;
    faults->CAN_OUTGOING_FAULT = (bool)CAN_OUTGOING_FAULT_raw;
    uint64_t CAN_INCOMING_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t CAN_INCOMING_FAULT_raw = (data >> 30) & CAN_INCOMING_FAULT_mask;
    faults->CAN_INCOMING_FAULT = (bool)CAN_INCOMING_FAULT_raw;
    uint64_t BMS_CAN_MONITOR_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t BMS_CAN_MONITOR_FAULT_raw = (data >> 29) & BMS_CAN_MONITOR_FAULT_mask;
    faults->BMS_CAN_MONITOR_FAULT = (bool)BMS_CAN_MONITOR_FAULT_raw;
    uint64_t LIGHTNING_CAN_MONITOR_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t LIGHTNING_CAN_MONITOR_FAULT_raw = (data >> 28) & LIGHTNING_CAN_MONITOR_FAULT_mask;
    faults->LIGHTNING_CAN_MONITOR_FAULT = (bool)LIGHTNING_CAN_MONITOR_FAULT_raw;
    uint64_t ONBOARD_TEMP_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_TEMP_FAULT_raw = (data >> 27) & ONBOARD_TEMP_FAULT_mask;
    faults->ONBOARD_TEMP_FAULT = (bool)ONBOARD_TEMP_FAULT_raw;
    uint64_t IMU_ACCEL_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t IMU_ACCEL_FAULT_raw = (data >> 26) & IMU_ACCEL_FAULT_mask;
    faults->IMU_ACCEL_FAULT = (bool)IMU_ACCEL_FAULT_raw;
    uint64_t IMU_GYRO_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t IMU_GYRO_FAULT_raw = (data >> 25) & IMU_GYRO_FAULT_mask;
    faults->IMU_GYRO_FAULT = (bool)IMU_GYRO_FAULT_raw;
    uint64_t BSPD_PREFAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t BSPD_PREFAULT_raw = (data >> 24) & BSPD_PREFAULT_mask;
    faults->BSPD_PREFAULT = (bool)BSPD_PREFAULT_raw;
    uint64_t ONBOARD_BRAKE_OPEN_CIRCUIT_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_BRAKE_OPEN_CIRCUIT_FAULT_raw = (data >> 23) & ONBOARD_BRAKE_OPEN_CIRCUIT_FAULT_mask;
    faults->ONBOARD_BRAKE_OPEN_CIRCUIT_FAULT = (bool)ONBOARD_BRAKE_OPEN_CIRCUIT_FAULT_raw;
    uint64_t ONBOARD_ACCEL_OPEN_CIRCUIT_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_ACCEL_OPEN_CIRCUIT_FAULT_raw = (data >> 22) & ONBOARD_ACCEL_OPEN_CIRCUIT_FAULT_mask;
    faults->ONBOARD_ACCEL_OPEN_CIRCUIT_FAULT = (bool)ONBOARD_ACCEL_OPEN_CIRCUIT_FAULT_raw;
    uint64_t ONBOARD_BRAKE_SHORT_CIRCUIT_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_BRAKE_SHORT_CIRCUIT_FAULT_raw = (data >> 21) & ONBOARD_BRAKE_SHORT_CIRCUIT_FAULT_mask;
    faults->ONBOARD_BRAKE_SHORT_CIRCUIT_FAULT = (bool)ONBOARD_BRAKE_SHORT_CIRCUIT_FAULT_raw;
    uint64_t ONBOARD_ACCEL_SHORT_CIRCUIT_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_ACCEL_SHORT_CIRCUIT_FAULT_raw = (data >> 20) & ONBOARD_ACCEL_SHORT_CIRCUIT_FAULT_mask;
    faults->ONBOARD_ACCEL_SHORT_CIRCUIT_FAULT = (bool)ONBOARD_ACCEL_SHORT_CIRCUIT_FAULT_raw;
    uint64_t ONBOARD_PEDAL_DIFFERENCE_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t ONBOARD_PEDAL_DIFFERENCE_FAULT_raw = (data >> 19) & ONBOARD_PEDAL_DIFFERENCE_FAULT_mask;
    faults->ONBOARD_PEDAL_DIFFERENCE_FAULT = (bool)ONBOARD_PEDAL_DIFFERENCE_FAULT_raw;
    uint64_t RTDS_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t RTDS_FAULT_raw = (data >> 18) & RTDS_FAULT_mask;
    faults->RTDS_FAULT = (bool)RTDS_FAULT_raw;
    uint64_t LV_LOW_VOLTAGE_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t LV_LOW_VOLTAGE_FAULT_raw = (data >> 17) & LV_LOW_VOLTAGE_FAULT_mask;
    faults->LV_LOW_VOLTAGE_FAULT = (bool)LV_LOW_VOLTAGE_FAULT_raw;
    uint64_t PRECHARGE_FLOATING_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t PRECHARGE_FLOATING_FAULT_raw = (data >> 16) & PRECHARGE_FLOATING_FAULT_mask;
    faults->PRECHARGE_FLOATING_FAULT = (bool)PRECHARGE_FLOATING_FAULT_raw;
    uint64_t LATCHING_ACTIVE_FAULT_mask = (1ULL << 1) - 1ULL;
    uint64_t LATCHING_ACTIVE_FAULT_raw = (data >> 15) & LATCHING_ACTIVE_FAULT_mask;
    faults->LATCHING_ACTIVE_FAULT = (bool)LATCHING_ACTIVE_FAULT_raw;
}

void receive_lv_voltage(const can_msg_t *message, lv_voltage_t *lv_voltage) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t ADC_mask = (1ULL << 16) - 1ULL;
    uint64_t ADC_raw = (data >> 48) & ADC_mask;
    lv_voltage->ADC = (uint16_t)ADC_raw;
    uint64_t Voltage_mask = (1ULL << 32) - 1ULL;
    uint64_t Voltage_raw = (data >> 16) & Voltage_mask;
    lv_voltage->Voltage = (float)(Voltage_raw / 1000);
}

void receive_vcu_test_message(const can_msg_t *message, vcu_test_message_t *vcu_test_message) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t three_bits_mask = (1ULL << 3) - 1ULL;
    uint64_t three_bits_raw = (data >> 61) & three_bits_mask;
    vcu_test_message->three_bits = (uint8_t)three_bits_raw;
    uint64_t float_value_mask = (1ULL << 32) - 1ULL;
    uint64_t float_value_bits = (data >> 29) & float_value_mask;
    int64_t float_value_raw = (float_value_bits & (1ULL << (32 - 1)))
        ? (int64_t)(float_value_bits | ~float_value_mask)
        : (int64_t)float_value_bits;
    vcu_test_message->float_value = (float)(float_value_raw / 100);
    uint64_t five_bits_mask = (1ULL << 5) - 1ULL;
    uint64_t five_bits_raw = (data >> 24) & five_bits_mask;
    vcu_test_message->five_bits = (uint8_t)five_bits_raw;
    uint64_t sixteen_bits_mask = (1ULL << 16) - 1ULL;
    uint64_t sixteen_bits_raw = (data >> 8) & sixteen_bits_mask;
    vcu_test_message->sixteen_bits = (uint16_t)sixteen_bits_raw;
    uint64_t signed_8_bits_mask = (1ULL << 8) - 1ULL;
    uint64_t signed_8_bits_bits = (data >> 0) & signed_8_bits_mask;
    int64_t signed_8_bits_raw = (signed_8_bits_bits & (1ULL << (8 - 1)))
        ? (int64_t)(signed_8_bits_bits | ~signed_8_bits_mask)
        : (int64_t)signed_8_bits_bits;
    vcu_test_message->signed_8_bits = (int8_t)signed_8_bits_raw;
}

void receive_dti_motor_temp_as_reported_by_vcu(const can_msg_t *message, dti_motor_temp_as_reported_by_vcu_t *dti_motor_temp_as_reported_by_vcu) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t temp_mask = (1ULL << 16) - 1ULL;
    uint64_t temp_raw = (data >> 0) & temp_mask;
    dti_motor_temp_as_reported_by_vcu->temp = (uint16_t)temp_raw;
}

void receive_dti_controller_temp_as_reported_by_vcu(const can_msg_t *message, dti_controller_temp_as_reported_by_vcu_t *dti_controller_temp_as_reported_by_vcu) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t temp_mask = (1ULL << 16) - 1ULL;
    uint64_t temp_raw = (data >> 0) & temp_mask;
    dti_controller_temp_as_reported_by_vcu->temp = (uint16_t)temp_raw;
}

void receive_bms_battbox_temp_as_reported_by_vcu(const can_msg_t *message, bms_battbox_temp_as_reported_by_vcu_t *bms_battbox_temp_as_reported_by_vcu) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t temp_mask = (1ULL << 32) - 1ULL;
    uint64_t temp_bits = (data >> 0) & temp_mask;
    int64_t temp_raw = (temp_bits & (1ULL << (32 - 1)))
        ? (int64_t)(temp_bits | ~temp_mask)
        : (int64_t)temp_bits;
    bms_battbox_temp_as_reported_by_vcu->temp = (float)(temp_raw / 100);
}

void receive_brake_state_as_reported_by_vcu(const can_msg_t *message, brake_state_as_reported_by_vcu_t *brake_state_as_reported_by_vcu) {
    
    uint8_t data = message->data[0];
    uint64_t brake_state_mask = (1ULL << 8) - 1ULL;
    uint64_t brake_state_bits = (data >> 0) & brake_state_mask;
    int64_t brake_state_raw = (brake_state_bits & (1ULL << (8 - 1)))
        ? (int64_t)(brake_state_bits | ~brake_state_mask)
        : (int64_t)brake_state_bits;
    brake_state_as_reported_by_vcu->brake_state = (bool)(brake_state_raw / 100);
}

void receive_rtds_state_message(const can_msg_t *message, rtds_state_message_t *rtds_state_message) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t pin_state_mask = (1ULL << 8) - 1ULL;
    uint64_t pin_state_raw = (data >> 24) & pin_state_mask;
    rtds_state_message->pin_state = (bool)pin_state_raw;
    uint64_t sounding_state_mask = (1ULL << 8) - 1ULL;
    uint64_t sounding_state_raw = (data >> 16) & sounding_state_mask;
    rtds_state_message->sounding_state = (bool)sounding_state_raw;
    uint64_t reverse_state_mask = (1ULL << 8) - 1ULL;
    uint64_t reverse_state_raw = (data >> 8) & reverse_state_mask;
    rtds_state_message->reverse_state = (bool)reverse_state_raw;
    uint64_t error_mask = (1ULL << 8) - 1ULL;
    uint64_t error_raw = (data >> 0) & error_mask;
    rtds_state_message->error = (bool)error_raw;
}

void receive_lfiu_low_current_adc_readings(const can_msg_t *message, lfiu_low_current_adc_readings_t *lfiu_low_current_adc_readings) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t raw_mask = (1ULL << 16) - 1ULL;
    uint64_t raw_raw = (data >> 48) & raw_mask;
    lfiu_low_current_adc_readings->raw = (uint16_t)raw_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    lfiu_low_current_adc_readings->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_bits = (data >> 16) & current_mask;
    int64_t current_raw = (current_bits & (1ULL << (16 - 1)))
        ? (int64_t)(current_bits | ~current_mask)
        : (int64_t)current_bits;
    lfiu_low_current_adc_readings->current = (float)(current_raw / 1000);
}

void receive_lfiu_high_current_adc_readings(const can_msg_t *message, lfiu_high_current_adc_readings_t *lfiu_high_current_adc_readings) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t raw_mask = (1ULL << 16) - 1ULL;
    uint64_t raw_raw = (data >> 48) & raw_mask;
    lfiu_high_current_adc_readings->raw = (uint16_t)raw_raw;
    uint64_t voltage_mask = (1ULL << 16) - 1ULL;
    uint64_t voltage_raw = (data >> 32) & voltage_mask;
    lfiu_high_current_adc_readings->voltage = (float)(voltage_raw / 1000);
    uint64_t current_mask = (1ULL << 16) - 1ULL;
    uint64_t current_bits = (data >> 16) & current_mask;
    int64_t current_raw = (current_bits & (1ULL << (16 - 1)))
        ? (int64_t)(current_bits | ~current_mask)
        : (int64_t)current_bits;
    lfiu_high_current_adc_readings->current = (float)(current_raw / 100);
}

void receive_second_vcu_test_message(const can_msg_t *message, second_vcu_test_message_t *second_vcu_test_message) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t one_mask = (1ULL << 14) - 1ULL;
    uint64_t one_raw = (data >> 50) & one_mask;
    second_vcu_test_message->one = (uint16_t)one_raw;
    uint64_t two_mask = (1ULL << 2) - 1ULL;
    uint64_t two_raw = (data >> 48) & two_mask;
    second_vcu_test_message->two = (uint8_t)two_raw;
    uint64_t three_mask = (1ULL << 2) - 1ULL;
    uint64_t three_raw = (data >> 46) & three_mask;
    second_vcu_test_message->three = (uint8_t)three_raw;
    uint64_t four_mask = (1ULL << 1) - 1ULL;
    uint64_t four_raw = (data >> 45) & four_mask;
    second_vcu_test_message->four = (bool)four_raw;
    uint64_t five_mask = (1ULL << 6) - 1ULL;
    uint64_t five_raw = (data >> 39) & five_mask;
    second_vcu_test_message->five = (uint8_t)five_raw;
    uint64_t six_mask = (1ULL << 23) - 1ULL;
    uint64_t six_raw = (data >> 16) & six_mask;
    second_vcu_test_message->six = (uint32_t)six_raw;
}

void receive_lv_box_fan_pwm(const can_msg_t *message, lv_box_fan_pwm_t *lv_box_fan_pwm) {
    
    uint8_t data = message->data[0];
    uint64_t fan_pwm_percentage_mask = (1ULL << 8) - 1ULL;
    uint64_t fan_pwm_percentage_raw = (data >> 0) & fan_pwm_percentage_mask;
    lv_box_fan_pwm->fan_pwm_percentage = (uint8_t)fan_pwm_percentage_raw;
}

void receive_bms_shutdown_status_as_reported_by_vcu(const can_msg_t *message, bms_shutdown_status_as_reported_by_vcu_t *bms_shutdown_status_as_reported_by_vcu) {
    
    uint8_t data = message->data[0];
    uint64_t bms_shutdown_as_reported_by_vcu_mask = (1ULL << 8) - 1ULL;
    uint64_t bms_shutdown_as_reported_by_vcu_raw = (data >> 0) & bms_shutdown_as_reported_by_vcu_mask;
    bms_shutdown_status_as_reported_by_vcu->bms_shutdown_as_reported_by_vcu = (bool)bms_shutdown_as_reported_by_vcu_raw;
}

void receive_drive_lock_states(const can_msg_t *message, drive_lock_states_t *drive_lock_states) {
    
    uint8_t data = message->data[0];
    uint64_t BRAKE_OC_mask = (1ULL << 1) - 1ULL;
    uint64_t BRAKE_OC_raw = (data >> 7) & BRAKE_OC_mask;
    drive_lock_states->BRAKE_OC = (bool)BRAKE_OC_raw;
    uint64_t BRAKE_SC_mask = (1ULL << 1) - 1ULL;
    uint64_t BRAKE_SC_raw = (data >> 6) & BRAKE_SC_mask;
    drive_lock_states->BRAKE_SC = (bool)BRAKE_SC_raw;
    uint64_t ACCEL_OC_mask = (1ULL << 1) - 1ULL;
    uint64_t ACCEL_OC_raw = (data >> 5) & ACCEL_OC_mask;
    drive_lock_states->ACCEL_OC = (bool)ACCEL_OC_raw;
    uint64_t ACCEL_SC_mask = (1ULL << 1) - 1ULL;
    uint64_t ACCEL_SC_raw = (data >> 4) & ACCEL_SC_mask;
    drive_lock_states->ACCEL_SC = (bool)ACCEL_SC_raw;
    uint64_t ACCEL_DIFF_mask = (1ULL << 1) - 1ULL;
    uint64_t ACCEL_DIFF_raw = (data >> 3) & ACCEL_DIFF_mask;
    drive_lock_states->ACCEL_DIFF = (bool)ACCEL_DIFF_raw;
    uint64_t BSPD_PREF_mask = (1ULL << 1) - 1ULL;
    uint64_t BSPD_PREF_raw = (data >> 2) & BSPD_PREF_mask;
    drive_lock_states->BSPD_PREF = (bool)BSPD_PREF_raw;
    uint64_t BMS_NOT_PRECHARGED_YET_mask = (1ULL << 1) - 1ULL;
    uint64_t BMS_NOT_PRECHARGED_YET_raw = (data >> 1) & BMS_NOT_PRECHARGED_YET_mask;
    drive_lock_states->BMS_NOT_PRECHARGED_YET = (bool)BMS_NOT_PRECHARGED_YET_raw;
}

void receive_reset_latching_fault(const can_msg_t *message, reset_latching_fault_t *reset_latching_fault) {
    
    uint8_t data = message->data[0];
    uint64_t reset_latching_mask = (1ULL << 1) - 1ULL;
    uint64_t reset_latching_raw = (data >> 7) & reset_latching_mask;
    reset_latching_fault->reset_latching = (bool)reset_latching_raw;
}

void receive_imd_general_information(const can_msg_t *message, imd_general_information_t *imd_general_information) {
    
    struct __attribute__((__packed__)) {
        uint16_t R_iso_corrected;
        uint8_t R_iso_status;
        uint8_t Iso_measurement_counter;
        uint8_t device_error;
        uint8_t HV_pos_conn_fail;
        uint8_t HV_neg_conn_fail;
        uint8_t Earth_conn_fail;
        uint8_t Iso_alarm;
        uint8_t iso_warning;
        uint8_t iso_outdated;
        uint8_t Unbalance_alarm;
        uint8_t Undervoltage_alarm;
        uint8_t Unsafe_to_start;
        uint8_t Earthlift_Open;
        uint8_t warnings_and_alarms_unused_bits;
        uint8_t Device_Activity;
        uint8_t Not_Applicable;
        
    } bitstream_data;

    memcpy(&bitstream_data, message->data, sizeof(bitstream_data));

    
    
    
    imd_general_information->R_iso_corrected = (uint16_t)bitstream_data.R_iso_corrected;
    
    
    
    
    
    imd_general_information->R_iso_status = (uint8_t)bitstream_data.R_iso_status;
    
    
    
    
    
    imd_general_information->Iso_measurement_counter = (uint8_t)bitstream_data.Iso_measurement_counter;
    
    
    
    
    
    imd_general_information->device_error = (bool)bitstream_data.device_error;
    
    
    
    
    
    imd_general_information->HV_pos_conn_fail = (bool)bitstream_data.HV_pos_conn_fail;
    
    
    
    
    
    imd_general_information->HV_neg_conn_fail = (bool)bitstream_data.HV_neg_conn_fail;
    
    
    
    
    
    imd_general_information->Earth_conn_fail = (bool)bitstream_data.Earth_conn_fail;
    
    
    
    
    
    imd_general_information->Iso_alarm = (bool)bitstream_data.Iso_alarm;
    
    
    
    
    
    imd_general_information->iso_warning = (bool)bitstream_data.iso_warning;
    
    
    
    
    
    imd_general_information->iso_outdated = (bool)bitstream_data.iso_outdated;
    
    
    
    
    
    imd_general_information->Unbalance_alarm = (bool)bitstream_data.Unbalance_alarm;
    
    
    
    
    
    imd_general_information->Undervoltage_alarm = (bool)bitstream_data.Undervoltage_alarm;
    
    
    
    
    
    imd_general_information->Unsafe_to_start = (bool)bitstream_data.Unsafe_to_start;
    
    
    
    
    
    
    
}

void receive_bms_charge_message_send(const can_msg_t *message, bms_charge_message_send_t *bms_charge_message_send) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t charge_volts_mask = (1ULL << 16) - 1ULL;
    uint64_t charge_volts_raw = (data >> 48) & charge_volts_mask;
    bms_charge_message_send->charge_volts = (float)(charge_volts_raw / 10);
    uint64_t charge_current_mask = (1ULL << 16) - 1ULL;
    uint64_t charge_current_raw = (data >> 32) & charge_current_mask;
    bms_charge_message_send->charge_current = (float)(charge_current_raw / 10);
    uint64_t enable_charging_mask = (1ULL << 8) - 1ULL;
    uint64_t enable_charging_raw = (data >> 24) & enable_charging_mask;
    bms_charge_message_send->enable_charging = (uint8_t)enable_charging_raw;
}

void receive_bms_status(const can_msg_t *message, bms_status_t *bms_status) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 8) & state_mask;
    bms_status->state = (uint8_t)state_raw;
    uint64_t temp_average_mask = (1ULL << 8) - 1ULL;
    uint64_t temp_average_raw = (data >> 0) & temp_average_mask;
    bms_status->temp_average = (float)temp_average_raw;
}

void receive_cell_voltage(const can_msg_t *message, cell_voltage_t *cell_voltage) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t high_val_mask = (1ULL << 16) - 1ULL;
    uint64_t high_val_raw = (data >> 48) & high_val_mask;
    cell_voltage->high_val = (float)(high_val_raw / 10000);
    uint64_t high_chip_mask = (1ULL << 4) - 1ULL;
    uint64_t high_chip_raw = (data >> 44) & high_chip_mask;
    cell_voltage->high_chip = (uint8_t)high_chip_raw;
    uint64_t high_cell_mask = (1ULL << 4) - 1ULL;
    uint64_t high_cell_raw = (data >> 40) & high_cell_mask;
    cell_voltage->high_cell = (uint8_t)high_cell_raw;
    uint64_t low_val_mask = (1ULL << 16) - 1ULL;
    uint64_t low_val_raw = (data >> 24) & low_val_mask;
    cell_voltage->low_val = (float)(low_val_raw / 10000);
    uint64_t low_chip_mask = (1ULL << 4) - 1ULL;
    uint64_t low_chip_raw = (data >> 20) & low_chip_mask;
    cell_voltage->low_chip = (uint8_t)low_chip_raw;
    uint64_t low_cell_mask = (1ULL << 4) - 1ULL;
    uint64_t low_cell_raw = (data >> 16) & low_cell_mask;
    cell_voltage->low_cell = (uint8_t)low_cell_raw;
    uint64_t avg_val_mask = (1ULL << 16) - 1ULL;
    uint64_t avg_val_raw = (data >> 0) & avg_val_mask;
    cell_voltage->avg_val = (float)(avg_val_raw / 10000);
}

void receive_cell_temperatures(const can_msg_t *message, cell_temperatures_t *cell_temperatures) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t high_val_mask = (1ULL << 16) - 1ULL;
    uint64_t high_val_raw = (data >> 48) & high_val_mask;
    cell_temperatures->high_val = (float)(high_val_raw / 100);
    uint64_t high_chip_mask = (1ULL << 4) - 1ULL;
    uint64_t high_chip_raw = (data >> 44) & high_chip_mask;
    cell_temperatures->high_chip = (uint8_t)high_chip_raw;
    uint64_t high_cell_mask = (1ULL << 4) - 1ULL;
    uint64_t high_cell_raw = (data >> 40) & high_cell_mask;
    cell_temperatures->high_cell = (uint8_t)high_cell_raw;
    uint64_t low_val_mask = (1ULL << 16) - 1ULL;
    uint64_t low_val_raw = (data >> 24) & low_val_mask;
    cell_temperatures->low_val = (float)(low_val_raw / 100);
    uint64_t low_chip_mask = (1ULL << 4) - 1ULL;
    uint64_t low_chip_raw = (data >> 20) & low_chip_mask;
    cell_temperatures->low_chip = (uint8_t)low_chip_raw;
    uint64_t low_cell_mask = (1ULL << 4) - 1ULL;
    uint64_t low_cell_raw = (data >> 16) & low_cell_mask;
    cell_temperatures->low_cell = (uint8_t)low_cell_raw;
    uint64_t avg_val_mask = (1ULL << 16) - 1ULL;
    uint64_t avg_val_raw = (data >> 0) & avg_val_mask;
    cell_temperatures->avg_val = (float)(avg_val_raw / 100);
}

void receive_segment_temperatures(const can_msg_t *message, segment_temperatures_t *segment_temperatures) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t seg1_mask = (1ULL << 8) - 1ULL;
    uint64_t seg1_bits = (data >> 56) & seg1_mask;
    int64_t seg1_raw = (seg1_bits & (1ULL << (8 - 1)))
        ? (int64_t)(seg1_bits | ~seg1_mask)
        : (int64_t)seg1_bits;
    segment_temperatures->seg1 = (float)seg1_raw;
    uint64_t seg2_mask = (1ULL << 8) - 1ULL;
    uint64_t seg2_bits = (data >> 48) & seg2_mask;
    int64_t seg2_raw = (seg2_bits & (1ULL << (8 - 1)))
        ? (int64_t)(seg2_bits | ~seg2_mask)
        : (int64_t)seg2_bits;
    segment_temperatures->seg2 = (float)seg2_raw;
    uint64_t seg3_mask = (1ULL << 8) - 1ULL;
    uint64_t seg3_bits = (data >> 40) & seg3_mask;
    int64_t seg3_raw = (seg3_bits & (1ULL << (8 - 1)))
        ? (int64_t)(seg3_bits | ~seg3_mask)
        : (int64_t)seg3_bits;
    segment_temperatures->seg3 = (float)seg3_raw;
    uint64_t seg4_mask = (1ULL << 8) - 1ULL;
    uint64_t seg4_bits = (data >> 32) & seg4_mask;
    int64_t seg4_raw = (seg4_bits & (1ULL << (8 - 1)))
        ? (int64_t)(seg4_bits | ~seg4_mask)
        : (int64_t)seg4_bits;
    segment_temperatures->seg4 = (float)seg4_raw;
    uint64_t seg5_mask = (1ULL << 8) - 1ULL;
    uint64_t seg5_bits = (data >> 24) & seg5_mask;
    int64_t seg5_raw = (seg5_bits & (1ULL << (8 - 1)))
        ? (int64_t)(seg5_bits | ~seg5_mask)
        : (int64_t)seg5_bits;
    segment_temperatures->seg5 = (float)seg5_raw;
}

void receive_segment_isospi_communication_status(const can_msg_t *message, segment_isospi_communication_status_t *segment_isospi_communication_status) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 24) & state_mask;
    segment_isospi_communication_status->state = (uint8_t)state_raw;
    uint64_t break_location_mask = (1ULL << 8) - 1ULL;
    uint64_t break_location_raw = (data >> 16) & break_location_mask;
    segment_isospi_communication_status->break_location = (uint8_t)break_location_raw;
    uint64_t verification_attempts_mask = (1ULL << 8) - 1ULL;
    uint64_t verification_attempts_raw = (data >> 8) & verification_attempts_mask;
    segment_isospi_communication_status->verification_attempts = (uint8_t)verification_attempts_raw;
    uint64_t recovery_successful_mask = (1ULL << 1) - 1ULL;
    uint64_t recovery_successful_raw = (data >> 7) & recovery_successful_mask;
    segment_isospi_communication_status->recovery_successful = (uint8_t)recovery_successful_raw;
}

void receive_fault_status(const can_msg_t *message, fault_status_t *fault_status) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t dcl_enforce_mask = (1ULL << 1) - 1ULL;
    uint64_t dcl_enforce_raw = (data >> 15) & dcl_enforce_mask;
    fault_status->dcl_enforce = (bool)dcl_enforce_raw;
    uint64_t ccl_enforce_mask = (1ULL << 1) - 1ULL;
    uint64_t ccl_enforce_raw = (data >> 14) & ccl_enforce_mask;
    fault_status->ccl_enforce = (bool)ccl_enforce_raw;
    uint64_t low_cell_volt_mask = (1ULL << 1) - 1ULL;
    uint64_t low_cell_volt_raw = (data >> 13) & low_cell_volt_mask;
    fault_status->low_cell_volt = (bool)low_cell_volt_raw;
    uint64_t high_cell_volt_mask = (1ULL << 1) - 1ULL;
    uint64_t high_cell_volt_raw = (data >> 12) & high_cell_volt_mask;
    fault_status->high_cell_volt = (bool)high_cell_volt_raw;
    uint64_t high_charge_volt_mask = (1ULL << 1) - 1ULL;
    uint64_t high_charge_volt_raw = (data >> 11) & high_charge_volt_mask;
    fault_status->high_charge_volt = (bool)high_charge_volt_raw;
    uint64_t pack_hot_mask = (1ULL << 1) - 1ULL;
    uint64_t pack_hot_raw = (data >> 10) & pack_hot_mask;
    fault_status->pack_hot = (bool)pack_hot_raw;
    uint64_t die_temp_max_mask = (1ULL << 1) - 1ULL;
    uint64_t die_temp_max_raw = (data >> 9) & die_temp_max_mask;
    fault_status->die_temp_max = (bool)die_temp_max_raw;
    uint64_t segment_comms_mask = (1ULL << 1) - 1ULL;
    uint64_t segment_comms_raw = (data >> 8) & segment_comms_mask;
    fault_status->segment_comms = (bool)segment_comms_raw;
    uint64_t hv_plate_comms_mask = (1ULL << 1) - 1ULL;
    uint64_t hv_plate_comms_raw = (data >> 7) & hv_plate_comms_mask;
    fault_status->hv_plate_comms = (bool)hv_plate_comms_raw;
}

void receive_segment_average_voltages(const can_msg_t *message, segment_average_voltages_t *segment_average_voltages) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t seg1_mask = (1ULL << 12) - 1ULL;
    uint64_t seg1_raw = (data >> 52) & seg1_mask;
    segment_average_voltages->seg1 = (float)(seg1_raw / 1000);
    uint64_t seg2_mask = (1ULL << 12) - 1ULL;
    uint64_t seg2_raw = (data >> 40) & seg2_mask;
    segment_average_voltages->seg2 = (float)(seg2_raw / 1000);
    uint64_t seg3_mask = (1ULL << 12) - 1ULL;
    uint64_t seg3_raw = (data >> 28) & seg3_mask;
    segment_average_voltages->seg3 = (float)(seg3_raw / 1000);
    uint64_t seg4_mask = (1ULL << 12) - 1ULL;
    uint64_t seg4_raw = (data >> 16) & seg4_mask;
    segment_average_voltages->seg4 = (float)(seg4_raw / 1000);
    uint64_t seg5_mask = (1ULL << 12) - 1ULL;
    uint64_t seg5_raw = (data >> 4) & seg5_mask;
    segment_average_voltages->seg5 = (float)(seg5_raw / 1000);
}

void receive_segment_total_voltages(const can_msg_t *message, segment_total_voltages_t *segment_total_voltages) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t seg1_mask = (1ULL << 12) - 1ULL;
    uint64_t seg1_raw = (data >> 52) & seg1_mask;
    segment_total_voltages->seg1 = (float)(seg1_raw / 39);
    uint64_t seg2_mask = (1ULL << 12) - 1ULL;
    uint64_t seg2_raw = (data >> 40) & seg2_mask;
    segment_total_voltages->seg2 = (float)(seg2_raw / 39);
    uint64_t seg3_mask = (1ULL << 12) - 1ULL;
    uint64_t seg3_raw = (data >> 28) & seg3_mask;
    segment_total_voltages->seg3 = (float)(seg3_raw / 39);
    uint64_t seg4_mask = (1ULL << 12) - 1ULL;
    uint64_t seg4_raw = (data >> 16) & seg4_mask;
    segment_total_voltages->seg4 = (float)(seg4_raw / 39);
    uint64_t seg5_mask = (1ULL << 12) - 1ULL;
    uint64_t seg5_raw = (data >> 4) & seg5_mask;
    segment_total_voltages->seg5 = (float)(seg5_raw / 39);
}

void receive_segment_delta_voltages(const can_msg_t *message, segment_delta_voltages_t *segment_delta_voltages) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t seg1_mask = (1ULL << 12) - 1ULL;
    uint64_t seg1_raw = (data >> 52) & seg1_mask;
    segment_delta_voltages->seg1 = (float)(seg1_raw / 1000);
    uint64_t seg2_mask = (1ULL << 12) - 1ULL;
    uint64_t seg2_raw = (data >> 40) & seg2_mask;
    segment_delta_voltages->seg2 = (float)(seg2_raw / 1000);
    uint64_t seg3_mask = (1ULL << 12) - 1ULL;
    uint64_t seg3_raw = (data >> 28) & seg3_mask;
    segment_delta_voltages->seg3 = (float)(seg3_raw / 1000);
    uint64_t seg4_mask = (1ULL << 12) - 1ULL;
    uint64_t seg4_raw = (data >> 16) & seg4_mask;
    segment_delta_voltages->seg4 = (float)(seg4_raw / 1000);
    uint64_t seg5_mask = (1ULL << 12) - 1ULL;
    uint64_t seg5_raw = (data >> 4) & seg5_mask;
    segment_delta_voltages->seg5 = (float)(seg5_raw / 1000);
}

void receive_bms_debug(const can_msg_t *message, bms_debug_t *bms_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t spare0_mask = (1ULL << 8) - 1ULL;
    uint64_t spare0_raw = (data >> 56) & spare0_mask;
    bms_debug->spare0 = (uint8_t)spare0_raw;
    uint64_t spare1_mask = (1ULL << 8) - 1ULL;
    uint64_t spare1_raw = (data >> 48) & spare1_mask;
    bms_debug->spare1 = (uint8_t)spare1_raw;
    uint64_t spare2_mask = (1ULL << 16) - 1ULL;
    uint64_t spare2_raw = (data >> 32) & spare2_mask;
    bms_debug->spare2 = (uint16_t)spare2_raw;
    uint64_t spare3_mask = (1ULL << 32) - 1ULL;
    uint64_t spare3_raw = (data >> 0) & spare3_mask;
    bms_debug->spare3 = (uint32_t)spare3_raw;
}

void receive_bms_fault_timers(const can_msg_t *message, bms_fault_timers_t *bms_fault_timers) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t start_stop_mask = (1ULL << 8) - 1ULL;
    uint64_t start_stop_raw = (data >> 56) & start_stop_mask;
    bms_fault_timers->start_stop = (uint8_t)start_stop_raw;
    uint64_t code_mask = (1ULL << 8) - 1ULL;
    uint64_t code_raw = (data >> 48) & code_mask;
    bms_fault_timers->code = (uint8_t)code_raw;
    uint64_t value_mask = (1ULL << 32) - 1ULL;
    uint64_t value_raw = (data >> 16) & value_mask;
    bms_fault_timers->value = (float)value_raw;
}

void receive_shepherd_version_tag(const can_msg_t *message, shepherd_version_tag_t *shepherd_version_tag) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t major_mask = (1ULL << 8) - 1ULL;
    uint64_t major_raw = (data >> 56) & major_mask;
    shepherd_version_tag->major = (uint8_t)major_raw;
    uint64_t minor_mask = (1ULL << 8) - 1ULL;
    uint64_t minor_raw = (data >> 48) & minor_mask;
    shepherd_version_tag->minor = (uint8_t)minor_raw;
    uint64_t patch_mask = (1ULL << 8) - 1ULL;
    uint64_t patch_raw = (data >> 40) & patch_mask;
    shepherd_version_tag->patch = (uint8_t)patch_raw;
    uint64_t dirty_mask = (1ULL << 8) - 1ULL;
    uint64_t dirty_raw = (data >> 32) & dirty_mask;
    shepherd_version_tag->dirty = (bool)dirty_raw;
    uint64_t local_commit_mask = (1ULL << 8) - 1ULL;
    uint64_t local_commit_raw = (data >> 24) & local_commit_mask;
    shepherd_version_tag->local_commit = (bool)local_commit_raw;
}

void receive_shepherd_version_hash(const can_msg_t *message, shepherd_version_hash_t *shepherd_version_hash) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t short_hash_mask = (1ULL << 32) - 1ULL;
    uint64_t short_hash_raw = (data >> 32) & short_hash_mask;
    shepherd_version_hash->short_hash = (uint32_t)short_hash_raw;
    uint64_t author_hash_mask = (1ULL << 32) - 1ULL;
    uint64_t author_hash_raw = (data >> 0) & author_hash_mask;
    shepherd_version_hash->author_hash = (uint32_t)author_hash_raw;
}

void receive_overflow_notification_for_percell(const can_msg_t *message, overflow_notification_for_percell_t *overflow_notification_for_percell) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t overflow_can_id_mask = (1ULL << 32) - 1ULL;
    uint64_t overflow_can_id_raw = (data >> 32) & overflow_can_id_mask;
    overflow_notification_for_percell->overflow_can_id = (uint32_t)overflow_can_id_raw;
    uint64_t overflow_cnt_mask = (1ULL << 16) - 1ULL;
    uint64_t overflow_cnt_raw = (data >> 16) & overflow_cnt_mask;
    overflow_notification_for_percell->overflow_cnt = (uint16_t)overflow_cnt_raw;
}

void receive_alpha_cell_data_debug(const can_msg_t *message, alpha_cell_data_debug_t *alpha_cell_data_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t therm_mask = (1ULL << 10) - 1ULL;
    uint64_t therm_raw = (data >> 54) & therm_mask;
    alpha_cell_data_debug->therm = (float)(therm_raw / 10);
    uint64_t voltage_a_mask = (1ULL << 13) - 1ULL;
    uint64_t voltage_a_raw = (data >> 41) & voltage_a_mask;
    alpha_cell_data_debug->voltage_a = (float)(voltage_a_raw / 1000);
    uint64_t voltage_b_mask = (1ULL << 13) - 1ULL;
    uint64_t voltage_b_raw = (data >> 28) & voltage_b_mask;
    alpha_cell_data_debug->voltage_b = (float)(voltage_b_raw / 1000);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 24) & chip_id_mask;
    alpha_cell_data_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t cell_a_mask = (1ULL << 4) - 1ULL;
    uint64_t cell_a_raw = (data >> 20) & cell_a_mask;
    alpha_cell_data_debug->cell_a = (uint8_t)cell_a_raw;
    uint64_t cell_b_mask = (1ULL << 4) - 1ULL;
    uint64_t cell_b_raw = (data >> 16) & cell_b_mask;
    alpha_cell_data_debug->cell_b = (uint8_t)cell_b_raw;
    uint64_t discharging_a_mask = (1ULL << 1) - 1ULL;
    uint64_t discharging_a_raw = (data >> 15) & discharging_a_mask;
    alpha_cell_data_debug->discharging_a = (bool)discharging_a_raw;
    uint64_t discharging_b_mask = (1ULL << 1) - 1ULL;
    uint64_t discharging_b_raw = (data >> 14) & discharging_b_mask;
    alpha_cell_data_debug->discharging_b = (bool)discharging_b_raw;
    uint64_t cvs_a_mask = (1ULL << 1) - 1ULL;
    uint64_t cvs_a_raw = (data >> 13) & cvs_a_mask;
    alpha_cell_data_debug->cvs_a = (bool)cvs_a_raw;
    uint64_t cvs_b_mask = (1ULL << 1) - 1ULL;
    uint64_t cvs_b_raw = (data >> 12) & cvs_b_mask;
    alpha_cell_data_debug->cvs_b = (bool)cvs_b_raw;
}

void receive_beta_cell_data_debug(const can_msg_t *message, beta_cell_data_debug_t *beta_cell_data_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t therm_mask = (1ULL << 10) - 1ULL;
    uint64_t therm_raw = (data >> 54) & therm_mask;
    beta_cell_data_debug->therm = (float)(therm_raw / 10);
    uint64_t voltage_a_mask = (1ULL << 13) - 1ULL;
    uint64_t voltage_a_raw = (data >> 41) & voltage_a_mask;
    beta_cell_data_debug->voltage_a = (float)(voltage_a_raw / 1000);
    uint64_t voltage_b_mask = (1ULL << 13) - 1ULL;
    uint64_t voltage_b_raw = (data >> 28) & voltage_b_mask;
    beta_cell_data_debug->voltage_b = (float)(voltage_b_raw / 1000);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 24) & chip_id_mask;
    beta_cell_data_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t cell_a_mask = (1ULL << 4) - 1ULL;
    uint64_t cell_a_raw = (data >> 20) & cell_a_mask;
    beta_cell_data_debug->cell_a = (uint8_t)cell_a_raw;
    uint64_t cell_b_mask = (1ULL << 4) - 1ULL;
    uint64_t cell_b_raw = (data >> 16) & cell_b_mask;
    beta_cell_data_debug->cell_b = (uint8_t)cell_b_raw;
    uint64_t discharging_a_mask = (1ULL << 1) - 1ULL;
    uint64_t discharging_a_raw = (data >> 15) & discharging_a_mask;
    beta_cell_data_debug->discharging_a = (bool)discharging_a_raw;
    uint64_t discharging_b_mask = (1ULL << 1) - 1ULL;
    uint64_t discharging_b_raw = (data >> 14) & discharging_b_mask;
    beta_cell_data_debug->discharging_b = (bool)discharging_b_raw;
    uint64_t cvs_a_mask = (1ULL << 1) - 1ULL;
    uint64_t cvs_a_raw = (data >> 13) & cvs_a_mask;
    beta_cell_data_debug->cvs_a = (bool)cvs_a_raw;
    uint64_t cvs_b_mask = (1ULL << 1) - 1ULL;
    uint64_t cvs_b_raw = (data >> 12) & cvs_b_mask;
    beta_cell_data_debug->cvs_b = (bool)cvs_b_raw;
}

void receive_alpha_chip_a_debug(const can_msg_t *message, alpha_chip_a_debug_t *alpha_chip_a_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 60) & chip_id_mask;
    alpha_chip_a_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t die_temp_mask = (1ULL << 13) - 1ULL;
    uint64_t die_temp_raw = (data >> 47) & die_temp_mask;
    alpha_chip_a_debug->die_temp = (float)(die_temp_raw / 100);
    uint64_t vpv_mask = (1ULL << 13) - 1ULL;
    uint64_t vpv_raw = (data >> 34) & vpv_mask;
    alpha_chip_a_debug->vpv = (float)(vpv_raw / 100);
    uint64_t vmv_mask = (1ULL << 13) - 1ULL;
    uint64_t vmv_raw = (data >> 21) & vmv_mask;
    alpha_chip_a_debug->vmv = (float)(vmv_raw / 1000);
    uint64_t va_ov_mask = (1ULL << 1) - 1ULL;
    uint64_t va_ov_raw = (data >> 20) & va_ov_mask;
    alpha_chip_a_debug->va_ov = (bool)va_ov_raw;
    uint64_t va_uv_mask = (1ULL << 1) - 1ULL;
    uint64_t va_uv_raw = (data >> 19) & va_uv_mask;
    alpha_chip_a_debug->va_uv = (bool)va_uv_raw;
    uint64_t vd_ov_mask = (1ULL << 1) - 1ULL;
    uint64_t vd_ov_raw = (data >> 18) & vd_ov_mask;
    alpha_chip_a_debug->vd_ov = (bool)vd_ov_raw;
    uint64_t vd_uv_mask = (1ULL << 1) - 1ULL;
    uint64_t vd_uv_raw = (data >> 17) & vd_uv_mask;
    alpha_chip_a_debug->vd_uv = (bool)vd_uv_raw;
    uint64_t vde_mask = (1ULL << 1) - 1ULL;
    uint64_t vde_raw = (data >> 16) & vde_mask;
    alpha_chip_a_debug->vde = (bool)vde_raw;
    uint64_t vdel_mask = (1ULL << 1) - 1ULL;
    uint64_t vdel_raw = (data >> 15) & vdel_mask;
    alpha_chip_a_debug->vdel = (bool)vdel_raw;
    uint64_t spiflt_mask = (1ULL << 1) - 1ULL;
    uint64_t spiflt_raw = (data >> 14) & spiflt_mask;
    alpha_chip_a_debug->spiflt = (bool)spiflt_raw;
    uint64_t sleep_mask = (1ULL << 1) - 1ULL;
    uint64_t sleep_raw = (data >> 13) & sleep_mask;
    alpha_chip_a_debug->sleep = (bool)sleep_raw;
    uint64_t thsd_mask = (1ULL << 1) - 1ULL;
    uint64_t thsd_raw = (data >> 12) & thsd_mask;
    alpha_chip_a_debug->thsd = (bool)thsd_raw;
    uint64_t tmodchk_mask = (1ULL << 1) - 1ULL;
    uint64_t tmodchk_raw = (data >> 11) & tmodchk_mask;
    alpha_chip_a_debug->tmodchk = (bool)tmodchk_raw;
    uint64_t oscchk_mask = (1ULL << 1) - 1ULL;
    uint64_t oscchk_raw = (data >> 10) & oscchk_mask;
    alpha_chip_a_debug->oscchk = (bool)oscchk_raw;
}

void receive_alpha_chip_b_debug(const can_msg_t *message, alpha_chip_b_debug_t *alpha_chip_b_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t vres_mask = (1ULL << 13) - 1ULL;
    uint64_t vres_raw = (data >> 51) & vres_mask;
    alpha_chip_b_debug->vres = (float)(vres_raw / 1000);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 47) & chip_id_mask;
    alpha_chip_b_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t vref2_mask = (1ULL << 13) - 1ULL;
    uint64_t vref2_raw = (data >> 34) & vref2_mask;
    alpha_chip_b_debug->vref2 = (float)(vref2_raw / 1000);
    uint64_t v_analog_mask = (1ULL << 13) - 1ULL;
    uint64_t v_analog_raw = (data >> 21) & v_analog_mask;
    alpha_chip_b_debug->v_analog = (float)(v_analog_raw / 1000);
    uint64_t v_digital_mask = (1ULL << 13) - 1ULL;
    uint64_t v_digital_raw = (data >> 8) & v_digital_mask;
    alpha_chip_b_debug->v_digital = (float)(v_digital_raw / 1000);
    uint64_t otp1_med_mask = (1ULL << 1) - 1ULL;
    uint64_t otp1_med_raw = (data >> 7) & otp1_med_mask;
    alpha_chip_b_debug->otp1_med = (bool)otp1_med_raw;
    uint64_t opt2_med_mask = (1ULL << 1) - 1ULL;
    uint64_t opt2_med_raw = (data >> 6) & opt2_med_mask;
    alpha_chip_b_debug->opt2_med = (bool)opt2_med_raw;
}

void receive_beta_chip_a_debug(const can_msg_t *message, beta_chip_a_debug_t *beta_chip_a_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 60) & chip_id_mask;
    beta_chip_a_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t die_temp_mask = (1ULL << 13) - 1ULL;
    uint64_t die_temp_raw = (data >> 47) & die_temp_mask;
    beta_chip_a_debug->die_temp = (float)(die_temp_raw / 100);
    uint64_t vpv_mask = (1ULL << 13) - 1ULL;
    uint64_t vpv_raw = (data >> 34) & vpv_mask;
    beta_chip_a_debug->vpv = (float)(vpv_raw / 100);
    uint64_t vmv_mask = (1ULL << 13) - 1ULL;
    uint64_t vmv_raw = (data >> 21) & vmv_mask;
    beta_chip_a_debug->vmv = (float)(vmv_raw / 1000);
    uint64_t va_ov_mask = (1ULL << 1) - 1ULL;
    uint64_t va_ov_raw = (data >> 20) & va_ov_mask;
    beta_chip_a_debug->va_ov = (bool)va_ov_raw;
    uint64_t va_uv_mask = (1ULL << 1) - 1ULL;
    uint64_t va_uv_raw = (data >> 19) & va_uv_mask;
    beta_chip_a_debug->va_uv = (bool)va_uv_raw;
    uint64_t vd_ov_mask = (1ULL << 1) - 1ULL;
    uint64_t vd_ov_raw = (data >> 18) & vd_ov_mask;
    beta_chip_a_debug->vd_ov = (bool)vd_ov_raw;
    uint64_t vd_uv_mask = (1ULL << 1) - 1ULL;
    uint64_t vd_uv_raw = (data >> 17) & vd_uv_mask;
    beta_chip_a_debug->vd_uv = (bool)vd_uv_raw;
    uint64_t vde_mask = (1ULL << 1) - 1ULL;
    uint64_t vde_raw = (data >> 16) & vde_mask;
    beta_chip_a_debug->vde = (bool)vde_raw;
    uint64_t vdel_mask = (1ULL << 1) - 1ULL;
    uint64_t vdel_raw = (data >> 15) & vdel_mask;
    beta_chip_a_debug->vdel = (bool)vdel_raw;
    uint64_t spiflt_mask = (1ULL << 1) - 1ULL;
    uint64_t spiflt_raw = (data >> 14) & spiflt_mask;
    beta_chip_a_debug->spiflt = (bool)spiflt_raw;
    uint64_t sleep_mask = (1ULL << 1) - 1ULL;
    uint64_t sleep_raw = (data >> 13) & sleep_mask;
    beta_chip_a_debug->sleep = (bool)sleep_raw;
    uint64_t thsd_mask = (1ULL << 1) - 1ULL;
    uint64_t thsd_raw = (data >> 12) & thsd_mask;
    beta_chip_a_debug->thsd = (bool)thsd_raw;
    uint64_t tmodchk_mask = (1ULL << 1) - 1ULL;
    uint64_t tmodchk_raw = (data >> 11) & tmodchk_mask;
    beta_chip_a_debug->tmodchk = (bool)tmodchk_raw;
    uint64_t oscchk_mask = (1ULL << 1) - 1ULL;
    uint64_t oscchk_raw = (data >> 10) & oscchk_mask;
    beta_chip_a_debug->oscchk = (bool)oscchk_raw;
}

void receive_beta_chip_b_debug(const can_msg_t *message, beta_chip_b_debug_t *beta_chip_b_debug) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t vres_mask = (1ULL << 13) - 1ULL;
    uint64_t vres_raw = (data >> 51) & vres_mask;
    beta_chip_b_debug->vres = (float)(vres_raw / 1000);
    uint64_t chip_id_mask = (1ULL << 4) - 1ULL;
    uint64_t chip_id_raw = (data >> 47) & chip_id_mask;
    beta_chip_b_debug->chip_id = (uint8_t)chip_id_raw;
    uint64_t vref2_mask = (1ULL << 13) - 1ULL;
    uint64_t vref2_raw = (data >> 34) & vref2_mask;
    beta_chip_b_debug->vref2 = (float)(vref2_raw / 1000);
    uint64_t v_analog_mask = (1ULL << 13) - 1ULL;
    uint64_t v_analog_raw = (data >> 21) & v_analog_mask;
    beta_chip_b_debug->v_analog = (float)(v_analog_raw / 1000);
    uint64_t v_digital_mask = (1ULL << 13) - 1ULL;
    uint64_t v_digital_raw = (data >> 8) & v_digital_mask;
    beta_chip_b_debug->v_digital = (float)(v_digital_raw / 1000);
    uint64_t otp1_med_mask = (1ULL << 1) - 1ULL;
    uint64_t otp1_med_raw = (data >> 7) & otp1_med_mask;
    beta_chip_b_debug->otp1_med = (bool)otp1_med_raw;
    uint64_t opt2_med_mask = (1ULL << 1) - 1ULL;
    uint64_t opt2_med_raw = (data >> 6) & opt2_med_mask;
    beta_chip_b_debug->opt2_med = (bool)opt2_med_raw;
}

void receive_fan_duty_cycle_percentage(const can_msg_t *message, fan_duty_cycle_percentage_t *fan_duty_cycle_percentage) {
    
    uint8_t data = message->data[0];
    uint64_t fan_duty_cycle_mask = (1ULL << 8) - 1ULL;
    uint64_t fan_duty_cycle_raw = (data >> 0) & fan_duty_cycle_mask;
    fan_duty_cycle_percentage->fan_duty_cycle = (uint8_t)fan_duty_cycle_raw;
}

void receive_onboard_therm_temperatures(const can_msg_t *message, onboard_therm_temperatures_t *onboard_therm_temperatures) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t chip_id_mask = (1ULL << 8) - 1ULL;
    uint64_t chip_id_raw = (data >> 56) & chip_id_mask;
    onboard_therm_temperatures->chip_id = (uint8_t)chip_id_raw;
    uint64_t therm_temp_1_mask = (1ULL << 16) - 1ULL;
    uint64_t therm_temp_1_bits = (data >> 40) & therm_temp_1_mask;
    int64_t therm_temp_1_raw = (therm_temp_1_bits & (1ULL << (16 - 1)))
        ? (int64_t)(therm_temp_1_bits | ~therm_temp_1_mask)
        : (int64_t)therm_temp_1_bits;
    onboard_therm_temperatures->therm_temp_1 = (float)(therm_temp_1_raw / 100);
    uint64_t therm_temp_2_mask = (1ULL << 16) - 1ULL;
    uint64_t therm_temp_2_bits = (data >> 24) & therm_temp_2_mask;
    int64_t therm_temp_2_raw = (therm_temp_2_bits & (1ULL << (16 - 1)))
        ? (int64_t)(therm_temp_2_bits | ~therm_temp_2_mask)
        : (int64_t)therm_temp_2_bits;
    onboard_therm_temperatures->therm_temp_2 = (float)(therm_temp_2_raw / 100);
    uint64_t therm_temp_3_mask = (1ULL << 16) - 1ULL;
    uint64_t therm_temp_3_bits = (data >> 8) & therm_temp_3_mask;
    int64_t therm_temp_3_raw = (therm_temp_3_bits & (1ULL << (16 - 1)))
        ? (int64_t)(therm_temp_3_bits | ~therm_temp_3_mask)
        : (int64_t)therm_temp_3_bits;
    onboard_therm_temperatures->therm_temp_3 = (float)(therm_temp_3_raw / 100);
}

void receive_precharge_status(const can_msg_t *message, precharge_status_t *precharge_status) {
    
    uint8_t data = message->data[0];
    uint64_t precharge_status_mask = (1ULL << 8) - 1ULL;
    uint64_t precharge_status_raw = (data >> 0) & precharge_status_mask;
    precharge_status->precharge_status = (uint8_t)precharge_status_raw;
}

void receive_hv_plate_voltages(const can_msg_t *message, hv_plate_voltages_t *hv_plate_voltages) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t batt_voltage_mask = (1ULL << 32) - 1ULL;
    uint64_t batt_voltage_bits = (data >> 32) & batt_voltage_mask;
    int64_t batt_voltage_raw = (batt_voltage_bits & (1ULL << (32 - 1)))
        ? (int64_t)(batt_voltage_bits | ~batt_voltage_mask)
        : (int64_t)batt_voltage_bits;
    hv_plate_voltages->batt_voltage = (float)(batt_voltage_raw / 100);
    uint64_t ts_voltage_mask = (1ULL << 32) - 1ULL;
    uint64_t ts_voltage_bits = (data >> 0) & ts_voltage_mask;
    int64_t ts_voltage_raw = (ts_voltage_bits & (1ULL << (32 - 1)))
        ? (int64_t)(ts_voltage_bits | ~ts_voltage_mask)
        : (int64_t)ts_voltage_bits;
    hv_plate_voltages->ts_voltage = (float)(ts_voltage_raw / 100);
}

void receive_segment_pec_errors(const can_msg_t *message, segment_pec_errors_t *segment_pec_errors) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t chip_id_mask = (1ULL << 8) - 1ULL;
    uint64_t chip_id_raw = (data >> 24) & chip_id_mask;
    segment_pec_errors->chip_id = (uint8_t)chip_id_raw;
    uint64_t pec_errors_mask = (1ULL << 16) - 1ULL;
    uint64_t pec_errors_raw = (data >> 8) & pec_errors_mask;
    segment_pec_errors->pec_errors = (uint16_t)pec_errors_raw;
}

void receive_hv_plate_pec_errors(const can_msg_t *message, hv_plate_pec_errors_t *hv_plate_pec_errors) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t pec_errors_mask = (1ULL << 16) - 1ULL;
    uint64_t pec_errors_raw = (data >> 0) & pec_errors_mask;
    hv_plate_pec_errors->pec_errors = (uint16_t)pec_errors_raw;
}

void receive_hv_plate_diagnostics(const can_msg_t *message, hv_plate_diagnostics_t *hv_plate_diagnostics) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t flags_mask = (1ULL << 12) - 1ULL;
    uint64_t flags_raw = (data >> 52) & flags_mask;
    hv_plate_diagnostics->flags = (uint16_t)flags_raw;
    uint64_t vreg_mask = (1ULL << 12) - 1ULL;
    uint64_t vreg_raw = (data >> 40) & vreg_mask;
    hv_plate_diagnostics->vreg = (float)(vreg_raw / 100);
    uint64_t tmp1_mask = (1ULL << 12) - 1ULL;
    uint64_t tmp1_raw = (data >> 28) & tmp1_mask;
    hv_plate_diagnostics->tmp1 = (float)(tmp1_raw / 100);
    uint64_t vref1p25_mask = (1ULL << 12) - 1ULL;
    uint64_t vref1p25_raw = (data >> 16) & vref1p25_mask;
    hv_plate_diagnostics->vref1p25 = (float)(vref1p25_raw / 100);
    uint64_t osccnt_mask = (1ULL << 16) - 1ULL;
    uint64_t osccnt_raw = (data >> 0) & osccnt_mask;
    hv_plate_diagnostics->osccnt = (uint16_t)osccnt_raw;
}

void receive_hv_plate_diagnostics_second(const can_msg_t *message, hv_plate_diagnostics_second_t *hv_plate_diagnostics_second) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t epad_mask = (1ULL << 12) - 1ULL;
    uint64_t epad_raw = (data >> 52) & epad_mask;
    hv_plate_diagnostics_second->epad = (float)(epad_raw / 100);
    uint64_t vdig_mask = (1ULL << 12) - 1ULL;
    uint64_t vdig_raw = (data >> 40) & vdig_mask;
    hv_plate_diagnostics_second->vdig = (float)(vdig_raw / 100);
    uint64_t vdd_mask = (1ULL << 12) - 1ULL;
    uint64_t vdd_raw = (data >> 28) & vdd_mask;
    hv_plate_diagnostics_second->vdd = (float)(vdd_raw / 100);
    uint64_t tmp2_mask = (1ULL << 12) - 1ULL;
    uint64_t tmp2_raw = (data >> 16) & tmp2_mask;
    hv_plate_diagnostics_second->tmp2 = (float)(tmp2_raw / 100);
    uint64_t vdiv_mask = (1ULL << 12) - 1ULL;
    uint64_t vdiv_raw = (data >> 4) & vdiv_mask;
    hv_plate_diagnostics_second->vdiv = (float)(vdiv_raw / 100);
}

void receive_bms_onboard_temperature(const can_msg_t *message, bms_onboard_temperature_t *bms_onboard_temperature) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t internal_temp_mask = (1ULL << 16) - 1ULL;
    uint64_t internal_temp_raw = (data >> 0) & internal_temp_mask;
    bms_onboard_temperature->internal_temp = (float)(internal_temp_raw / 100);
}

void receive_bms_imu_accelerometer(const can_msg_t *message, bms_imu_accelerometer_t *bms_imu_accelerometer) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t imu_accelerometer_x_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_x_bits = (data >> 48) & imu_accelerometer_x_mask;
    int64_t imu_accelerometer_x_raw = (imu_accelerometer_x_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_x_bits | ~imu_accelerometer_x_mask)
        : (int64_t)imu_accelerometer_x_bits;
    bms_imu_accelerometer->imu_accelerometer_x = (float)(imu_accelerometer_x_raw / 4);
    uint64_t imu_accelerometer_y_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_y_bits = (data >> 32) & imu_accelerometer_y_mask;
    int64_t imu_accelerometer_y_raw = (imu_accelerometer_y_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_y_bits | ~imu_accelerometer_y_mask)
        : (int64_t)imu_accelerometer_y_bits;
    bms_imu_accelerometer->imu_accelerometer_y = (float)(imu_accelerometer_y_raw / 4);
    uint64_t imu_accelerometer_z_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_accelerometer_z_bits = (data >> 16) & imu_accelerometer_z_mask;
    int64_t imu_accelerometer_z_raw = (imu_accelerometer_z_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_accelerometer_z_bits | ~imu_accelerometer_z_mask)
        : (int64_t)imu_accelerometer_z_bits;
    bms_imu_accelerometer->imu_accelerometer_z = (float)(imu_accelerometer_z_raw / 4);
}

void receive_bms_imu_gyro(const can_msg_t *message, bms_imu_gyro_t *bms_imu_gyro) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t imu_gyro_x_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_x_bits = (data >> 48) & imu_gyro_x_mask;
    int64_t imu_gyro_x_raw = (imu_gyro_x_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_x_bits | ~imu_gyro_x_mask)
        : (int64_t)imu_gyro_x_bits;
    bms_imu_gyro->imu_gyro_x = (float)(imu_gyro_x_raw / 100);
    uint64_t imu_gyro_y_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_y_bits = (data >> 32) & imu_gyro_y_mask;
    int64_t imu_gyro_y_raw = (imu_gyro_y_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_y_bits | ~imu_gyro_y_mask)
        : (int64_t)imu_gyro_y_bits;
    bms_imu_gyro->imu_gyro_y = (float)(imu_gyro_y_raw / 100);
    uint64_t imu_gyro_z_mask = (1ULL << 16) - 1ULL;
    uint64_t imu_gyro_z_bits = (data >> 16) & imu_gyro_z_mask;
    int64_t imu_gyro_z_raw = (imu_gyro_z_bits & (1ULL << (16 - 1)))
        ? (int64_t)(imu_gyro_z_bits | ~imu_gyro_z_mask)
        : (int64_t)imu_gyro_z_bits;
    bms_imu_gyro->imu_gyro_z = (float)(imu_gyro_z_raw / 100);
}

void receive_pack_soc_status(const can_msg_t *message, pack_soc_status_t *pack_soc_status) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t Pack_SoC_mask = (1ULL << 16) - 1ULL;
    uint64_t Pack_SoC_raw = (data >> 16) & Pack_SoC_mask;
    pack_soc_status->Pack_SoC = (float)(Pack_SoC_raw / 1000);
    uint64_t Pack_SoC_Drift_mask = (1ULL << 16) - 1ULL;
    uint64_t Pack_SoC_Drift_bits = (data >> 0) & Pack_SoC_Drift_mask;
    int64_t Pack_SoC_Drift_raw = (Pack_SoC_Drift_bits & (1ULL << (16 - 1)))
        ? (int64_t)(Pack_SoC_Drift_bits | ~Pack_SoC_Drift_mask)
        : (int64_t)Pack_SoC_Drift_bits;
    pack_soc_status->Pack_SoC_Drift = (float)(Pack_SoC_Drift_raw / 1000);
}

void receive_shutdown_as_read_by_bms(const can_msg_t *message, shutdown_as_read_by_bms_t *shutdown_as_read_by_bms) {
    
    uint8_t data = message->data[0];
    uint64_t shutdown_state_mask = (1ULL << 1) - 1ULL;
    uint64_t shutdown_state_raw = (data >> 7) & shutdown_state_mask;
    shutdown_as_read_by_bms->shutdown_state = (bool)shutdown_state_raw;
    uint64_t shutdown_ts_minus_sense_mask = (1ULL << 1) - 1ULL;
    uint64_t shutdown_ts_minus_sense_raw = (data >> 6) & shutdown_ts_minus_sense_mask;
    shutdown_as_read_by_bms->shutdown_ts_minus_sense = (bool)shutdown_ts_minus_sense_raw;
    uint64_t shutdown_ts_plus_sense_mask = (1ULL << 1) - 1ULL;
    uint64_t shutdown_ts_plus_sense_raw = (data >> 5) & shutdown_ts_plus_sense_mask;
    shutdown_as_read_by_bms->shutdown_ts_plus_sense = (bool)shutdown_ts_plus_sense_raw;
    uint64_t shutdown_acc_sense_mask = (1ULL << 1) - 1ULL;
    uint64_t shutdown_acc_sense_raw = (data >> 4) & shutdown_acc_sense_mask;
    shutdown_as_read_by_bms->shutdown_acc_sense = (bool)shutdown_acc_sense_raw;
    uint64_t shutdown_tsip_sense_mask = (1ULL << 1) - 1ULL;
    uint64_t shutdown_tsip_sense_raw = (data >> 3) & shutdown_tsip_sense_mask;
    shutdown_as_read_by_bms->shutdown_tsip_sense = (bool)shutdown_tsip_sense_raw;
}

void receive_hv_plate_isospi_communication_status(const can_msg_t *message, hv_plate_isospi_communication_status_t *hv_plate_isospi_communication_status) {
    
    uint32_t data_bigendian;
    memcpy(&data_bigendian, message->data, 4);
    uint32_t data = __builtin_bswap32(data_bigendian);
    uint64_t state_mask = (1ULL << 8) - 1ULL;
    uint64_t state_raw = (data >> 24) & state_mask;
    hv_plate_isospi_communication_status->state = (uint8_t)state_raw;
    uint64_t verification_attempts_mask = (1ULL << 8) - 1ULL;
    uint64_t verification_attempts_raw = (data >> 16) & verification_attempts_mask;
    hv_plate_isospi_communication_status->verification_attempts = (uint8_t)verification_attempts_raw;
    uint64_t recovery_successful_mask = (1ULL << 1) - 1ULL;
    uint64_t recovery_successful_raw = (data >> 15) & recovery_successful_mask;
    hv_plate_isospi_communication_status->recovery_successful = (uint8_t)recovery_successful_raw;
}

void receive_bms_critically_faulted(const can_msg_t *message, bms_critically_faulted_t *bms_critically_faulted) {
    
    uint8_t data = message->data[0];
    uint64_t critically_faulted_mask = (1ULL << 1) - 1ULL;
    uint64_t critically_faulted_raw = (data >> 7) & critically_faulted_mask;
    bms_critically_faulted->critically_faulted = (bool)critically_faulted_raw;
}

void receive_pack_current_and_shunt_temp(const can_msg_t *message, pack_current_and_shunt_temp_t *pack_current_and_shunt_temp) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t pack_current_mask = (1ULL << 32) - 1ULL;
    uint64_t pack_current_bits = (data >> 32) & pack_current_mask;
    int64_t pack_current_raw = (pack_current_bits & (1ULL << (32 - 1)))
        ? (int64_t)(pack_current_bits | ~pack_current_mask)
        : (int64_t)pack_current_bits;
    pack_current_and_shunt_temp->pack_current = (float)(pack_current_raw / 100);
    uint64_t shunt_temp_mask = (1ULL << 32) - 1ULL;
    uint64_t shunt_temp_raw = (data >> 0) & shunt_temp_mask;
    pack_current_and_shunt_temp->shunt_temp = (float)(shunt_temp_raw / 100);
}

void receive_hv_plate_voltages_adbms(const can_msg_t *message, hv_plate_voltages_adbms_t *hv_plate_voltages_adbms) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t batt_volts_mask = (1ULL << 32) - 1ULL;
    uint64_t batt_volts_bits = (data >> 32) & batt_volts_mask;
    int64_t batt_volts_raw = (batt_volts_bits & (1ULL << (32 - 1)))
        ? (int64_t)(batt_volts_bits | ~batt_volts_mask)
        : (int64_t)batt_volts_bits;
    hv_plate_voltages_adbms->batt_volts = (float)(batt_volts_raw / 100);
    uint64_t ts_volts_mask = (1ULL << 32) - 1ULL;
    uint64_t ts_volts_bits = (data >> 0) & ts_volts_mask;
    int64_t ts_volts_raw = (ts_volts_bits & (1ULL << (32 - 1)))
        ? (int64_t)(ts_volts_bits | ~ts_volts_mask)
        : (int64_t)ts_volts_bits;
    hv_plate_voltages_adbms->ts_volts = (float)(ts_volts_raw / 100);
}

void receive_pack_current_and_shunt_temp_adbms(const can_msg_t *message, pack_current_and_shunt_temp_adbms_t *pack_current_and_shunt_temp_adbms) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t pack_current_mask = (1ULL << 32) - 1ULL;
    uint64_t pack_current_bits = (data >> 32) & pack_current_mask;
    int64_t pack_current_raw = (pack_current_bits & (1ULL << (32 - 1)))
        ? (int64_t)(pack_current_bits | ~pack_current_mask)
        : (int64_t)pack_current_bits;
    pack_current_and_shunt_temp_adbms->pack_current = (float)(pack_current_raw / 100);
    uint64_t shunt_temp_mask = (1ULL << 32) - 1ULL;
    uint64_t shunt_temp_bits = (data >> 0) & shunt_temp_mask;
    int64_t shunt_temp_raw = (shunt_temp_bits & (1ULL << (32 - 1)))
        ? (int64_t)(shunt_temp_bits | ~shunt_temp_mask)
        : (int64_t)shunt_temp_bits;
    pack_current_and_shunt_temp_adbms->shunt_temp = (float)(shunt_temp_raw / 100);
}

void receive_ac_current_command(const can_msg_t *message, ac_current_command_t *ac_current_command) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t current_target_ac_mask = (1ULL << 16) - 1ULL;
    uint64_t current_target_ac_bits = (data >> 0) & current_target_ac_mask;
    int64_t current_target_ac_raw = (current_target_ac_bits & (1ULL << (16 - 1)))
        ? (int64_t)(current_target_ac_bits | ~current_target_ac_mask)
        : (int64_t)current_target_ac_bits;
    ac_current_command->current_target_ac = (float)(current_target_ac_raw / 10);
}

void receive_brake_current_command(const can_msg_t *message, brake_current_command_t *brake_current_command) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t brake_ac_current_mask = (1ULL << 16) - 1ULL;
    uint64_t brake_ac_current_bits = (data >> 48) & brake_ac_current_mask;
    int64_t brake_ac_current_raw = (brake_ac_current_bits & (1ULL << (16 - 1)))
        ? (int64_t)(brake_ac_current_bits | ~brake_ac_current_mask)
        : (int64_t)brake_ac_current_bits;
    brake_current_command->brake_ac_current = (float)(brake_ac_current_raw / 10);
}

void receive_max_ac_current_command(const can_msg_t *message, max_ac_current_command_t *max_ac_current_command) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t max_current_ac_target_mask = (1ULL << 16) - 1ULL;
    uint64_t max_current_ac_target_bits = (data >> 48) & max_current_ac_target_mask;
    int64_t max_current_ac_target_raw = (max_current_ac_target_bits & (1ULL << (16 - 1)))
        ? (int64_t)(max_current_ac_target_bits | ~max_current_ac_target_mask)
        : (int64_t)max_current_ac_target_bits;
    max_ac_current_command->max_current_ac_target = (float)(max_current_ac_target_raw / 10);
}

void receive_max_ac_brake_current_command(const can_msg_t *message, max_ac_brake_current_command_t *max_ac_brake_current_command) {
    
    uint64_t data_bigendian;
    memcpy(&data_bigendian, message->data, 8);
    uint64_t data = __builtin_bswap64(data_bigendian);
    uint64_t max_ac_brake_current_target_mask = (1ULL << 16) - 1ULL;
    uint64_t max_ac_brake_current_target_bits = (data >> 48) & max_ac_brake_current_target_mask;
    int64_t max_ac_brake_current_target_raw = (max_ac_brake_current_target_bits & (1ULL << (16 - 1)))
        ? (int64_t)(max_ac_brake_current_target_bits | ~max_ac_brake_current_target_mask)
        : (int64_t)max_ac_brake_current_target_bits;
    max_ac_brake_current_command->max_ac_brake_current_target = (float)(max_ac_brake_current_target_raw / 10);
}

void receive_max_dc_current_command(const can_msg_t *message, max_dc_current_command_t *max_dc_current_command) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t max_dc_current_target_mask = (1ULL << 16) - 1ULL;
    uint64_t max_dc_current_target_bits = (data >> 0) & max_dc_current_target_mask;
    int64_t max_dc_current_target_raw = (max_dc_current_target_bits & (1ULL << (16 - 1)))
        ? (int64_t)(max_dc_current_target_bits | ~max_dc_current_target_mask)
        : (int64_t)max_dc_current_target_bits;
    max_dc_current_command->max_dc_current_target = (float)(max_dc_current_target_raw / 10);
}

void receive_max_dc_brake_current_command(const can_msg_t *message, max_dc_brake_current_command_t *max_dc_brake_current_command) {
    
    uint16_t data_bigendian;
    memcpy(&data_bigendian, message->data, 2);
    uint16_t data = __builtin_bswap16(data_bigendian);
    uint64_t max_dc_brake_current_target_mask = (1ULL << 16) - 1ULL;
    uint64_t max_dc_brake_current_target_bits = (data >> 0) & max_dc_brake_current_target_mask;
    int64_t max_dc_brake_current_target_raw = (max_dc_brake_current_target_bits & (1ULL << (16 - 1)))
        ? (int64_t)(max_dc_brake_current_target_bits | ~max_dc_brake_current_target_mask)
        : (int64_t)max_dc_brake_current_target_bits;
    max_dc_brake_current_command->max_dc_brake_current_target = (float)(max_dc_brake_current_target_raw / 10);
}

void receive_drive_enable_command(const can_msg_t *message, drive_enable_command_t *drive_enable_command) {
    
    uint8_t data = message->data[0];
    uint64_t drive_enable_mask = (1ULL << 8) - 1ULL;
    uint64_t drive_enable_raw = (data >> 0) & drive_enable_mask;
    drive_enable_command->drive_enable = (uint8_t)drive_enable_raw;
}

