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
            PRINTLN_INFO("can - receieved the bms lightning message");
            statemachine_handleBMSMessage(message);
            break;
        default:
            PRINTLN_WARNING("Unknown Inbox Message. ID: 0x%X", message->id);
            break;
    }
}