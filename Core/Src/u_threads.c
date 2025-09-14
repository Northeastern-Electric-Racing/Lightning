#include "main.h"
#include "u_threads.h"
#include "u_queues.h"
#include "u_inbox.h"
#include "u_can.h"
#include "u_sensors.h"
#include "bitstream.h"
#include "u_statemachine.h"

/* Default Thread */
static thread_t _default_thread = {
        .name       = "Default Thread",  /* Name */
        .size       = 512,               /* Stack Size (in bytes) */
        .priority   = 7,                 /* Priority */
        .threshold  = 9,                 /* Preemption Threshold */
        .time_slice = TX_NO_TIME_SLICE,  /* Time Slice */
        .auto_start = TX_AUTO_START,     /* Auto Start */
        .sleep      = 500,               /* Sleep (in ticks) */
        .function   = default_thread     /* Thread Function */
    };
void default_thread(ULONG thread_input) {
    
    while(1) {

        /* Kick the watchdogs (sad) )*/
        HAL_IWDG_Refresh(&hiwdg); // Internal Watchdog

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_default_thread.sleep);
    }
}

/* CAN Incoming Thread. Processes incoming messages. */
static thread_t _can_incoming_thread = {
        .name       = "CAN Incoming Thread",     /* Name */
        .size       = 512,                       /* Stack Size (in bytes) */
        .priority   = 5,                         /* Priority */
        .threshold  = 9,                         /* Preemption Threshold */
        .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
        .auto_start = TX_AUTO_START,             /* Auto Start */
        .sleep      = 500,                       /* Sleep (in ticks) */
        .function   = can_incoming_thread        /* Thread Function */
    };
void can_incoming_thread(ULONG thread_input) {
    
    while(1) {

        can_msg_t message;

        /* Process incoming messages */
        while(queue_receive(&can_incoming, &message) == U_SUCCESS) {
            inbox_can(&message);
        }

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_can_incoming_thread.sleep);
    }
}

/* CAN Outgoing Thread. Sends outgoing CAN messages. */
static thread_t _can_outgoing_thread = {
    .name       = "CAN Outgoing Thread",     /* Name */
    .size       = 512,                       /* Stack Size (in bytes) */
    .priority   = 9,                         /* Priority */
    .threshold  = 9,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 500,                       /* Sleep (in ticks) */
    .function   = can_outgoing_thread        /* Thread Function */
};
void can_outgoing_thread(ULONG thread_input) {

    while(1) {

        can_msg_t message;
        uint8_t status;

        /* Process outgoing messages */
        while(queue_receive(&can_outgoing, &message) == U_SUCCESS) {
            status = can_send_msg(&can2, &message);
            if(status != U_SUCCESS) {
                DEBUG_PRINTLN("WARNING: Failed to send message (on can2) after removing from outgoing queue (Message ID: %ld).", message.id);
                // u_TODO - maybe add the message back into the queue if it fails to send? not sure if this is a good idea tho
                }
        }

        /* Sleep Thread for specified number of ticks. */
        tx_thread_sleep(_can_outgoing_thread.sleep);
    }
}

static uint32_t lightning_sensor_value;
static uint32_t imu_value;
static uint32_t magnetometer_value;

/* Sensors Thread. Reads sensors's information. */
static thread_t _sensors_thread = {
    .name       = "Sensors Thread",          /* Name */
    .size       = 512,                       /* Stack Size (in bytes) */
    .priority   = 9,                         /* Priority */
    .threshold  = 9,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 500,                       /* Sleep (in ticks) */
    .function   = sensors_thread             /* Thread Function */
};
void sensors_thread(ULONG thread_input) {
    
    while (1) {

        lightning_sensor_value = read_lightning_sensor();
        imu_value = read_imu();
        magnetometer_value = read_magnetometer();
        
        queue_send(&can_outgoing, &lightning_sensor_value);
        queue_send(&can_outgoing, &imu_value);
        queue_send(&can_outgoing, &magnetometer_value);

        tx_thread_sleep(_sensors_thread.sleep);
    }
}

