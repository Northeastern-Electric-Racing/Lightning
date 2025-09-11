#ifndef __U_GENERAL_H
#define __U_GENERAL_H

#include <string.h>
#include <stdio.h>
#include "tx_port.h"
#include "stm32h5xx_hal.h"

/* This file (and u_general.c) contain miscellaneous macros, functions, and configuration settings that can be used throughout the project. */

/* General-purpose status macros. */
#define U_SUCCESS     0
#define U_ERROR       1
#define U_QUEUE_EMPTY 2

/* Debugging Macros */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) /* Gets the name of the file. */
#define U_DEBUG /* Comment out to enable/disable debugging. */
#ifdef U_DEBUG
    #define DEBUG_PRINTLN(message, ...) printf("[%s/%s()] " message "\n", __FILENAME__, __func__, ##__VA_ARGS__) /* Prints an error message in the format: "[file_name.c/function()] {message}"*/
#else
    #define DEBUG_PRINTLN(message, ...) /* If debugging is turned off, macro doesn't need to expand to anything. */
#endif

/**
 * @brief Checks if a function is successful when called. DEBUG_PRINTLNs an error message if it fails.
 * @param function_call The function to call.
 * @param success The function's success code/macro (e.g., U_SUCCESS, TX_SUCCESS, etc.).
 * 
 * @note This macro intentionally doesn't support custom error messages, for the sake of readability. If an error is complex enough to 
 *       require a custom message, the error should probably be checked manually and DEBUG_PRINTLN() called directly.
 */
#define CATCH_ERROR(function_call, success) do { \
    int _function_status = (function_call); \
    if (_function_status != success) { \
        DEBUG_PRINTLN("CATCH_ERROR(): Function failed: %s (Status: %d)", #function_call, _function_status); \
        return _function_status; \
    } \
} while(0)

/* Time and tick conversions */
#define MS_TO_TICKS(ms) (((ms) * TX_TIMER_TICKS_PER_SECOND + 999) / 1000)
#define TICKS_TO_MS(ticks)  ((ticks) * 1000 / TX_TIMER_TICKS_PER_SECOND)

/* These functions convert status macros to a printable string. */
/* They are intended to be used with DEBUG_PRINTLN(), and shouldn't really be used outside of debugging purposes. */
const char* tx_status_toString(UINT status); // Converts a ThreadX status macro to a printable string.
const char* nx_status_toString(UINT status); // Converts a NetX status macro to a printable string.
const char* hal_status_toString(HAL_StatusTypeDef status); // Converts a HAL status macro to a printable string.

#endif /* u_general.h */