
#include "Display.h"
#include "../Macros.h"
#include "hardware/pwm.h"

static  Display*    Disp;
static  uint8_t     Brght   = 0xFF;

static  uint8_t     Font[]  = {
//    .GFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
};

static  uint8_t segNum      = 0;
static  uint8_t digNum      = 0;
static  uint8_t segNumOld   = 0;
static  uint8_t digNumOld   = 0;
static  uint8_t dispBuff[]  = { 0xFF, 0xFF };

static  bool    PWMEN;

void DisplayInit(Display* disp, bool pwmEn) {
    Disp = disp;
    PWMEN = pwmEn;
    for(uint8_t i = 0; i < Disp->segC; i++) {
        gpio_init(Disp->pins[i]);
        gpio_set_dir(Disp->pins[i], GPIO_OUT);
        gpio_put(Disp->pins[i], 0);
        gpio_set_outover(Disp->pins[i], Disp->comA ? GPIO_OVERRIDE_INVERT : GPIO_OVERRIDE_NORMAL);
    }
    for(uint8_t i = Disp->segC; i < (Disp->segC + Disp->comC); i++) {
        if(PWMEN) {
            gpio_set_function(Disp->pins[i], GPIO_FUNC_PWM);
            uint8_t slice = pwm_gpio_to_slice_num(Disp->pins[i]);
            pwm_set_clkdiv(slice, 1.0f);
            pwm_set_wrap(slice, 0xFF);
            pwm_set_enabled(slice, true);
            pwm_set_gpio_level(Disp->pins[i], 0x00);
        } else {
            gpio_init(Disp->pins[i]);
            gpio_set_dir(Disp->pins[i], GPIO_OUT);
            gpio_put(Disp->pins[i], 0);
        }
        gpio_set_outover(Disp->pins[i], Disp->comA ? GPIO_OVERRIDE_NORMAL : GPIO_OVERRIDE_INVERT);
    }
}
void DisplayBrght(uint8_t brght) {
    Brght = brght;
}

bool DisplayUpdateSymbol(uint8_t symbol) {
    gpio_put(Disp->pins[segNumOld], 0);
    gpio_put(Disp->pins[segNum], ((symbol >> segNum) & 0b1));
    segNumOld = segNum;
    segNum = ((Disp->segC > (segNum + 1)) ? (segNum + 1) : 0);
    return (segNum == 0);
}

bool DisplayUpdate() {
    if(PWMEN) {
        pwm_set_gpio_level(Disp->pins[Disp->segC + digNumOld], 0);
        pwm_set_gpio_level(Disp->pins[Disp->segC + digNum], Brght);
    } else {
        gpio_put(Disp->pins[Disp->segC + digNumOld], 0);
        gpio_put(Disp->pins[Disp->segC + digNum], 1);
    }
    if(DisplayUpdateSymbol(
            ((dispBuff[digNum] < ARRAYSIZE(Font)) ? Font[dispBuff[digNum]] : 0b00000000)
          | ((digNum == 0) << 7)
        )) {
        digNumOld = digNum;
        digNum = ((Disp->comC > (digNum + 1)) ? (digNum + 1) : 0);
    }
    return (digNum == 0);
}

void DisplayClear() {
    dispBuff[0] = 0xFF;
    dispBuff[1] = 0xFF;
}

void DisplaySetByteHex(uint8_t byte) {
    dispBuff[0] = ((byte >> (4 * 0)) & 0b1111);
    dispBuff[1] = ((byte >> (4 * 1)) & 0b1111);
}
