#include <stdlib.h>

#include "u_test.h"

void gpio_test() {
    int value = rand() % 3;
    int status = set_statemachine((Lightning_Board_Light_Status) value);

    if (status != U_SUCCESS) {
        PRINTLN_INFO("Failed to set State in GPIO Test");
    }
}