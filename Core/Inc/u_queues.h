#ifndef __U_QUEUES_H
#define __U_QUEUES_H

#include "tx_api.h"
#include "u_tx_debug.h"
#include "u_tx_queues.h"
#include <stdint.h>

/* Queue List */
extern queue_t can_incoming; // Incoming CAN Queue
extern queue_t can_outgoing; // Outgoing CAN Queue
// add more as necessary

/* API */
uint8_t queues_init(TX_BYTE_POOL *byte_pool); // Initializes all queues. Called from app_threadx.c

#endif /* u_queues.h */