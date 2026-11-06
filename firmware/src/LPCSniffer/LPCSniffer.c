
#include "LPCSniffer.h"
#include "hardware/pio.h"
#include "lpc_bus_sniffer.pio.h"
#include <stdio.h>

static  uint8_t PinF;
static  uint8_t PinC;

static  PIO     lpc_pio     = pio0;
static  int     sm_lpc_bus_sniffer;

static  uint8_t valueOld    = 0x00;

typedef union __attribute__((__packed__)) {
  struct {
    uint16_t addr   : 16;
    uint8_t  cy_dir : 4;
    uint8_t  start  : 4;
    uint16_t resv   : 8;
  } val;
  uint32_t value;
} LPCIOFrame;

void LPCGPIOInit(uint8_t pinF, uint8_t pinC) {
    PinF = pinF;
    PinC = pinC;
    for(uint8_t i = PinF; i < PinF + PinC; i++) {
        // Init the LAD[0-3] + LCLK + LFRAME pins.
        gpio_init(i);
        // Disable internal pull up and downs.
        gpio_disable_pulls(i);
    }
}

uint32_t reverse_nibbles(uint32_t in_val) {
    uint32_t out_val = 0;

    for(uint8_t i = 0; i < 32; i += 4) {
        out_val <<= 4;
        out_val |= in_val >> i & 0xf;
    }
    return out_val;
}

int init_lpc_bus_sniffer(PIO pio) {
    uint offset;

    #if defined(DEBUG_LOG)
    printf("Initializing the lpc bus sniffer program\n");
    #endif
    int sm = pio_claim_unused_sm(pio, true);
    if(sm < 0){
        #if defined(DEBUG_LOG)
        printf("Error: Cannot claim a free state machine\n");
        #endif
        return -1;
    }

    if (pio_can_add_program(pio, &lpc_bus_sniffer_program)) {
        offset = pio_add_program(pio, &lpc_bus_sniffer_program);
    } else {
        #if defined(DEBUG_LOG)
        printf("Error: pio program can not be loaded\n");
        #endif
        return -2;
    }

    lpc_bus_sniffer_program_init(pio, sm, offset, PinF, PinC);
    pio_sm_set_enabled(pio, sm, true);

    // Set the I/O RW frame filter
    LPCIOFrame filter; 
    filter.value    = 0x00000000;
    // Set I/O port address  (POST codes)
    filter.val.addr     = 0x0080;
    // Set Cycle/Dir (write: 0x2, read: 0x0)
    filter.val.cy_dir   = 0x02;
    // Set Start
    filter.val.start    = 0x00;
    
    // Print filter (0x08002000)
    #if defined(DEBUG_LOG)
    printf("Filter: 0x%08x\n", reverse_nibbles(filter.value));
    #endif

    pio->txf[sm] = reverse_nibbles(filter.value);

    #if defined(DEBUG_LOG)
    printf("Program loaded at %d, sm: %d\n", offset, sm);
    #endif
    return sm;
}

bool LPCInit() {
    // Init the PIO
    sm_lpc_bus_sniffer = init_lpc_bus_sniffer(lpc_pio);
    return (sm_lpc_bus_sniffer >= 0);
}

bool LPCTryGetValue(uint8_t* value_out) {
    if(!pio_sm_is_rx_fifo_empty(lpc_pio, sm_lpc_bus_sniffer)) {
        *value_out = lpc_pio->rxf[sm_lpc_bus_sniffer];
        return true;
    }
    return false;
}

bool LPCTryGetNewValue(uint8_t* value_out) {
    if(LPCTryGetValue(value_out) && *value_out != valueOld) {
        valueOld = *value_out;
        return true;
    }
    return false;
}
