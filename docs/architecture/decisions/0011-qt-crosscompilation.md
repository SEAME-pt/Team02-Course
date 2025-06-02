# ADR 0011: Cross-compilation for Instrument Cluster on Raspberry Pi  

## Status  
Accepted  

## Context  
The Instrument Cluster application needs to run on a Raspberry Pi, which uses an ARM64 architecture. However, the development environment is typically on a non-ARM64 architecture (e.g., x86_64). To address this, cross-compilation is used to build the application for the target architecture.  

## Decision  
We decided to use a cross-compilation approach to build the Instrument Cluster application for the Raspberry Pi. This involves using Docker with platform emulation (via `buildx`) when building on non-ARM64 architectures.  

The process includes:  
1. Building the application using a custom Docker image (`ruipires99/raspcrosscompile:latest`) that contains the necessary tools for cross-compilation.  
2. Copying the compiled binaries from the Docker container.  
3. Transferring the binaries to the Raspberry Pi using SCP.  
4. Restarting the necessary services on the Raspberry Pi to run the updated application.  

## Prerequisites  
For the cross-compilation process to work seamlessly, ensure that the development environment has the correct credentials configured. These credentials are typically stored in a `.env` file. This includes:  
- SSH keys set up for secure access to the Raspberry Pi.  
- Proper permissions to use Docker and `buildx` for cross-compilation.  
- Access to the private repository (if applicable) containing the source code or dependencies.  

Make sure the `.env` file inside RaspberryPi repository is properly set up and sourced before starting the process. Without these credentials, the process may fail at various stages, such as transferring binaries or accessing required resources.  

The script used for this process can be accessed [here](RaspberryPi/deploy/scripts/deployToRasp.sh):  

The Dockerfile used for cross-compilation can be accessed [here](RaspberryPi/deploy/dockerfiles/DockerfileDeployRasp)  

This approach ensures that the Instrument Cluster application can be efficiently built and deployed to the Raspberry Pi.  