/* GPIO Lights */
static thread_t _gpio_lights_thread = {
    .name       = "GPIO Lights",             /* Name */
    .size       = 512,                       /* Stack Size (in bytes) */
    .priority   = 5,                         /* Priority */
    .threshold  = 9,                         /* Preemption Threshold */
    .time_slice = TX_NO_TIME_SLICE,          /* Time Slice */
    .auto_start = TX_AUTO_START,             /* Auto Start */
    .sleep      = 500,                       /* Sleep (in ticks) */
    .function   = gpio_lights_thread         /* Thread Function */
};
void gpio_lights_thread(ULONG thread_input) {
    
    while (1) {

        state_t state = get_current_state();

        if (state == CAR_STABLE) {
            // TODO: GPIO GREEN
            //HAL_GPIO_WritePin(GREEN_Pin, GREEN_GPIO_Port, 1);
            //HAL_GPIO_WritePin(RED_Pin, RED_GPIO_Port, 0);
            return;
        }

        if (state == CAR_FAULTED) {
            // TODO: GPIO RED
            //HAL_GPIO_WritePin(GREEN_Pin, GREEN_GPIO_Port, 0);
            //HAL_GPIO_WritePin(RED_Pin, RED_GPIO_Port, 1);
            return;
        }

        // TODO: Reset GPIO
        //HAL_GPIO_WritePin(GREEN_Pin, GREEN_GPIO_Port, 0);
        //HAL_GPIO_WritePin(RED_Pin, RED_GPIO_Port, 0);

        tx_thread_sleep(_gpio_lights_thread.sleep);
    }
}

/* Helper function. Creates a ThreadX thread. */
static uint8_t _create_thread(TX_BYTE_POOL *byte_pool, thread_t *thread) {
    CHAR *pointer;
    uint8_t status;

    /* Allocate the stack for the thread. */
    status = tx_byte_allocate(byte_pool, (VOID**) &pointer, thread->size, TX_NO_WAIT);
    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to allocate stack before creating thread (Status: %d/%s, Thread: %s).", status, tx_status_toString(status), thread->name);
        return U_ERROR;
    }

    /* Create the thread. */
    status = tx_thread_create(&thread->_TX_THREAD, (CHAR*)thread->name, thread->function, thread->thread_input, pointer, thread->size, thread->priority, thread->threshold, thread->time_slice, thread->auto_start);
    if(status != TX_SUCCESS) {
        DEBUG_PRINTLN("ERROR: Failed to create thread (Status: %d/%s, Thread: %s).", status, tx_status_toString(status), thread->name);
        tx_byte_release(pointer); // Free allocated memory if thread creation fails
        return U_ERROR;
    }
    
    return U_SUCCESS;
}

/* Initializes all ThreadX threads. 
*  Calls to _create_thread() should go in here
*/
uint8_t threads_init(TX_BYTE_POOL *byte_pool) {

    /* Create Threads */
    CATCH_ERROR(_create_thread(byte_pool, &_default_thread), U_SUCCESS);           // Create Default thread.
    CATCH_ERROR(_create_thread(byte_pool, &_can_incoming_thread), U_SUCCESS);      // Create CAN Incoming thread.
    CATCH_ERROR(_create_thread(byte_pool, &_can_outgoing_thread), U_SUCCESS);      // Create CAN Outgoing thread.
    CATCH_ERROR(_create_thread(byte_pool, &_sensors_thread), U_SUCCESS);           // Create Sensor thread.
    CATCH_ERROR(_create_thread(byte_pool, &_gpio_lights_thread), U_SUCCESS);       // Create GPIO Lights thread.
    // add more threads here if necessary

    DEBUG_PRINTLN("Ran threads_init().");
    return U_SUCCESS;
}
