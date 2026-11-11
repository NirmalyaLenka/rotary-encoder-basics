# ESP32 Rotary Encoder Basics

[![Platform](https://img.shields.io/badge/Platform-ESP32-00979D?logo=espressif&logoColor=white)](https://www.espressif.com/en/products/socs/esp32)
[![Language](https://img.shields.io/badge/Language-Arduino%20C%2B%2B-00599C?logo=cplusplus&logoColor=white)](https://docs.arduino.cc/)

A minimal Arduino-framework example for reading a KY-040-style rotary encoder on an ESP32. The sketch polls the CLK and DT signals, updates a signed counter based on direction, and resets the counter when the encoder's push button is pressed. Events and the counter value are printed to the Serial Monitor.

This is a learning and bench-test example. It is **not** an interrupt-driven or fully debounced encoder driver.

## Features

- Reads CLK and DT as digital inputs.
- Increments or decrements a counter using a simple quadrature-direction check.
- Uses the encoder push button to reset the counter to zero.
- Reports direction, counter value and reset events over serial.
- Uses a 5 ms polling delay and a 300 ms button debounce delay.

## Hardware and bill of materials

| Item | Status | Notes |
|---|---|---|
| ESP32 development board | Required | Arduino-compatible ESP32 board with GPIO 25, 26 and 27 available. |
| KY-040 rotary encoder module | Required for the documented wiring | Confirm the pin labels on the specific module. |
| Jumper wires | Required | Share ground between module and ESP32. |
| USB data cable | Required | For programming, power and serial output. |
| External pull-ups | Conditional | The sketch does not enable internal pull-ups for CLK/DT. Use a module with onboard pull-ups or add suitable pull-ups to 3.3 V if needed. |

Power the encoder module from **3.3 V** for this wiring so its signal lines do not pull ESP32 GPIOs above their safe voltage. Do not connect 5 V logic to ESP32 pins.

## Pin connections

| KY-040 pin | ESP32 pin | Purpose |
|---|---|---|
| VCC / + | 3V3 | Module supply |
| GND | GND | Common ground |
| CLK | GPIO 25 | Clock signal |
| DT | GPIO 26 | Direction/data signal |
| SW | GPIO 27 | Active-low push button |

The source uses `INPUT` for CLK and DT and `INPUT_PULLUP` for SW. Therefore, CLK/DT need valid external or module-provided biasing; SW is expected to be pulled HIGH when idle and LOW when pressed.

## How the sketch works

1. On startup, it configures the GPIOs and records the initial CLK state.
2. In the main loop, it compares the current CLK state with the previous state.
3. Whenever CLK changes, it samples DT to choose whether to increment or decrement the counter.
4. It detects a HIGH-to-LOW transition on SW, resets the counter, prints a message and waits 300 ms.
5. It waits another 5 ms before polling again.

### Important behavior and limitations

- The current code reacts to **both rising and falling CLK transitions**. Depending on the encoder's detent/pulse pattern, a physical click may produce more than one counter update. The exact count-per-click has not been calibrated for a particular KY-040 module.
- CLK/DT have no software debounce or full quadrature state validation. Contact bounce can create extra counts, and rapid rotation may be missed by polling.
- The 300 ms button delay blocks encoder polling while it runs.
- The sketch resets the counter but does not persist it across power cycles.
- Direction labels may be reversed depending on encoder wiring and the module's mechanical orientation; swap CLK/DT or reverse the direction branch if required.

## Setup and upload

1. Wire the module using the pin table and verify that its signal pins are safe for 3.3 V ESP32 logic.
2. Install the Arduino IDE and ESP32 board support.
3. Because Arduino sketches normally need a folder matching the `.ino` filename, place the source in a folder named `rotary_encoder_esp32/` containing `rotary_encoder_esp32.ino` before compiling.
4. Open the sketch, select the exact ESP32 board under **Tools → Board**, and select its serial port under **Tools → Port**.
5. Upload the sketch.
6. Open **Serial Monitor** at **115200 baud**.
7. Rotate the knob slowly in both directions, then press and release SW. Observe whether direction and count match the behavior expected from your module.

## Example serial output

Output format varies with rotation and button presses. A typical sequence is:

```text
Direction: CLOCKWISE
Counter: 1
Direction: COUNTER-CLOCKWISE
Counter: 0
Button Pressed! Counter Reset.
```

The counter value shown is illustrative, not a hardware test result.

## Troubleshooting

| Symptom | Checks |
|---|---|
| No serial output | Confirm the selected port, upload result and 115200 baud setting. |
| Counter changes without touching the knob | Check common ground, CLK/DT wiring and pull-ups; contact bounce can cause extra transitions. |
| Multiple counts per click | Expected with the current both-edge polling approach on some encoder variants. A falling-edge-only or full quadrature decoder may be needed for one count per detent. |
| Direction is reversed | Swap CLK and DT or invert the direction decision after confirming wiring. |
| Button does not reset the counter | Confirm SW is connected to GPIO 27 and is active LOW; check that the module ground is common with the ESP32. |
| ESP32 resets or GPIOs appear damaged | Disconnect immediately and verify the module is not pulling signal pins to 5 V. |

## Repository layout

```text
.
├── rotary_encoder_esp32.ino
└── README.md
```

There are no separate libraries, build scripts, CI workflows or automated tests in the repository tree. The sketch uses the Arduino core APIs and has no additional third-party library dependency.

## License and maintenance

- **License:** no `LICENSE` file is present, so reuse and redistribution permissions are not formally declared. Add a license file to state explicit terms.
- **Verification:** source-level review only. Compilation and physical encoder behavior have not been verified for this maintenance pass.
- **Last updated:** 11 November 2026 — documentation, wiring safety, setup and limitation notes reviewed.
- **Maintenance:** if pins, polling logic, debounce behavior or count semantics change, update this guide and validate the result on the intended encoder module.
