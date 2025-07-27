# Vulcan
Decrypting and reverse engineering propreitory Advanced Fire Alarm Polling Protocols.
# Project Vulcan: Advanced Fire Alarm Polling Protocols

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Status: R&D](https://img.shields.io/badge/status-research_%26_development-red.svg)](https://github.com/)

An open-source initiative dedicated to researching, developing, and reverse-engineering the communication protocols of addressable fire alarm systems. This project aims to create some of the world's fastest and most robust polling algorithms for life-safety applications.

---

## ## About The Project

Commercial addressable fire alarm systems rely on sophisticated, proprietary protocols to communicate with hundreds of sensors over a single two-wire loop. This repository documents the journey of decrypting these methods and engineering new, high-performance open-source alternatives.

We explore both classic and modern techniques, from software-defined FSK (Frequency-Shift Keying) to the robust current/voltage modulation methods used in professional-grade systems. The goal is to push the boundaries of speed and reliability for microcontroller-based polling networks.

---

## ## Key Research & Development Areas

* **Software-Defined FSK:** Complete implementation of an FSK transmitter and receiver using the Goertzel algorithm on ESP32, including protocol-level robustness features.
* **Current & Voltage Modulation:** Design and implementation of a production-level communication scheme based on industry standards, using voltage dips for panel-to-sensor commands and current pulses for sensor-to-panel responses.
* **Hardware Abstraction:** Development of coupling and decoupling circuits to enable reliable communication over a 12V DC power line.
* **Microcontroller Analysis:** Comparative analysis of different MCU architectures for this task, including:
    * **ESP32:** For rapid prototyping and its powerful dual-core architecture.
    * **ATtiny85 / PIC16F:** Analyzing their role as ultra-low-power, cost-effective sensor nodes.
    * **STM32 (G4/H7):** Research into high-performance ARM cores with FPU/DSP for achieving maximum polling speeds.
* **Protocol Optimization:** A deep dive into stability and speed, implementing features like DC bias calibration, preambles, and various timing delays to create a near-flawless communication link.

---

## ## Technology Stack

This project utilizes a combination of hardware and embedded software to achieve its goals.

* **Hardware Platforms:**
    * ESP32 Dev Boards
    * Arduino Nano / ATtiny85
    * Custom Analog Front-End (Op-Amps, MOSFETs, Filters)
* **Software & Algorithms:**
    * C++ (Arduino Framework)
    * Goertzel Algorithm for FSK Demodulation
    * Custom Pulse-Width & Current-Pulse Protocols

---

## ## Project Roadmap

-   [x] Develop and stabilize a software-defined FSK polling system. (Deprecated)
-   [ ] Design and build a robust current/voltage modulation system.
-   [ ] Design a custom PCB for the Panel and Sensor nodes.
-   [ ] Implement a fully non-blocking, interrupt-driven state machine for maximum efficiency.
-   [ ] Test the system with a large number of physical sensor nodes (100+).
-   [ ] Implement a more advanced data protocol with CRC error checking.

---

## ## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

---

## ## Acknowledgments

This project represents a deep exploration of embedded systems communication. Special thanks to all the resources and communities that make this research possible.
