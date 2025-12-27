#include "fdcan.h"
#include "u_can.h"
#include "u_statemachine.h"

void inbox_can(can_msg_t *message) {
    switch(message->id) {
        case CERBERUS_MSG_ID:
            Lightning_Board_Light_Status state = message->data[0];
            set_statemachine(state);
            break;
        default:
            PRINTLN_WARNING("Unknown Inbox Message. ID: %lu", message->id);
            break;
    }
}