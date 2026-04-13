#include <stdio.h>
#include "u_mutexes.h"
#include "u_tx_debug.h"

// No mutexes as of right now.

uint8_t mutexes_init() {
    PRINTLN_INFO("Ran mutexes_init().");
    return U_SUCCESS;
}
