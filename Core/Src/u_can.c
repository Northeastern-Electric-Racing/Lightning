#include <stdint.h>
#include "u_can.h"
#include "u_general.h"
#include "stm32h5xx_hal.h"


/* CAN interfaces */
can_t can1;

uint8_t can1_init(FDCAN_HandleTypeDef *hcan) {
    
    /* Init CAN interface */
    HAL_StatusTypeDef status = can_init(&can1, hcan);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("Failed to execute can_init() when initializing can1 (Status: %d/%s).", status, hal_status_toString(status));
        return U_ERROR;
    }

    /* Add filters for standard IDs */
    uint16_t standard_ids[] = {BMS_ALIVE_CAN_ID, IMD_ALIVE_CAN_ID}; // TODO: can_add_filter_standard accepts a range; I do not know what this range should be; fix later
    status = can_add_filter_standard(&can1, &standard_ids);
    if(status != HAL_OK) {
        DEBUG_PRINTLN("Failed to add standard filter to can1 (Status: %d/%s, ID1: %d, ID2: %d).", status, hal_status_toString(status), standard_ids[0], standard_ids[1]);
        return U_ERROR;
    }

    /* Add fitlers for extended IDs */
    uint16_t extended_ids[] = {BMS_ALIVE_CAN_ID, IMD_ALIVE_CAN_ID}; // TODO: can_add_filter_standard accepts a range; I do not know what this range should be; fix later
    status = can_add_filter_extended(&can1, extended_ids);
    if (status != HAL_OK) {
        DEBUG_PRINTLN("Failed to add extended filter to can1 (Status: %d/%s, ID1: %ld, ID2: %ld).", status, hal_status_toString(status), extended_ids[0], extended_ids[1]);
        return U_ERROR;
    }

    DEBUG_PRINTLN("Ran can1_init().");

    return U_SUCCESS;
}