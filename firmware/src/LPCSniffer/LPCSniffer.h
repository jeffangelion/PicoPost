
#ifndef LPCSnifferH
#define LPCSnifferH

#include "pico/stdlib.h"

void    LPCGPIOInit(uint8_t pinF, uint8_t pinC);
bool    LPCInit();
bool    LPCTryGetValue(uint8_t* value_out);
bool    LPCTryGetNewValue(uint8_t* value_out);

#endif
