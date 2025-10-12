#include <stdint.h>

#include "main.h"
#include "as3935.h"

as3935_t *as3935;

void init_lightning_sensor(SPI_HandleTypeDef *hspi) {
    as3935 = malloc(sizeof(as3935_t));
    as3935_init(as3935, &hspi, AS3935_INT_GPIO_Port, AS3935_INT_Pin);
}

uint16_t read_lightning_sensor() {
    // TODO: implement later
    return 0;
}

uint16_t read_imu() {
    // TODO: implement later
    return 0;
}

uint16_t read_magnetometer() {
    // TODO: implement later
    return 0;
}