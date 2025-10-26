#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "u_tx_debug.h"
#include <stdint.h>
#include <stdio.h>

/* States */
typedef enum {
    LIGHT_OFF = 0,      /**  Light Off      */
    LIGHT_GREEN = 1,    /**  Light Green On */
    LIGHT_RED = 2       /**  Light Red ON   */
} Lightning_Board_Light_Status;

/**
 * @brief sets the state of the lightning board
 * @param state the state to change to
 */
uint8_t set_statemachine(Lightning_Board_Light_Status state);

/**
 * @brief returns the statemachine state
 * @return the state machine state
 */
Lightning_Board_Light_Status get_current_state();

#endif /* u_statemachine.h */

