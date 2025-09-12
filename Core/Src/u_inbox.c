#include "fdcan.h"

/* Processes received CAN messages. */
void inbox_can(can_msg_t *message) {
    switch(message->id) {
        case 0x01:
            // do thing
            break;
        case 0x02:
            // do thing
            break;
        case 0x03:
            // etc
            break;
    }
}