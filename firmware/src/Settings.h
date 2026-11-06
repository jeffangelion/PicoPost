
#ifndef SettingsH
#define SettingsH

// Set the system frequency to 33 MHz x 6 to make time for handling LPC frames.
#define SYS_FREQ_IN_KHZ (198 * 1000)

// LAD[0-3] + LCLK + LFRAME, which starts from GPIO0.
#define LPC_BUS_PIN_BASE    9U
#define LPC_BUS_PIN_COUNT   6U

#define LPC_PWRON_PIN       2U

// #define IM_ALIVE_PRINT
// #define IM_ALIVE_LED_PIN    25U

// POST UART
#define PUART_ID         uart0
#define PUART_BAUDRATE   115200
#define PUART_TX         0U
#define PUART_RX         1U
// USB  UART
#define UUART_ID         uart1
#define UUART_BAUDRATE   115200
#define UUART_TX         8U
#define UUART_RX         21U

// Display MODEL
// #define DISPLAY KL2821B
// #define DISPLAY E20281I
// #define DISPLAY E20281L
#define DISPLAY _2281BY
#if defined(DISPLAY)
#include "Display/Display.h"
// #define DISPLAY_TEST
// #define FREEZE_LAST_POST
// Display brightness 0x00-0xFF
#define DISPLAY_BRIGHTNESS  (0xFF / 2)
#define DISPLAY_SEGTIME_US  500
//      Model   = { comA ,SC,CC, { A, B, C, D, E, F, G,DP,D2,D1} }
Display _2281BY = { false, 8, 2, {29,15, 7, 6, 5, 4,26,27,28, 3} };
// Display KL2821B = { true , 8, 2, {29,15, 7, 6, 5, 4,26,27,28, 3} };
Display KL2821B = { true , 8, 2, {28,15, 7, 6, 5, 3,26,27,29, 4} };
Display E20281I = { true , 8, 2, { 7, 3,15,28,29,26, 5, 6,27, 4} };
Display E20281L = { false, 8, 2, { 7, 3,15,28,29,26, 5, 6,27, 4} };
// Display E20281L = { false, 8, 2, { 7, 4,15,29,28,26, 5, 6,27, 3} };
#endif

#endif
