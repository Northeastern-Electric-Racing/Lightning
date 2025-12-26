#include "u_queues.h"
#include "u_can.h"
#include "u_tx_debug.h"
#include <stdio.h>

/* Incoming CAN Queue */
queue_t can_incoming = {
    .name = "Incoming CAN Queue",           /* Name of the queue. */
    .message_size = sizeof(can_msg_t),      /* Size of each queue message, in bytes. */
    .capacity = 10                          /* Number of messages the queue can hold. */
};

/* Outgoing CAN Queue */
queue_t can_outgoing = {
    .name = "Outgoing CAN Queue",          /* Name of the queue. */
    .message_size = sizeof(can_msg_t),     /* Size of each queue message, in bytes. */
    .capacity = 10                         /* Number of messages the queue can hold. */
};

uint8_t queues_init(TX_BYTE_POOL *byte_pool) {
    if (create_queue(byte_pool, &can_incoming) != U_SUCCESS) {
        PRINTLN_INFO("CAN Incoming queue creation failed.");
        return U_ERROR;
    }

    if (create_queue(byte_pool, &can_outgoing) != U_SUCCESS) {
        PRINTLN_INFO("CAN Outgoing queue creation failed.");
        return U_ERROR;
    }

    PRINTLN_INFO("Ran queues_init().");
    return U_SUCCESS;
}