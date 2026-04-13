#include "fdcan.h"
#include "u_can.h"
#include "u_statemachine.h"

void inbox_can(can_msg_t *message) {
    switch(message->id) {
        case IMD_GENERAL_MSG_ID:
            statemachine_handleIMDMessage(message);
            break;
        case BMS_LIGHTNING_OKAY_MSG_ID:
            statemachine_handleBMSMessage(message);
            break;
        default:
            PRINTLN_WARNING("Unknown Inbox Message. ID: %lu", message->id);
            break;
    }
}