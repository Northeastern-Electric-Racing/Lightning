#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "u_tx_debug.h"
#include "u_can.h"
#include <stdint.h>
#include <stdio.h>

/* States */
typedef enum {
    LIGHT_OFF = 0,      /**  Light Off      */
    LIGHT_GREEN = 1,    /**  Light Green On */
    LIGHT_RED = 2       /**  Light Red ON   */
} Lightning_Board_Light_Status;

/**
 * @brief Determines the appropriate Lighting board light state based on the IMD and BMS statuses.
 * @return The Lightning Board Light Status.
 */
Lightning_Board_Light_Status statemachine_getState();

void statemachine_handleIMDMessage(can_msg_t* message); // Handles the IMD status message. 
void statemachine_handleBMSMessage(can_msg_t* message); // Handles the BMS status message.
void statemachine_handleResetLatchMessage(can_msg_t *message); // Handles the RESET LATCHING FAULTS message
int statemachine_init(void); // Start the lightning timeout timer.
 
#endif /* u_statemachine.h */

