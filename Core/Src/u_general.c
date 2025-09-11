#include "u_general.h"
#include "tx_api.h"
#include "nx_api.h"
#include "stm32h5xx_hal.h"
#include <stdio.h>

/* Converts a ThreadX status macro to a printable string. */
/* This function is intended to be used with DEBUG_PRINTLN(), and shouldn't really ever be used outside of debugging purposes. */
/* (these macros are defined in tx_api.h) */
const char* tx_status_toString(UINT status) {
    switch(status) {
        case TX_SUCCESS: return "TX_SUCCESS";
        case TX_DELETED: return "TX_DELETED";
        case TX_POOL_ERROR: return "TX_POOL_ERROR";
        case TX_PTR_ERROR: return "TX_PTR_ERROR";
        case TX_WAIT_ERROR: return "TX_WAIT_ERROR";
        case TX_SIZE_ERROR: return "TX_SIZE_ERROR";
        case TX_GROUP_ERROR: return "TX_GROUP_ERROR";
        case TX_NO_EVENTS: return "TX_NO_EVENTS";
        case TX_OPTION_ERROR: return "TX_OPTION_ERROR";
        case TX_QUEUE_ERROR: return "TX_QUEUE_ERROR";
        case TX_QUEUE_EMPTY: return "TX_QUEUE_EMPTY";
        case TX_QUEUE_FULL: return "TX_QUEUE_FULL";
        case TX_SEMAPHORE_ERROR: return "TX_SEMAPHORE_ERROR";
        case TX_NO_INSTANCE: return "TX_NO_INSTANCE";
        case TX_THREAD_ERROR: return "TX_THREAD_ERROR";
        case TX_PRIORITY_ERROR: return "TX_PRIORITY_ERROR";
        case TX_NO_MEMORY: return "TX_NO_MEMORY"; // Same as TX_START_ERROR
        case TX_DELETE_ERROR: return "TX_DELETE_ERROR";
        case TX_RESUME_ERROR: return "TX_RESUME_ERROR";
        case TX_CALLER_ERROR: return "TX_CALLER_ERROR";
        case TX_SUSPEND_ERROR: return "TX_SUSPEND_ERROR";
        case TX_TIMER_ERROR: return "TX_TIMER_ERROR";
        case TX_TICK_ERROR: return "TX_TICK_ERROR";
        case TX_ACTIVATE_ERROR: return "TX_ACTIVATE_ERROR";
        case TX_THRESH_ERROR: return "TX_THRESH_ERROR";
        case TX_SUSPEND_LIFTED: return "TX_SUSPEND_LIFTED";
        case TX_WAIT_ABORTED: return "TX_WAIT_ABORTED";
        case TX_WAIT_ABORT_ERROR: return "TX_WAIT_ABORT_ERROR";
        case TX_MUTEX_ERROR: return "TX_MUTEX_ERROR";
        case TX_NOT_AVAILABLE: return "TX_NOT_AVAILABLE";
        case TX_NOT_OWNED: return "TX_NOT_OWNED";
        case TX_INHERIT_ERROR: return "TX_INHERIT_ERROR";
        case TX_NOT_DONE: return "TX_NOT_DONE";
        case TX_CEILING_EXCEEDED: return "TX_CEILING_EXCEEDED";
        case TX_INVALID_CEILING: return "TX_INVALID_CEILING";
        case TX_FEATURE_NOT_ENABLED: return "TX_FEATURE_NOT_ENABLED";
        default: return "UNKNOWN_STATUS";
    }
}

