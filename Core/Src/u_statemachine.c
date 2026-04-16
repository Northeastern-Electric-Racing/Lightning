#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include "u_can.h"
#include "can_messages_rx.h"
#include "u_statemachine.h"
#include "u_mutexes.h"

/* First contact trackers. */
static _Atomic bool has_bms_made_contact = false;
static _Atomic bool has_imd_made_contact = false;
// These bools track whether or not we've recieved any "okay" messages from BMS or IMD.
// Obviously, on startup, we won't have recieved any yet. So, these start as `false`, and remain so until the first "okay" messages are recieved from BMS and IMD respectively (i.e., "first contact" is made).
// While either of these bools are still `false`, the statemachine will always return LIGHT_OFF, indicating that we are still in our startup phase.
// Once both bools are `true`, meaning that we have actual "okay" states reported from both BMS and IMD, the statemachine will return either LIGHT_GREEN or LIGHT_RED based on those states.

/* "Okay" Statuses. */
static _Atomic bool bms_error; // Is the BMS okay? false = bms is okay, true = bms is NOT okay.
static _Atomic bool imd_error; // Is the IMD okay? false = imd is okay, true = imd is NOT okay.
// These values are updated via CAN messages that are sent from the BMS and IMD.
// As explained in the "first contact trackers" section, these bools are not used by the statemachine until at least one "okay" message has been received from each board.

/* Handles the IMD status message. */
#define _GET_BIT(data, bit) (((data) & (1U << (bit))) != 0U)
void statemachine_handleIMDMessage(can_msg_t* message) {
    /* Extract the warnings and alarms field (bytes 4 and 5 of the message). */
    uint16_t warnings_and_alarms = 0;
    memcpy(&warnings_and_alarms, &message->data[4], 2); // Copy over two bytes of the message, starting at byte 4. This should result in byte 4 and byte 5 being copied over.
    
    /* Get all the bit states from the register. */
    bool device_error_active = _GET_BIT(warnings_and_alarms, 0); // true = device error active
    bool HV_pos_connection_failure = _GET_BIT(warnings_and_alarms, 1); // true = HV_pos connection failure
    bool HV_neg_connection_failure = _GET_BIT(warnings_and_alarms, 2); // true = HV_neg connection failure
    bool Earth_connection_failure = _GET_BIT(warnings_and_alarms, 3); // true = Earth connection failure
    bool Iso_alarm = _GET_BIT(warnings_and_alarms, 4); // true = Iso value below threshold error
    bool Iso_warning = _GET_BIT(warnings_and_alarms, 5); // true = Iso value below treshold warning
    bool Iso_outdated = _GET_BIT(warnings_and_alarms, 6); // true = Iso outdated
    bool Unbalance_alarm = _GET_BIT(warnings_and_alarms, 7); // true = unbalane value below threshold
    bool Undervoltage_alarm = _GET_BIT(warnings_and_alarms, 8); // true = undervoltage alarm
    bool Unsafe_to_start = _GET_BIT(warnings_and_alarms, 9); // true = Unsafe to start
    bool Earthlift_open = _GET_BIT(warnings_and_alarms, 10); // true = Earthlift open

    /* Do we have an error? */
    imd_error = 
    device_error_active ||
    HV_pos_connection_failure ||
    HV_neg_connection_failure ||
    Earth_connection_failure ||
    Iso_alarm ||
    Iso_warning ||
    Iso_outdated ||
    Unbalance_alarm ||
    Undervoltage_alarm ||
    Unsafe_to_start ||
    Earthlift_open;
    // Right now, if any of these are true, we are considering it an error.

    /* Update `has_imd_made_contact`, since we have made contact if this has been called. */
    has_imd_made_contact = true;
}

/* Handles the BMS status message. */
void statemachine_handleBMSMessage(can_msg_t* message) {
    bms_critically_faulted_t data = { 0 };
    receive_bms_critically_faulted(message, &data);
    PRINTLN_INFO("bms critically faulted=%d", data.critically_faulted);
    bms_error = data.critically_faulted;
    has_bms_made_contact = true;
}

Lightning_Board_Light_Status statemachine_getState() {
    /* If we haven't made first contact yet from either board, just return LIGHT_OFF. */
    if(!has_bms_made_contact || !has_imd_made_contact) {
        return LIGHT_OFF;
    }
    
    /* If either the BMS or IMD has an error, return LIGHT_RED. */
    if(bms_error || imd_error) {
        return LIGHT_RED;
    }

    /* If everything is good, return LIGHT_GREEN. */
    return LIGHT_GREEN;
}
