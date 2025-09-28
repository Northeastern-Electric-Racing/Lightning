#include <stdio.h>
#include "u_mutexes.h"
#include "u_tx_debug.h"

/* State Machine Mutex */
/* Used to protect multiple threads attempting to write to the fault flags variable at once. */
mutex_t state_machine_mutex = {
    .name = "Faults Mutex",        /* Name of the mutex. */
    .priority_inherit = TX_INHERIT /* Priority inheritance setting. */
};

/* Initializes all ThreadX mutexes. 
*  Calls to _create_mutex() should go in here
*/
uint8_t mutexes_init() {
    /* Create Mutexes. */
    CATCH_ERROR(create_mutex(&state_machine_mutex), U_SUCCESS);  // Create Faults Mutex.
    // add more as necessary.

    DEBUG_PRINTLN("Ran mutexes_init().");
    return U_SUCCESS;
}
