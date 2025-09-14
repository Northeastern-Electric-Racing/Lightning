#ifndef __U_MUTEX_H
#define __U_MUTEX_H

#include "tx_api.h"
#include "u_general.h"
#include <stdint.h>
#include <stdbool.h>

/* Mutex Config Macros */
#define MUTEX_WAIT_TIME TX_WAIT_FOREVER // Wait time for mutex stuff before timing out

typedef struct {
    /* PUBLIC: Mutex Configuration Settings */
    /* Set these when defining an instance of this struct. */
    const CHAR *name;            /* Name of Mutex */
    const UINT priority_inherit; /* Specifies if the mutex supports priority inheritance. See page 55 of the "Azure RTOS ThreadX User Guide". */

    /* PRIVATE: Internal implementation - DO NOT ACCESS DIRECTLY */
    /* (should only be accessed by functions in u_mutexes.c) */
    TX_MUTEX _TX_MUTEX;
} mutex_t;

/* Mutex List */
extern mutex_t state_machine_mutex; // State Machine Mutex
// add more as necessary...

/* API */
uint8_t mutexes_init(); // Initializes all mutexes set up in u_mutexes.c
uint8_t mutex_get(mutex_t *mutex); // Gets a mutex.
uint8_t mutex_put(mutex_t *mutex); // Puts a mutex.

#endif /* u_mutex.h */