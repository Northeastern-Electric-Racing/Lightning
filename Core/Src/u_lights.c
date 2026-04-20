#include "u_lights.h"
#include "main.h"
#include "u_tx_debug.h"

/* Turns on the red light, and off the green light. */
void lights_setRed(void) {
    //PRINTLN_INFO("Lightning set to red light.");
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
}

/* Turns on the green light, and off the red light. */
void lights_setGreen(void) {
    //PRINTLN_INFO("Lightning set to green light.");
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
}

/* Sets all lights off. */
void lights_setOff(void) {
    //PRINTLN_INFO("Lightning set to lights off.");
    HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
}