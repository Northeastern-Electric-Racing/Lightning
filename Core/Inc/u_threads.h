#ifndef __U_THREADS_H
#define __U_THREADS_H

#include "tx_api.h"
#include "u_tx_debug.h"
#include "u_tx_threads.h"
#include <stdint.h>
#include <stdio.h>

/* Initializes all threads. Called from app_threadx.c */
uint8_t threads_init(TX_BYTE_POOL *byte_pool);

/* Thread Functions */
void default_thread(ULONG thread_input);
void can_incoming_thread(ULONG thread_input);
void can_outgoing_thread(ULONG thread_input);
void sensors_thread(ULONG thread_input);
void gpio_lights_thread(ULONG thread_input);
void pulse_thread(ULONG thread_input);

#endif /* u_threads.h */