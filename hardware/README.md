# PicoPost Hardware

This folder contains all KiCad design files, schematics, fabrication gerbers, and documentation for the PicoPost PCB.

## Files & Directories

- `kicad/`: Complete KiCad 7 / 8 project:
  - `PicoPost.kicad_pro` — Project file
  - `PicoPost.kicad_sch` — Root schematic sheet
  - `PicoPost.kicad_pcb` — Board layout
  - `7S2D.kicad_sch` — 7-Segment display subsystem
  - `SoM.kicad_sch` — RP2040-Zero controller subsystem
  - `LPC.kicad_sch` — LPC header connector
  - `UART.kicad_sch` — UART header
  - `LPC_ASUS_10Pin.kicad_sch`, `LPC_GIGABYTE_12Pin.kicad_sch`, `eLPC_ASUS_12Pin.kicad_sch` — Vendor-specific LPC breakouts
  - `Library.kicad_sym`, `Library.pretty/` — Project footprints and symbols
  - `PinHeader_2x05_P2.54mm_Vertical.step` — 3D model
- `gerbers/`:
  - `PicoPost_Gerbers.zip` — Ready to upload to JLCPCB, PCBWay, etc.
  - Extracted `.gbr` and `.drl` files.
- `enclosure/`:
  - `PicoPost_Case.step` — 3D printable protective enclosure model.
- `PicoPost_Schematic.pdf` — Complete multi-page schematic in PDF format.

## PCB Specs
- **Dimensions**: ~18.4 mm × 23.5 mm (ultra-compact)
- **Layers**: 2 Layers (FR-4, 1.6mm or 1.0mm thickness recommended)
- **Finish**: HASL lead-free or ENIG (recommended for castellated soldering)
- **Solder Mask**: Black, Green, Purple, etc.
