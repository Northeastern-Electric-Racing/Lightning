#include <stdio.h>
#include "u_mutexes.h"
#include "u_general.h"

/* State Machine Mutex */
/* Used to protect multiple threads attempting to write to the fault flags variable at once. */
mutex_t state_machine_mutex = {
    .name = "Faults Mutex",        /* Name of the mutex. */
    .priority_inherit = TX_INHERIT /* Priority inheritance setting. */
};

/* Helper function. Creates a ThreadX mutex. */
static uint8_t _create_mutex(mutex_t *mutex) {
    uint8_t status = tx_mutex_create(&mutex->_TX_MUTEX, (CHAR*)mutex->name, mutex->priority_inherit);
    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to create mutex (Status: %d/%s, Name: %s).", status, tx_status_toString(status), mutex->name);
        return status;
    }

    return U_SUCCESS;
}

/* Initializes all ThreadX mutexes. 
*  Calls to _create_mutex() should go in here
*/
uint8_t mutexes_init() {
    /* Create Mutexes. */
    CATCH_ERROR(_create_mutex(&state_machine_mutex), U_SUCCESS);  // Create Faults Mutex.
    // add more as necessary.

    DEBUG_PRINTLN("Ran mutexes_init().");
    return U_SUCCESS;
}

/* Get a mutex. */
uint8_t mutex_get(mutex_t *mutex) {
    uint8_t status = tx_mutex_get(&mutex->_TX_MUTEX, MUTEX_WAIT_TIME);
    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to get mutex (Status: %d/%s, Mutex: %s).", status, tx_status_toString(status), mutex->name);
        return status;
    }

    return U_SUCCESS;
}

/* Put a mutex. */
uint8_t mutex_put(mutex_t *mutex) {
    uint8_t status = tx_mutex_put(&mutex->_TX_MUTEX);
    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to put mutex (Status: %d/%s, Mutex: %s).", status, tx_status_toString(status), mutex->name);
        return status;
    }

    return U_SUCCESS;
}