# ADR 0005: CAN Communication Between Jetson Nano and Raspberry Pi

## Context
To ensure effective and reliable communication between the Jetson Nano and Raspberry Pi in our system, we need a robust communication protocol. CAN (Controller Area Network) is a well-established protocol designed for reliable communication in embedded systems, especially in environments with potential electrical noise.

## Decision
We have decided to use CAN communication as the primary method for data exchange between the Jetson Nano and Raspberry Pi.

## Rationale
- **Reliability**: CAN is designed for high-reliability communication in embedded systems.
- **Noise Resistance**: CAN is robust against electrical noise, making it suitable for our environment.
- **Scalability**: CAN allows for easy addition of other devices to the network in the future.
- **Real-Time Communication**: CAN supports real-time data exchange, which is critical for our application.

## Consequences
- **Positive**: 
    - Improved communication reliability and robustness.
    - Simplified integration of additional devices if needed.
- **Negative**:
    - Additional hardware (CAN transceivers) are required.

## Alternatives Considered
- **Ethernet/Wi-Fi**: These options were considered but were overkill for the simplicity and real-time requirements of our system.

## Implementation
- Use CAN transceivers to enable communication between the Jetson Nano and Raspberry Pi.
- Configure the CAN bus with appropriate baud rates and termination resistors.
- Develop software drivers or use existing libraries to handle CAN communication.

## Status
Accepted