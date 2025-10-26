#include <stdbool.h>
#include "u_statemachine.h"
#include "u_mutexes.h"

static Lightning_Board_Light_Status current_state = LIGHT_OFF;

/**
 * @brief sets the state of the lightning board
 * @param state the state to change to
 */
uint8_t set_statemachine(Lightning_Board_Light_Status state) {
    int status = mutex_get(&state_machine_mutex);

    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to get statemachine mutex. (Status: %d/%s).", status, tx_status_toString(status));
        return U_ERROR;
    }

    current_state = state;

    mutex_put(&state_machine_mutex);
}

/**
 * @brief returns the statemachine state
 * @return the state machine state
 */
Lightning_Board_Light_Status get_current_state() {
    return current_state;
}
