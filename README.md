# ESP32-S3 IoT AC Voltage Controller

This repository contains the hardware design files, schematics, and firmware for a 4-zone, IoT-enabled 220V AC Voltage Controller built around the ESP32-S3. It is designed for smart apartment lighting dimming and fan motor speed control via Wi-Fi and the Blynk IoT platform.

## Hardware Overview

The board physically separates high-voltage 220V AC mains from the 3.3V DC logic utilizing strict PCB creepage rules and optical isolation.

* **Microcontroller:** ESP32-S3 with an onboard FT232RL USB-to-UART bridge for direct programming and debugging.
* **Power Supply:** HLK-PM01 AC-DC 5V step-down module paired with an AMS1117-3.3V linear regulator.
* **Zero-Crossing Detector (ZCD):** 50Hz mains synchronization using an EI-30 step-down transformer and a 4N25 optocoupler to trigger ESP32 hardware interrupts.
* **AC Switching:** BTA16-600B Triac (with heatsink) driven by a dual-trigger configuration (`IO6`, `IO7`) via parallel $330\,\Omega$ resistors and MOC3022 optoisolators.
* **Outputs:** 1 AC Mains input terminal and 4 independent AC Load output terminals for zoned apartment control.
* **Protection:** Main 10A glass fuse covering the entire board load (`PH_FUSED` net).

## PCB Layout and Safety Design

The PCB is designed with a primary focus on high-voltage safety, strict isolation boundaries, and RF performance.

* The top-down 3D render (`PCB0.jpg`) demonstrates the component arrangement, featuring the ESP32 module with a fully cleared antenna keepout area on the bottom left, alongside the FT232RL chip, micro-USB port, and tactile switches.


* The high-voltage terminals for AC MAINS and LOAD 1 through LOAD 4 are strictly grouped along the right edge of the board to isolate high-current wiring.


* An angled 3D view (`PCB1.jpg`) highlights the physical profile of the isolation components, showing the BTA16 Triac secured to a heatsink, the tall blue MOC3022 optocouplers, and the bulky EI-30 transformer and HLK-PM01 module bridging the AC and DC sections.


* A strict 6.0 mm unplated copper keepout trench is maintained underneath all isolating components to prevent high-voltage creeping.
* AC Live (Phase) and Neutral traces are routed exclusively on the X/Y plane without overlapping across layers. This prevents Z-axis "sandwiching" across the FR-4 core, eliminating parasitic capacitive coupling and dielectric stress.

## Firmware Modes

The repository includes two distinct C++ firmware implementations for the ESP32-S3. Both integrate with Blynk IoT via Wi-Fi, utilizing a single Virtual Pin (`V0`) linked to a mobile app slider or HTML web dashboard for 0–100% control.

1. **Phase Angle Control (Light Dimming):** Utilizes hardware timers to delay the Triac trigger pulse (500 µs to 9200 µs) after the zero-crossing edge, chopping the AC wave for smooth incandescent or dimmable LED lighting control.
2. **Integral Cycle Control (Fan/Motor Control):** Counts 100 AC half-cycles (1 second at 50Hz) and fires full sine waves proportionally to the target power percentage. This eliminates audible electrical motor humming and reduces harmonic distortion for inductive loads.
## 3D Views 
<img width="3645" height="2436" alt="PCB0" src="https://github.com/user-attachments/assets/bcb2a70d-8652-42f9-9dbd-2b78b61848b4" />
<img width="5955" height="3500" alt="PCB1" src="https://github.com/user-attachments/assets/0c35d5e4-fa9d-4ab3-b510-06b33bd8c903" />

## Disclaimer

**DANGER: HIGH VOLTAGE.** This project involves routing and switching 220V AC mains electricity, which is lethal. Do not build, test, or deploy this board unless you are experienced with high-voltage safety protocols. Always use a dim-bulb tester in series with the Live input for initial power-up testing.
