# ADR 0011: Cross-compilation for Instrument Cluster on Raspberry Pi

## Status  
Accepted

## Context  
The Instrument Cluster application must run on a Raspberry Pi (ARM64), while development occurs on x86_64 environments. Direct compilation is not feasible due to architecture mismatch.

## Decision  
We decided to adopt cross-compilation using Docker with `buildx` to build ARM64 binaries from x86_64 environments. This approach allows developers to build and prepare deployable artifacts without needing ARM hardware.

## Consequences  
- Enables faster and platform-independent builds.
- Requires maintaining a custom Docker image with the necessary build tools.
- Developers must ensure correct environment configuration (e.g., credentials, SSH access).

Implementation details and operational steps are documented in the [project deployment guide in README file](RaspberryPi/README.md).
