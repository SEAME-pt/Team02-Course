@page architecture_decisions2 2. Use Jetson Nano for Processing and Raspberry Pi for Display in Car Cluster
Date: 2024-12-18

# ADR 0002: Use Jetson Nano for Processing and Raspberry Pi for Display in Car Cluster

## Status
Accepted

## Context
We need to create a car cluster in our project. We are using a Jetson Nano to handle all the math and processing tasks. The data will be sent via CAN bus to the Raspberry Pi, which will be used solely for rendering the QT image on the display.

## Decision
We will use the Jetson Nano for all processing tasks and the Raspberry Pi for display rendering.

## Consequences

1. The Jetson Nano will be dedicated to handling all processing tasks, which may improve performance and reduce latency.
2. The Raspberry Pi will handle the display rendering, which may simplify the display connection and reduce the load on the Jetson Nano.
3. This separation of concerns may lead to a more modular and maintainable system.
4. Potential increase in complexity due to the need for communication between the Jetson Nano and Raspberry Pi.