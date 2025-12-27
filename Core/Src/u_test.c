#include <stdlib.h>

#include "u_test.h"

void gpio_test() {
    uint8_t value = rand() % 3;
    uint8_t status = set_statemachine((Lightning_Board_Light_Status) value);

    if (status != U_SUCCESS) {
        PRINTLN_ERROR("Failed to set State in GPIO Test");
    }
}