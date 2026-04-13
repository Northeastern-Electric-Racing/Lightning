#ifndef __U_MUTEX_H
#define __U_MUTEX_H

#include "tx_api.h"
#include "u_tx_debug.h"
#include "u_tx_mutex.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes all mutexes
 * @return Returns a tx status
 */
uint8_t mutexes_init();

#endif /* u_mutex.h */