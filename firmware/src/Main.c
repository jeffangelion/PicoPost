
#pragma region      INCLUDE
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"

#include "LPCSniffer/LPCSniffer.h"
#include "LPCPwrDet/LPCPwrDet.h"
#include "Display/Display.h"
#include "Display/DisplayTest.h"
#include "Settings.h"
#pragma endregion   INCLUDE

#pragma region      CODE
#pragma region      INIT
void GPIOInit() {
    LPCGPIOInit(LPC_BUS_PIN_BASE, LPC_BUS_PIN_COUNT);
    #if defined(LPC_PWRON_PIN)
    LPCPwrDetInit(LPC_PWRON_PIN);
    #endif

    #if defined(IM_ALIVE_LED_PIN)
    gpio_init(IM_ALIVE_LED_PIN);
    gpio_set_dir(IM_ALIVE_LED_PIN, GPIO_OUT);
    #endif

    #if defined(DISPLAY)
        #if defined(DISPLAY_BRIGHTNESS)
            DisplayInit(&DISPLAY, true);
            #if defined(DISPLAY_TEST)
                DisplayBrght(0x00);
            #else
                DisplayBrght(DISPLAY_BRIGHTNESS);
            #endif
        #else
            DisplayInit(&DISPLAY, false);
        #endif
    #endif
}

void StdioInit() {
    #if LIB_PICO_STDIO_UART
    #if defined(PUART_ID) && defined(PUART_BAUDRATE) && defined(PUART_TX) && defined(PUART_RX)
    stdio_uart_init_full(PUART_ID, PUART_BAUDRATE, PUART_TX, PUART_RX);
    #else
    stdio_uart_init();
    #endif
    #endif
    #if LIB_PICO_STDIO_SEMIHOSTING
    stdio_semihosting_init();
    #endif
    #if LIB_PICO_STDIO_USB
    stdio_usb_init();
    #endif
}

void UartInitFull(struct uart_inst* uart, uint baud_rate, int tx_pin, int rx_pin) {
    if(tx_pin >= 0) { gpio_set_function((uint)tx_pin, GPIO_FUNC_UART); }
    if(rx_pin >= 0) { gpio_set_function((uint)rx_pin, GPIO_FUNC_UART); }
    uart_init(uart, baud_rate);
}

#if defined(LPC_PWRON_PIN) && !defined(DISPLAY_TEST)
bool PostEn = false;
#else
bool PostEn = true;
#endif
#pragma endregion   INIT

#if defined(IM_ALIVE_PRINT) || defined(IM_ALIVE_LED_PIN)
bool AliveTimerCallback(struct repeating_timer* segTimer) {
    // Print the I'm alive sign every 1 sec.
    #if defined(IM_ALIVE_PRINT)
    printf(".");
    #endif
    #if defined(IM_ALIVE_LED_PIN)
    gpio_put(IM_ALIVE_LED_PIN, !gpio_get(IM_ALIVE_LED_PIN));
    #endif
    return true;
}
#endif

uint8_t PostRaw    = 0x00;
#if defined(DISPLAY)
bool DispTimerCallback(struct repeating_timer* segTimer) {
    if(DisplayUpdate()) {
        if(PostEn) {
            #if defined(DISPLAY_TEST)
            if(DisplayTestUpdate(&PostRaw)) {
                DisplaySetByteHex(PostRaw);
                #if defined(DISPLAY_BRIGHTNESS)
                DisplayBrght(PostRaw);
                #endif
            }
            #else
            DisplaySetByteHex(PostRaw);
            #endif
        } else {
            #if !defined(FREEZE_LAST_POST)
            PostRaw = 0x00;
            DisplayClear();
            #endif
        }
    }
    return true;
}

void Core1Entry() {
    #if defined(DISPLAY_TEST)
    DisplayTestInit();
    #endif

    struct repeating_timer segTimer;
    add_repeating_timer_us(DISPLAY_SEGTIME_US, DispTimerCallback, NULL, &segTimer);
    while(true) {
        uart_putc_raw(UUART_ID, getchar());
    }
}
#endif

int main() {
    // Initialize the GPIOs. 
    GPIOInit();
    StdioInit();
    UartInitFull(UUART_ID, UUART_BAUDRATE, UUART_TX, UUART_RX);
    // Set the system frequency.
    set_sys_clock_khz(SYS_FREQ_IN_KHZ, true);

    #if defined(LPC_PWRON_PIN) && !defined(DISPLAY_TEST)
    PostEn = LPCPwrDetGetValue();
    #endif
    // If powered not from LPC
    if(!PostEn) {
        // Waiting for USB CDC connection or timeout.
        for(int i = 0; ((i < (1000 / 50)) && !stdio_usb_connected()); i++) {
            sleep_ms(50);
        }
        sleep_ms(100);
    }
    #if defined(DEBUG_LOG)
    printf("STARTED\n");
    #endif

    if(!LPCInit()) {
        return -1;
    }

    #if defined(DEBUG_LOG)
    // Print the system frequency.
    printf("System clock is %u kHz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
    printf("Starting to sample the POST codes.\n");
    #endif

    // Let the user know initialization was successful and the program is going to start.
    // Set the repeating timer for I'm alive sign.
    #if defined(IM_ALIVE_PRINT) || defined(IM_ALIVE_LED_PIN)
    struct repeating_timer aliveTimer;
    add_repeating_timer_ms(1000, AliveTimerCallback, NULL, &aliveTimer);
    #endif

    #if defined(DISPLAY)
    multicore_launch_core1(Core1Entry);
    #endif

    while(true) {
        #if defined(LPC_PWRON_PIN) && !defined(DISPLAY_TEST)
        if(LPCPwrDetGetNewValue(&PostEn)) {
            printf(PostEn ? "PON\n" : "POF\n");
        }
        #endif
        if(PostEn && LPCTryGetNewValue(&PostRaw)) {
            printf("#%02x\n", PostRaw);
        }
    }
}
#pragma endregion   CODE
