#ifndef __U_FAULTS_H
#define __U_FAULTS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Determine whether the CAN message means the car is faulted
 * @param fault_message the fault CAN message
 * @return whether car is faulted depending on the CAN message
 */
bool is_car_faulted(uint8_t fault_message[8]);

#endif