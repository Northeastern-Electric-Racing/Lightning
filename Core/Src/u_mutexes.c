#include <stdio.h>
#include "u_mutexes.h"
#include "u_tx_debug.h"

/* State Machine Mutex */
/* Used to protect multiple threads attempting to write to the fault flags variable at once. */
mutex_t state_machine_mutex = {
    .name = "Faults Mutex",        /* Name of the mutex. */
    .priority_inherit = TX_INHERIT /* Priority inheritance setting. */
};

uint8_t mutexes_init() {
    if (create_mutex(&state_machine_mutex) != U_SUCCESS) {
        PRINTLN_INFO("mutexes_init() failed.");
        return U_ERROR;
    }

    PRINTLN_INFO("Ran mutexes_init().");
    return U_SUCCESS;
}
