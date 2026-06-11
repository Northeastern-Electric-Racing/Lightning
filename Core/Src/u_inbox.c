#include "fdcan.h"
#include "u_can.h"
#include "u_statemachine.h"

void inbox_can(can_msg_t *message) {
    PRINTLN_INFO("can - incoming can message");
    switch(message->id) {
        case IMD_GENERAL_MSG_ID:
            statemachine_handleIMDMessage(message);
            break;
        case BMS_LIGHTNING_OKAY_MSG_ID:
            statemachine_handleBMSMessage(message);
            break;
        case RESET_LATCHING_MSG_ID:
            statemachine_handleResetLatchMessage(message);
            break;
        default:
            PRINTLN_WARNING("Unknown Inbox Message. ID: 0x%X", message->id);
            break;
    }
}