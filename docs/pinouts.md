# PicoPost Pinouts & Wiring Guide

## LPC Bus Interface Pinouts

PicoPost captures BIOS POST codes sent to I/O port `0x80` over the LPC (Low Pin Count) bus at 33 MHz.

### Signals Required for POST Sniffing:
- **LAD[0..3]**: 4-bit multiplexed command, address, and data bus.
- **LCLK**: 33 MHz LPC bus clock from the chipset/Super I/O.
- **LFRAME#**: Frame indicator signal (active low).
- **GND**: Common ground between motherboard and PicoPost.
- **3.3V (PWRON)**: 3.3V power and/or power detection line.

---

### 1. Standard LPC Header (2x5 Pin, 2.54mm) — J3 on PCB

```
           +-----------+
  LPC.AD0  |  1     2  |  +3.3V
  LPC.AD1  |  3     4  |  KEY (no pin)
  LPC.AD2  |  5     6  |  LPC.CLK
  LPC.AD3  |  7     8  |  GND
 LPC.FRAME#|  9    10  |  KEY (no pin)
           +-----------+
```

---

### 2. ASUS LPC Header (10-pin)

Found on many ASUS desktop motherboards with TPM / LPC debug header:

| Pin | Signal | PicoPost Connection |
|:---:|:------:|:-------------------:|
| 1 | +3.3V | 3.3V / PWRON |
| 2 | +3.3V | (tied to 3.3V) |
| 3 | LPC.AD0 | GP9 |
| 4 | LPC.AD1 | GP10 |
| 5 | LPC.AD2 | GP11 |
| 6 | LPC.AD3 | GP12 |
| 7 | LPC.CLK (33 MHz) | GP13 |
| 8 | LPC.FRAME# | GP14 |
| 9 | GND | GND |
| 10 | KEY / NC | — |

---

### 3. Gigabyte LPC Header (12-pin, 2x6)

| Pin | Signal | PicoPost Connection |
|:---:|:------:|:-------------------:|
| 1 | +3.3V | 3.3V / PWRON |
| 2 | +3.3V | (tied to 3.3V) |
| 3 | LPC.AD0 | GP9 |
| 4 | LPC.AD1 | GP10 |
| 5 | LPC.AD2 | GP11 |
| 6 | LPC.AD3 | GP12 |
| 7 | LPC.CLK | GP13 |
| 8 | LPC.FRAME# | GP14 |
| 9 | GND | GND |
| 10 | GND | GND |
| 11 | KEY | — |
| 12 | NC | — |

---

### 4. ASUS eLPC Header (12-pin, 1x12 single row)

Found on modern ASUS motherboards:

| Pin | Signal | PicoPost Connection |
|:---:|:------:|:-------------------:|
| 1 | +3.3V | 3.3V / PWRON |
| 2 | LPC.AD0 | GP9 |
| 3 | +3.3V | 3.3V |
| 4 | LPC.AD1 | GP10 |
| 5 | +3.3V | 3.3V |
| 6 | LPC.AD2 | GP11 |
| 7 | LPC.AD3 | GP12 |
| 8 | GND | GND |
| 9 | LPC.FRAME# | GP14 |
| 10 | GND | GND |
| 11 | LPC.CLK | GP13 |
| 12 | GND | GND |

---

## RP2040-Zero Pin Mapping (U1)

| RP2040 Pin | Function | Description |
|:---|:---|:---|
| **GP0** | PUART_TX | UART0 TX (115200 baud) for POST code logging |
| **GP1** | PUART_RX | UART0 RX |
| **GP2** | PWRON | LPC Power Detect input (connect to +3.3V LPC) |
| **GP3** | 7S2D Segment | Display Segment control (D1/DP/etc depending on model) |
| **GP4** | 7S2D Segment | Display Segment F |
| **GP5** | 7S2D Segment | Display Segment E |
| **GP6** | 7S2D Segment | Display Segment D |
| **GP7** | 7S2D Segment | Display Segment C |
| **GP8** | UUART_TX | UART1 TX for USB/PC serial bridge |
| **GP9** | LPC.AD0 | LPC Bus Data 0 |
| **GP10** | LPC.AD1 | LPC Bus Data 1 |
| **GP11** | LPC.AD2 | LPC Bus Data 2 |
| **GP12** | LPC.AD3 | LPC Bus Data 3 |
| **GP13** | LPC.CLK | LPC Bus 33 MHz Clock |
| **GP14** | LPC.FRAME# | LPC Bus Frame signal |
| **GP15** | 7S2D Segment | Display Segment B |
| **GP21** | UUART_RX | UART1 RX for USB/PC serial bridge |
| **GP26** | 7S2D Segment | Display Segment G |
| **GP27** | 7S2D Segment | Display Decimal Point / Segment DP |
| **GP28** | 7S2D Digit | Common Cathode/Anode Digit 2 |
| **GP29** | 7S2D Digit | Common Cathode/Anode Digit 1 |

---

## UART Header (J2)

Standard 1x5 pin header (2.54mm pitch):

1. **3V3**: 3.3V Rail
2. **TXD**: GP0 (POST Code UART Output)
3. **GND**: Ground
4. **RXD**: GP1 (UART Input)
5. **VBUS**: 5V from USB
