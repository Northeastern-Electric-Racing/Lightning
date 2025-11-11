#include <stdbool.h>
#include "u_statemachine.h"
#include "u_mutexes.h"

static Lightning_Board_Light_Status current_state = LIGHT_OFF;

uint8_t set_statemachine(Lightning_Board_Light_Status state) {
    int status = mutex_get(&state_machine_mutex);

    if (status != TX_SUCCESS) {
        PRINTLN_INFO("ERROR: Failed to get statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return U_ERROR;
    }

    current_state = state;

    status = mutex_put(&state_machine_mutex);

    if (status != TX_SUCCESS) {
        PRINTLN_INFO("ERROR: Failed to put statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return U_ERROR;
    }

    return U_SUCCESS;
}

Lightning_Board_Light_Status get_current_state() {
    int status = mutex_get(&state_machine_mutex);

    if (status != TX_SUCCESS) {
        PRINTLN_INFO("ERROR: Failed to get statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return LIGHT_OFF;
    }
    
    Lightning_Board_Light_Status state = current_state;
    
    status = mutex_put(&state_machine_mutex);

    if (status != TX_SUCCESS) {
        PRINTLN_INFO("ERROR: Failed to put statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return state;
    }

    return current_state;
}