/* Converts a NetX status macro to a printable string. */
/* This function is intended to be used with DEBUG_PRINTLN(), and shouldn't really ever be used outside of debugging purposes. */
/* (these macros are defined in nx_api.h) */
const char* nx_status_toString(UINT status) {
    switch(status) {
        case NX_SUCCESS: return "NX_SUCCESS";
        case NX_NO_PACKET: return "NX_NO_PACKET";
        case NX_UNDERFLOW: return "NX_UNDERFLOW";
        case NX_OVERFLOW: return "NX_OVERFLOW";
        case NX_NO_MAPPING: return "NX_NO_MAPPING";
        case NX_DELETED: return "NX_DELETED";
        case NX_POOL_ERROR: return "NX_POOL_ERROR";
        case NX_PTR_ERROR: return "NX_PTR_ERROR";
        case NX_WAIT_ERROR: return "NX_WAIT_ERROR";
        case NX_SIZE_ERROR: return "NX_SIZE_ERROR";
        case NX_OPTION_ERROR: return "NX_OPTION_ERROR";
        case NX_DELETE_ERROR: return "NX_DELETE_ERROR";
        case NX_CALLER_ERROR: return "NX_CALLER_ERROR";
        case NX_INVALID_PACKET: return "NX_INVALID_PACKET";
        case NX_INVALID_SOCKET: return "NX_INVALID_SOCKET";
        case NX_NOT_ENABLED: return "NX_NOT_ENABLED";
        case NX_ALREADY_ENABLED: return "NX_ALREADY_ENABLED";
        case NX_ENTRY_NOT_FOUND: return "NX_ENTRY_NOT_FOUND";
        case NX_NO_MORE_ENTRIES: return "NX_NO_MORE_ENTRIES";
        case NX_ARP_TIMER_ERROR: return "NX_ARP_TIMER_ERROR";
        case NX_RESERVED_CODE0: return "NX_RESERVED_CODE0";
        case NX_WAIT_ABORTED: return "NX_WAIT_ABORTED";
        case NX_IP_INTERNAL_ERROR: return "NX_IP_INTERNAL_ERROR";
        case NX_IP_ADDRESS_ERROR: return "NX_IP_ADDRESS_ERROR";
        case NX_ALREADY_BOUND: return "NX_ALREADY_BOUND";
        case NX_PORT_UNAVAILABLE: return "NX_PORT_UNAVAILABLE";
        case NX_NOT_BOUND: return "NX_NOT_BOUND";
        case NX_RESERVED_CODE1: return "NX_RESERVED_CODE1";
        case NX_SOCKET_UNBOUND: return "NX_SOCKET_UNBOUND";
        case NX_NOT_CREATED: return "NX_NOT_CREATED";
        case NX_SOCKETS_BOUND: return "NX_SOCKETS_BOUND";
        case NX_NO_RESPONSE: return "NX_NO_RESPONSE";
        case NX_POOL_DELETED: return "NX_POOL_DELETED";
        case NX_ALREADY_RELEASED: return "NX_ALREADY_RELEASED";
        case NX_RESERVED_CODE2: return "NX_RESERVED_CODE2";
        case NX_MAX_LISTEN: return "NX_MAX_LISTEN";
        case NX_DUPLICATE_LISTEN: return "NX_DUPLICATE_LISTEN";
        case NX_NOT_CLOSED: return "NX_NOT_CLOSED";
        case NX_NOT_LISTEN_STATE: return "NX_NOT_LISTEN_STATE";
        case NX_IN_PROGRESS: return "NX_IN_PROGRESS";
        case NX_NOT_CONNECTED: return "NX_NOT_CONNECTED";
        case NX_WINDOW_OVERFLOW: return "NX_WINDOW_OVERFLOW";
        case NX_ALREADY_SUSPENDED: return "NX_ALREADY_SUSPENDED";
        case NX_DISCONNECT_FAILED: return "NX_DISCONNECT_FAILED";
        case NX_STILL_BOUND: return "NX_STILL_BOUND";
        case NX_NOT_SUCCESSFUL: return "NX_NOT_SUCCESSFUL";
        case NX_UNHANDLED_COMMAND: return "NX_UNHANDLED_COMMAND";
        case NX_NO_FREE_PORTS: return "NX_NO_FREE_PORTS";
        case NX_INVALID_PORT: return "NX_INVALID_PORT";
        case NX_INVALID_RELISTEN: return "NX_INVALID_RELISTEN";
        case NX_CONNECTION_PENDING: return "NX_CONNECTION_PENDING";
        case NX_TX_QUEUE_DEPTH: return "NX_TX_QUEUE_DEPTH";
        case NX_NOT_IMPLEMENTED: return "NX_NOT_IMPLEMENTED";
        case NX_NOT_SUPPORTED: return "NX_NOT_SUPPORTED";
        case NX_INVALID_INTERFACE: return "NX_INVALID_INTERFACE";
        case NX_INVALID_PARAMETERS: return "NX_INVALID_PARAMETERS";
        case NX_NOT_FOUND: return "NX_NOT_FOUND";
        case NX_CANNOT_START: return "NX_CANNOT_START";
        case NX_NO_INTERFACE_ADDRESS: return "NX_NO_INTERFACE_ADDRESS";
        case NX_INVALID_MTU_DATA: return "NX_INVALID_MTU_DATA";
        case NX_DUPLICATED_ENTRY: return "NX_DUPLICATED_ENTRY";
        case NX_PACKET_OFFSET_ERROR: return "NX_PACKET_OFFSET_ERROR";
        case NX_OPTION_HEADER_ERROR: return "NX_OPTION_HEADER_ERROR";
        case NX_CONTINUE: return "NX_CONTINUE";
        case NX_TCPIP_OFFLOAD_ERROR: return "NX_TCPIP_OFFLOAD_ERROR";
        default: return "UNKNOWN_STATUS";
    }
}

/* Converts a STM32 HAL status macro to a printable string. */
/* This function is intended to be used with DEBUG_PRINTLN(), and shouldn't really ever be used outside of debugging purposes. */
/* (these macros are defined in stm32h5xx_hal_def.h) */
const char* hal_status_toString(HAL_StatusTypeDef status) {
    switch(status) {
        case HAL_OK: return "HAL_OK";
        case HAL_ERROR: return "HAL_ERROR";
        case HAL_BUSY: return "HAL_BUSY";
        case HAL_TIMEOUT: return "HAL_TIMEOUT";
        default: return "UNKNOWN_STATUS";
    }
}