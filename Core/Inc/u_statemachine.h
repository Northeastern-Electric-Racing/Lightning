#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "u_general.h"
#include <stdint.h>
#include <stdio.h>

/* States */
typedef enum {
    BOOTING,    // BMS & IMD are currently booting
    CAR_READY,  // BMS & IMD are done booting; waiting the recieve can fault messages
    CAR_STABLE, // No faults; Car is good;
    CAR_FAULTED // Car Faulted
} state_t;

/* Events */
typedef enum {
    BMS_ALIVE, // BMS is active
    IMD_ALIVE, // IMD is active
    STABLE,    // Car is Stable
    FAULT,     // Car is Fualted
} event_t;

/**
 * @brief Processes an event based on the current state.
 * @param event The event that was triggered.
 * @param data Pointer to any data associated with the event. Can be NULL if not needed.
 */
uint8_t statemachine_process_event(event_t event, uint8_t *data);

/**
 * @brief Returns the current machine state
 * @return machine state
 */
state_t get_current_state();

#endif /* u_statemachine.h */

