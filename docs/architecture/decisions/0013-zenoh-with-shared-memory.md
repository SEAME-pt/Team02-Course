# ADR 0013: Use of Zenoh with Shared Memory for Intra-Host Communication

## Status
Accepted

## Context
Our system uses Zenoh as a data-centric communication layer for real-time data exchange between distributed components. Many of these components are co-located on the same physical machine, particularly in edge computing and robotics scenarios.

Traditional communication methods (e.g., TCP or UDP over loopback) introduce unnecessary serialization, copying, and latency when processes are on the same host. To improve performance, Zenoh supports shared memory transport for intra-host communication.

One of the main scenarios where this is applied is on the **Raspberry Pi**, where:
- The **middleware** component publishes CAN bus values using Zenoh
- The **Qt application** running on the same device subscribes to these values

Using shared memory in this case improves responsiveness and reduces system resource usage, which is crucial on resource-constrained hardware like the Raspberry Pi.

## Decision
We decided to enable and use **Zenoh with shared memory transport** for communication between local components running on the same host.

This approach allows Zenoh participants (publishers and subscribers) to exchange data via shared memory, reducing latency and CPU overhead.

## Consequences
### Positive
- Significantly reduced latency for intra-host communication
- Lower CPU and memory usage due to zero-copy semantics
- Improved throughput for high-frequency data streams
- Better performance on resource-constrained devices (e.g., Raspberry Pi)

### Negative / Considerations
- Debugging shared memory issues can be more complex
- Fallback mechanisms should be in place if shared memory is not available

## Related
- Zenoh documentation: https://zenoh.io/docs