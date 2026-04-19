#include "main.h"
#include "u_threads.h"
#include "u_queues.h"
#include "u_inbox.h"
#include "u_can.h"
#include "u_sensors.h"
#include "bitstream.h"
#include "u_lights.h"
#include "u_statemachine.h"
#include "can_messages_tx.h"
#include "u_mutexes.h"
#include "u_test.h"

#define TEST_MODE 0

/* Default Thread */
static thread_t _default_thread = {
        .name       = "Default Thread",  /* Name */
        .size       = 4096,               /* Stack Size (in bytes) */
        .priority   = 0,                 /* Priority */
        .threshold  = 0,                 /* Preemption Threshold */
        .time_slice = TX_NO_TIME_SLICE,  /* Time Slice */
        .auto_start = TX_AUTO_START,     /* Auto Start */
        .sleep      = 10,                /* Sleep (in ticks) */
        .function   = default_thread     /* Thread Function */
    };
void default_thread(ULONG thread_input) {

    bool alt = true;
    
    while(1) {
        /* Kick the watchdogs (sad) */
        HAL_IWDG_Refresh(&hiwdg); // Internal Watchdog

        if (alt) {
			printf(".\n");
		} else {
			printf("..\n");
		}

		alt = !alt;

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_default_thread.sleep);
    }
}

/* CAN Incoming Thread. Processes incoming messages. */
static thread_t _can_incoming_thread = {
        .name       = "CAN Incoming Thread",     /* Name */
        .size       = 4096,                       /* Stack Size (in bytes) */
        .priority   = 0,                         /* Priority */
        .threshold  = 0,                         /* Preemption Threshold */
        .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
        .auto_start = TX_AUTO_START,             /* Auto Start */
        .sleep      = 10,                        /* Sleep (in ticks) */
        .function   = can_incoming_thread        /* Thread Function */
    };
void can_incoming_thread(ULONG thread_input) {
    
    while(1) {
        can_msg_t message;

        /* Process incoming messages */
        while(queue_receive(&can_incoming, &message, TX_WAIT_FOREVER) == U_SUCCESS) {
            inbox_can(&message);
        }

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_can_incoming_thread.sleep);
    }
}

/* CAN Outgoing Thread. Sends outgoing CAN messages. */
static thread_t _can_outgoing_thread = {
    .name       = "CAN Outgoing Thread",     /* Name */
    .size       = 4096,                       /* Stack Size (in bytes) */
    .priority   = 0,                         /* Priority */
    .threshold  = 0,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 10,                        /* Sleep (in ticks) */
    .function   = can_outgoing_thread        /* Thread Function */
};
void can_outgoing_thread(ULONG thread_input) {

    while(1) {

        can_msg_t message;
        uint8_t status;

        /* Process outgoing messages */
        while(queue_receive(&can_outgoing, &message, TX_WAIT_FOREVER) == U_SUCCESS) {
            status = can_send_msg(&can2, &message);
            if(status != HAL_OK) {
                PRINTLN_WARNING("WARNING: Failed to send message (on can2) after removing from outgoing queue (Message ID: %ld, Status: %d/%s).", message.id, status, hal_status_toString(status));
            }
        }

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_can_outgoing_thread.sleep);
    }
}

/* Sensors Thread. Reads sensors's information. */
static thread_t _sensors_thread = {
    .name       = "Sensors Thread",          /* Name */
    .size       = 4096,                       /* Stack Size (in bytes) */
    .priority   = 0,                         /* Priority */
    .threshold  = 0,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 500,                       /* Sleep (in ticks) */
    .function   = sensors_thread             /* Thread Function */
};
void sensors_thread(ULONG thread_input) {
    
    while (1) {
        if (read_lightning_sensor() != U_SUCCESS) {
            PRINTLN_ERROR("Reading & Sending Lightning Sensor Data Failed.");
        }

        // if (read_imu() != U_SUCCESS) {
            // PRINTLN_ERROR("Reading & Sending IMU Data Failed.");
        // }

        if (read_magnetometer() != U_SUCCESS) {
            PRINTLN_ERROR("Reading & Sending Magnetometer Data Failed.");
        }

        tx_thread_sleep(_sensors_thread.sleep);
    }
}

/* GPIO Lights */
static thread_t _gpio_lights_thread = {
    .name       = "GPIO Lights",             /* Name */
    .size       = 4096,                       /* Stack Size (in bytes) */
    .priority   = 0,                         /* Priority */
    .threshold  = 0,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 10,                        /* Sleep (in ticks) */
    .function   = gpio_lights_thread         /* Thread Function */
};
void gpio_lights_thread(ULONG thread_input) {

    while (1) {
        Lightning_Board_Light_Status state = statemachine_getState();

        switch (state) {
            case LIGHT_GREEN:
                lights_setGreen();
                break;
            case LIGHT_RED:
                lights_setRed();
                break;
            case LIGHT_OFF:
                lights_setOff();
                break;
            default:
                PRINTLN_WARNING("State machine state is not in range %d", state);
                break;
        }

        tx_thread_sleep(_gpio_lights_thread.sleep);

        #if TEST_MODE
        gpio_test();
        #endif
    }
}

/* Lightning Pulse Thread */
static thread_t _pulse_thread = {
    .name       = "Lightning Pulse Thread",  /* Name */
    .size       = 4096,                      /* Stack Size (in bytes) */
    .priority   = 0,                         /* Priority */
    .threshold  = 0,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 500,                       /* Sleep (in ticks) */
    .function   = pulse_thread               /* Thread Function */
};
void pulse_thread(ULONG thread_input) {
    
    while (1) {
        static uint32_t count = 0;
        count++;
        send_lightning_pulse_message(count);

        tx_thread_sleep(_pulse_thread.sleep);
    }
}

/* Initializes all ThreadX threads. 
 * Calls to _create_thread() should go in here
 */
uint8_t threads_init(TX_BYTE_POOL *byte_pool) {

    /* Create Threads */
    CATCH_ERROR(create_thread(byte_pool, &_default_thread), U_SUCCESS);           // Create Default thread.
    CATCH_ERROR(create_thread(byte_pool, &_can_incoming_thread), U_SUCCESS);      // Create CAN Incoming thread.
    CATCH_ERROR(create_thread(byte_pool, &_can_outgoing_thread), U_SUCCESS);      // Create CAN Outgoing thread.
    CATCH_ERROR(create_thread(byte_pool, &_sensors_thread), U_SUCCESS);           // Create Sensor thread.
    CATCH_ERROR(create_thread(byte_pool, &_gpio_lights_thread), U_SUCCESS);       // Create GPIO Lights thread.
    CATCH_ERROR(create_thread(byte_pool, &_pulse_thread), U_SUCCESS);             // Create Pulse Thread

    PRINTLN_INFO("Ran threads_init().");
    return U_SUCCESS;
}
