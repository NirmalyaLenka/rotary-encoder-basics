# ESP32 Rotary Encoder Test

## Overview

This project reads rotation direction and button input from a KY-040 rotary encoder connected to an ESP32. It tracks a counter that increments clockwise and decrements counter-clockwise. Pressing the built-in button resets the counter to zero. All output is printed to the Serial Monitor.

---

## Hardware Required

- ESP32 development board
- KY-040 rotary encoder module (or equivalent)
- Jumper wires
- USB cable for power and programming

---

## Pin Connections

| Encoder Pin | ESP32 Pin | Description            |
|-------------|-----------|------------------------|
| VCC         | 3.3V      | Power supply           |
| GND         | GND       | Ground                 |
| CLK         | GPIO 25   | Clock signal input     |
| DT          | GPIO 26   | Data signal input      |
| SW          | GPIO 27   | Push button input      |

Note: The KY-040 module has onboard pull-up resistors for CLK and DT. The SW pin is handled with INPUT_PULLUP in code. Do not supply more than 3.3V to any pin.

---

## How It Works

A rotary encoder outputs two square wave signals, CLK and DT, that are 90 degrees out of phase with each other. By comparing the state of CLK at the moment it changes to the current state of DT, the code can determine which direction the knob is being turned.

When CLK changes state and DT does not match CLK, the rotation is clockwise and the counter increments. When DT matches CLK, the rotation is counter-clockwise and the counter decrements.

The button on the encoder is active LOW. When pressed, it pulls the SW pin to GND. The code detects a falling edge on SW and resets the counter to zero, with a 300ms debounce delay to prevent multiple triggers from a single press.

---

## Direction Logic

| CLK Changed | DT != CLK | Direction         |
|-------------|-----------|-------------------|
| Yes         | Yes       | Clockwise         |
| Yes         | No        | Counter-clockwise |

---

## Setup Steps

1. Wire the rotary encoder to the ESP32 as shown in the pin table.
2. Open the Arduino IDE and make sure ESP32 board support is installed.
3. Open the rotary_encoder_esp32.ino file.
4. Select your ESP32 board from Tools > Board.
5. Select the correct COM port from Tools > Port.
6. Upload the sketch.
7. Open Serial Monitor at 115200 baud.
8. Rotate the knob and press the button to see output.

---

## Expected Serial Output

```
Direction: CLOCKWISE
Counter: 1
Direction: CLOCKWISE
Counter: 2
Direction: COUNTER-CLOCKWISE
Counter: 1
Button Pressed! Counter Reset.
Counter: 0
```

---

## Troubleshooting

- If the counter jumps by more than one per click, reduce the delay at the bottom of the loop or add software debouncing on CLK and DT.
- If the direction is always reported incorrectly, swap the CLK and DT pin connections or reverse the comparison logic in the code.
- If the button does not register, confirm SW is connected to GPIO 27 and that INPUT_PULLUP is set in the code.
- If no output appears at all, verify VCC is connected to 3.3V and GND is common between the encoder and ESP32.

---

## Possible Projects

**Volume or Brightness Controller**
Use the encoder knob to increase or decrease audio volume or screen brightness on a connected device. The counter value maps directly to a percentage. Pressing the button can mute or toggle the display on and off.

**OLED Menu System**
Pair the encoder with an SSD1306 OLED display. Rotating the knob scrolls through menu items and pressing the button selects the highlighted option. This is a clean way to build a standalone device interface without a touchscreen.

**Stepper Motor Position Control**
Each encoder tick moves a stepper motor one step in the corresponding direction. This gives precise manual control over a motor axis, useful for CNC positioning, focus adjustments on a camera rail, or tuning a mechanical fixture.

**Digital Potentiometer**
Replace a physical potentiometer in a circuit with the encoder plus ESP32. The counter value is mapped to a PWM output or sent over I2C to a digital potentiometer IC, giving smooth stepped control over voltage or resistance.

**Settings Panel for IoT Device**
Use the encoder to adjust parameters on a connected IoT device such as temperature setpoints, timer durations, or WiFi channel selection. Values are saved to EEPROM or NVS flash so they persist after power off.

**Rotary Combination Lock**
Build a combination lock using the encoder. The user must rotate to specific counter values in a set sequence. If the sequence matches, an output pin triggers a relay or servo to unlock a latch.

**Frequency or Parameter Tuner**
Feed the counter value into a formula to tune a frequency output on a DAC pin or a DDS module. Pressing the button can switch between coarse and fine tuning steps, making it easy to dial in precise values.

**Scroll Wheel for Custom Keyboard**
Register the ESP32 as a USB HID device using the TinyUSB library. Map encoder rotation to scroll up and scroll down events. Pressing the button can act as a middle-click, giving you a compact custom input device for a PC or Mac.

---

## License

This project is released for open use. No restrictions apply.


---

## Maintenance & Audit

- **Audit date:** 10 November 2026
- **Documentation status:** README reviewed and audit record added on this date.
- **Scope and evidence:** This entry records a documentation-level maintenance pass. It does not by itself establish that source code compiles, dependencies are current, security has been tested, or hardware/runtime behavior has been verified.
- **Code and tests:** No new functional code changes or test results are claimed by this README update. Run the project-specific build, test, and hardware/runtime checks before relying on it.
- **Maintenance baseline:** Documentation status recorded through 10 November 2026; revisit dependencies, setup instructions, known limitations, and tests whenever the project changes.
