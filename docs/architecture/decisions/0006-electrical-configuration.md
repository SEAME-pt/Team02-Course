@page architecture_decisions6 6. Electrical Configuration
Date: 2024-12-18

# ADR 0006: Electrical Configuration

## Status
Accepted

## Context
The current electrical design of the Jetracer car is insufficient due to the excessive power consumption of all the equipment present. The power board from the Jetracer has a maximum current supply for the 5V circuit of 3A. The Jetson Nano alone can consume up to 3A while running the GPU. Additionally, we need to power the servomotor of the car, an additional Raspberry Pi, an Arduino, and a screen connected to the Raspberry Pi.

## Decision
To address the power supply issue, we decided to add an extra power supply. This consists of three components: a battery stack with three batteries, a battery protection system (XY-0001 BMS), and a DC-DC converter 5V - 6A (JOY-IT SBC-BUCK04-5V).

## Consequences

1. The additional power supply will ensure that all components receive adequate power, preventing potential power shortages.
2. The battery protection system will safeguard the batteries from overcharging and discharging, enhancing the overall safety and longevity of the power system.
3. The DC-DC converter will provide a stable 5V output with a higher current capacity, ensuring reliable operation of all connected devices.
4. The inclusion of extra components may increase the overall weight and complexity of the electrical system.


<img src="../diagrams/Electrical Upgrade Diagram.drawio.png" alt="Architecture Overview" style="max-width:80%; height:auto;">