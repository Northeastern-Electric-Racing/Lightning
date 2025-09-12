#include "fdcan.h"
#include "u_can.h"
#include "u_statemachine.h"

/* Processes received CAN messages. */
void inbox_can(can_msg_t *message) {
    switch(message->id) {
        case BMS_ALIVE_CAN_ID:
            statemachine_process_event(BMS_ALIVE, NULL);
            break;
        case IMD_ALIVE_CAN_ID:
            statemachine_process_event(IMD_ALIVE, NULL);
            break;
        case VCU_FAULTS_CAN_ID:
            if (is_car_faulted(message->data)) {
                statemachine_process_event(FAULT, NULL);
            }
            else {
                statemachine_process_event(STABLE, NULL);
            }
            break;
    }
}