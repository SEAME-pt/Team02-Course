@page architecture_decisions9 9. Remote Cluster
Date: 2023-10-05

# ADR 0009: Remote Cluster

## Status
Accepted

## Context
In addition to the in-place cluster attached to our Jetson Nano, we aim to establish a remote cluster. This remote cluster will provide real-time access to most of the same information available on the in-place cluster. Additionally, it will offer insights into specific aspects such as lane and object detection.

## Decision
We will implement a remote cluster that mirrors the data from the in-place cluster on the Jetson Nano. This remote cluster will be designed to handle and display real-time data, with a particular focus on lane and object detection.

## Consequences
- **Positive**: 
    - Enhanced monitoring capabilities with real-time data access from a remote location.
    - Improved analysis of lane and object detection data.
    - Increased flexibility in accessing and processing data.

- **Negative**:
    - Additional resources required for setting up and maintaining the remote cluster.
    - Potential latency issues in real-time data synchronization.


## References
- [Jetson Nano Documentation](https://developer.nvidia.com/embedded/jetson-nano)
