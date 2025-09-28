#include "u_queues.h"
#include "u_can.h"
#include "u_tx_debug.h"
#include <stdio.h>

/* 
*  NOTE: This file is kinda weird because of how ThreadX queues work. The size of each message in a ThreadX queue has to be a multiple of 4 bytes, since ThreadX
*  queues are implemented as arrays of 32-bit words. So, to create queues for structs that aren't a multiple of 4 bytes, you have to round up to the nearest multiple of 4 bytes.
*  This is handled in the _create_queue() function below.
*
*  Then, the queue_send() and queue_receive() functions use a buffer to send/receive messages to/from the queue.
*  This handles any conversions between the actual message size and the rounded-up size.
*
*  On the bright side (assuming my code works), this file should automatically handle all the 32-bit word stuff for you so you don't have to worry about it.
*
*/

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

/* Initializes all ThreadX queues. 
*  Calls to _create_queue() should go in here
*/
uint8_t queues_init(TX_BYTE_POOL *byte_pool) {

    /* Create Queues */
    CATCH_ERROR(create_queue(byte_pool, &can_incoming), U_SUCCESS); // Create Incoming CAN Queue
    CATCH_ERROR(create_queue(byte_pool, &can_outgoing), U_SUCCESS); // Create Outgoing CAN Queue

    DEBUG_PRINTLN("Ran queues_init().");
    return U_SUCCESS;
}