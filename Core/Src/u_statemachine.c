#include <stdbool.h>
#include "u_statemachine.h"
#include "u_mutexes.h"

static state_t current_state = BOOTING;
static bool bms_alive = false;
static bool imd_alive = false;

static uint8_t _bms_alive(uint8_t *data) {
    bms_alive = true;

    if (bms_alive && imd_alive) {
        current_state = CAR_READY;
    }

    return U_SUCCESS;
}

static uint8_t _imu_alive(uint8_t *data) {
    imd_alive = true;

    if (bms_alive && imd_alive) {
        current_state = (state_t) CAR_READY;
    }

    return U_SUCCESS;
}

static uint8_t _stable(uint8_t *data) {
    if (current_state == CAR_READY || current_state == CAR_FAULTED) {
        return U_ERROR;
    }

    current_state = (state_t) STABLE;

    return U_SUCCESS;
}

static uint8_t _fault(uint8_t *data) {
    if (current_state == CAR_READY || current_state == CAR_STABLE) {
        return U_ERROR;
    }

    current_state = (state_t) FAULT;

    return U_SUCCESS;
}

uint8_t statemachine_process_event(event_t event, uint8_t *data) {
    int status = mutex_get(&state_machine_mutex);

    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to get statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return U_ERROR;
    }

    switch (event) {
        /* Call the function corresponding to the event that occured. */
        case BMS_ALIVE:          return _bms_alive(data);
        case IMD_ALIVE:          return _imu_alive(data);
        case STABLE:             return _stable(data);
        case FAULT:              return _fault(data);
        
        /* If an invalid event is passed in, return an error. */
        default: 
            DEBUG_PRINTLN("Invalid event passed into function. (Event: %d)", event);
            return U_ERROR;
    }

    mutex_put(&state_machine_mutex);
}

state_t get_current_state() {
    return current_state;
}
