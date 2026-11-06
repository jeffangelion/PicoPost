
#ifndef DisplayH
#define DisplayH

#include "pico/stdlib.h"

typedef struct {
    bool    comA;
    uint8_t segC;
    uint8_t comC;
    uint8_t pins[];
}   Display;

void    DisplayInit(Display* disp, bool pwmEn);
void    DisplayBrght(uint8_t brght);
bool    DisplayUpdate();
void    DisplayClear();
void    DisplaySetByteHex(uint8_t byte);

#endif
