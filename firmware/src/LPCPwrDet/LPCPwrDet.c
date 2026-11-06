
#include "LPCPwrDet.h"

static  uint8_t Pin;
static  bool    valueOld    = false;

void LPCPwrDetInit(uint8_t pin) {
    Pin = pin;
    gpio_init(Pin);
    gpio_set_dir(Pin, GPIO_IN);
    gpio_pull_down(Pin);
}

bool LPCPwrDetGetNewValue(bool* value_out) {
    *value_out = gpio_get(Pin);
    if(*value_out != valueOld) {
        valueOld = *value_out;
        return true;
    }
    return false;
}

bool LPCPwrDetGetValue() {
    return gpio_get(Pin);
}
