### Wiring the display with Teensy: 2023.8.20

- The [instruction on the teensy website](https://www.pjrc.com/store/display_ili9341_touch.html) doesn't seem up to date for Teensy 4.1.
- Also refer to this [Arduino website](https://simple-circuit.com/interfacing-arduino-ili9341-tft-display/)
- The ILI9341 has a voltage regulator

| ILI9341 Pin | Teensy 4.1 Pin | Note                        |
| ----------- | -------------- | --------------------------- |
| VCC         | VIN            | Use the 3.3V pin next to 12, shared with Reset and the LED |
| GND         | GND            | Use the GND next to 0       |
| CS          | 10             |                             |
| RESET       | +3.3 V         | Share with VCC              |
| D/C         | 9              |                             |
| SDI (MOSI)  | 11 (DOUT)      |                             |
| SCK         | 13 (SCK)       |                             |
| LED         | VIN            | Share with VCC              |
| SDO (MISO)  | 12 (DIN)       |                             |
| T_CLK       | 13 (SCK)       | This and the following are related to the touch screen functions, and I didn't wire them at this moment |
| T_CS        | 8              |                             |
| T_DIN       | 11 (DOUT)      |                             |
| T_DO        | 12 (DIN)       |                             |
| T_IRQ       | 2              |
