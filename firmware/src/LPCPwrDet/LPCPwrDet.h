
#ifndef LPCPwrDetH
#define LPCPwrDetH

#include "pico/stdlib.h"

void    LPCPwrDetInit(uint8_t pin);
bool    LPCPwrDetGetNewValue(bool* value_out);
bool    LPCPwrDetGetValue();

#endif
