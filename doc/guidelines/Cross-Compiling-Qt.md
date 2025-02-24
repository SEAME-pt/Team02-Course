# Cross-Compiling Qt for Raspberry Pi 4

This guide provides instructions for cross-compiling Qt for the Raspberry Pi 4 using Docker and QEMU. It includes two scripts:
1. `buildQTLinux.sh`: Installs Docker and QEMU to simulate the Raspberry Pi environment and builds the Docker image for cross-compiling Qt.
2. `deployQTApp.sh`: Uses the Docker image with the source code to compile the project and then sends it to the Raspberry Pi.

## Prerequisites

- A Debian-based system (e.g., Ubuntu)
- Docker installed on your system
- QEMU installed on your system

## Step 1: Install Docker and QEMU

```bash
chmod +x buildQTLinux.sh && \
sudo ./buildQTLinux.sh
```

## Step 2: Build the Qt APP


## Step3: Compile and send it to the raspberry pi 4

```bash
chmod +x deployQTApp.sh && \
./deployQtApp.sh $PWD Cluster
```

