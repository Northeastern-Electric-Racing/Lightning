#include <stdint.h>
#include "u_can.h"
#include "u_tx_debug.h"
#include "stm32h5xx_hal.h"


/* CAN interfaces */
can_t can2;

uint8_t can2_init(FDCAN_HandleTypeDef *hcan) {
    /* Init CAN interface */
    HAL_StatusTypeDef status = can_init(&can2, hcan);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to execute can_init() when initializing can2 (Status: %d/%s).", status, hal_status_toString(status));
        return U_ERROR;
    }

    /* Add filters for standard IDs */
    uint16_t standard_ids[] = { IMD_GENERAL_MSG_ID, CERBERUS_MSG_ID };
    status = can_add_filter_standard(&can2, standard_ids);
    if(status != HAL_OK) {
        PRINTLN_ERROR("Failed to add standard filter to can2 (Status: %d/%s, ID1: %d, ID2: %d).", status, hal_status_toString(status), standard_ids[0], standard_ids[1]);
        return U_ERROR;
    }

    /* Add filters for extended IDs */
    uint32_t extended_ids[] = { BMS_LIGHTNING_OKAY_MSG_ID, CERBERUS_MSG_ID };
    status = can_add_filter_extended(&can2, extended_ids);
    if (status != HAL_OK) {
        PRINTLN_ERROR("Failed to add extended filter to can2 (Status: %d/%s, ID1: %lu, ID2: %lu).", status, hal_status_toString(status), extended_ids[0], extended_ids[1]);
        return U_ERROR;
    }

    PRINTLN_INFO("Ran can2_init().");  

    return U_SUCCESS;
}