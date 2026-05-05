# INF1900 — Autonomous Line-Following Robot

> Embedded systems project — Polytechnique Montréal  
> Microcontroller: **ATmega324PA** @ 8 MHz | Language: **C++ (AVR)**

---

## Overview

Fully autonomous robot that navigates a predefined map without human intervention. The robot exits a parking spot, travels through corridors, detects debris and damage on the floor, inspects rooms (classrooms and storage rooms), and returns to its original parking spot — all driven by a finite state machine running on bare metal.

---

## Hardware

| Component | Pin(s) | Description |
|---|---|---|
| Line sensors (×5) | PA0–PA5 | Detect black lines and damage zones on the floor |
| Distance sensor | PA5 | Detects obstacles/people in rooms |
| Left motor (PWM) | PD4, PD6 | Speed and direction control |
| Right motor (PWM) | PD5, PD7 | Speed and direction control |
| Piezo buzzer | PB2, PB3 | Audio feedback and alert sequences |
| Green/Red LED | PC2, PC3 | Visual state feedback |
| Interrupt button | PD2 | Mode selection trigger |
| EEPROM (I2C) | PC0 (SCL), PC1 (SDA) | Persistent instruction storage |
| UART | PD0 (RXD), PD1 (TXD) | Debug communication |
| SPI programmer | PB4–PB7 | Flashing firmware (disconnect motors first) |

### Full Pinout

```
PORTA            PORTB          PORTC          PORTD
┌───────┐        ┌──────┐       ┌──────┐       ┌──────┐
│ PA0 ●─│→S1     │PB0 ○ │       │PC0 ●─│→SCL   │PD0 ●─│→RXD
│ PA1 ●─│→S2     │PB1 ○ │       │PC1 ●─│→SDA   │PD1 ●─│→TXD
│ PA2 ●─│→S3     │PB2 ●─│→P-GND │PC2 ●─│→LED-G │PD2 ●─│→Button
│ PA3 ●─│→S4     │PB3 ●─│→P-PWM │PC3 ●─│→LED-R │PD3 ○ │
│ PA4 ●─│→S5     │PB4 ●─│→PG+   │PC4 ○ │       │PD4 ●─│→PWM-L
│ PA5 ●─│→DS     │PB5 ●─│→PG+   │PC5 ○ │       │PD5 ●─│→PWM-R
│ PA6 ○ │        │PB6 ●─│→PG+   │PC6 ○ │       │PD6 ●─│→DIR-L
│ PA7 ○ │        │PB7 ●─│→PG+   │PC7 ○ │       │PD7 ●─│→DIR-R
└───────┘        └──────┘       └──────┘       └──────┘

● = USED    ○ = UNUSED
```

---

## How It Works

### 3 Operating Modes

The robot boots into a **mode selection** screen driven by the interrupt button and LEDs:

| Mode | Description |
|---|---|
| **Instruction** | Receive mission parameters via UART (parking spot, direction) and store to EEPROM |
| **Execution** | Load instructions from EEPROM and run the full autonomous mission |
| **Report** | Transmit mission results back via UART |

---

### Autonomous Navigation — State Machine

The execution mode runs a finite state machine that drives the robot through the full course:

```
INIT
  │
  ▼
SOUTH_TO_CORRIDOR ──(intersection detected)──▶ IN_CORRIDOR
                                                    │
                          ┌─────────────────────────┘
                          │
                    (corridorCount = 0)
                          │
                          ▼
                      NORTH_WALL
                          │
                    ┌─────┴──────┐
                    │            │
              (room entrance) (intersection)
                    │            │
                    ▼            ▼
                ROOM_TASK    IN_CORRIDOR
                    │            │
                    └────────────┘
                          │
                    (corridorCount = 1)
                          │
                          ▼
                  SOUTH_TO_PARKING
                          │
                          ▼
                    ENTER_PARKING
                          │
                          ▼
                         DONE
```

| State | Trigger | Action |
|---|---|---|
| `INIT` | — | `exitParking()` |
| `SOUTH_TO_CORRIDOR` | Intersection detected | `followOneLine()` + `turnIntersection()` |
| `IN_CORRIDOR` | Intersection + corridorCount = 0 | `followTwoLines()` + `inspectCorridor()` |
| `IN_CORRIDOR` | Intersection + corridorCount = 1 | `followTwoLines()` → south to parking |
| `NORTH_WALL` | Room entrance detected | `followOneLine()` |
| `NORTH_WALL` | Intersection detected | `followOneLine()` + `turnIntersection()` |
| `ROOM_TASK` | — | `enterRoom()` + `evacuateRoom()` or `inspectStorageRoom()` |
| `ENTER_PARKING` | — | `enterParking()` |
| `DONE` | — | Stop |

---

### Line Following

- **Single line** (`followOneLine`): S3 (center) stays on black; S1/S5 used for correction
- **Two lines** (`followTwoLines`): S1 and S5 each track one wall line; robot stays centered
- **Damage detection**: when **S1 and S5 both detect black simultaneously**, a wide damage zone is identified — the robot continues straight and logs the finding

### Room Inspection

**Classroom** — `evacuateRoom()`:
1. Robot scans with distance sensor (rotates to face South, East, West)
2. If an obstacle (person) is detected → plays alert sequence until it moves
3. Counts evacuated people and flashes green LED for each one

**Storage room** — `inspectStorageRoom()`:
1. Robot scans with distance sensor
2. Detects and reports debris presence without evacuation

---

## Software Architecture

```
projet/
├── lib/
│   ├── motor.cpp / motor.h          # DC motor control (PWM via Timer1)
│   ├── sensor.cpp / sensor.h        # Line sensors + distance sensor (ADC)
│   ├── sound.cpp / sound.h          # Piezo buzzer (PWM)
│   ├── navigation.cpp / navigation.h # High-level movement logic
│   ├── execution.cpp / execution.h  # State machine
│   ├── instruction.cpp / instruction.h # Instruction mode
│   ├── bytecode_interpreter.cpp     # EEPROM read/write
│   └── ...
└── app/
    └── main.cpp                     # Entry point + mode selector
```

Key design choices:
- **Modular library**: each subsystem (motor, sensor, sound) is independently testable
- **Non-blocking methods**: navigation functions yield control back to the state machine each iteration
- **Filtered ADC readings**: distance sensor uses exponential smoothing (`90% old + 10% new`) to reduce noise

---

## Build & Flash

```bash
# Compile
make

# Flash to robot
# ⚠️ Disconnect motors before flashing (PB4–PB7 shared with SPI programmer)
make install

# Clean build artifacts
make clean
```

> **Note:** The robot waits 3 seconds after boot before starting — use this time to place it correctly on the track.

---

## Dependencies

- AVR-GCC toolchain (`avr-gcc`, `avr-objcopy`)
- `avrdude` for flashing
- `simavr` headers (for simulation)
- Standard AVR libs: `<avr/io.h>`, `<util/delay.h>`, `<avr/interrupt.h>`
