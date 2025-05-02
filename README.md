# Team02 Autonomous Vehicle Platform

This repository contains the complete software stack for our autonomous vehicle platform, organized into specialized submodules.

## Overview

Our autonomous vehicle platform integrates multiple computing systems, sensors, and machine learning models to provide a comprehensive solution for autonomous driving research and development. The system architecture includes:

- Jetson Nano for main vehicle control and autonomous capabilities
- Raspberry Pi for instrumentation cluster and user interface
- Arduino microcontrollers for sensor input and actuator control
- Machine learning models for perception tasks
- Dataset collection and management tools

## Repository Structure

```
Team02-Course/
├── JetsonNano/         # Main vehicle control software
├── RaspberryPi/        # Instrument cluster and UI
├── MicroController/    # Arduino code for sensors/actuators
├── MachineLearning/    # ML models for perception
├── Dataset/            # Dataset management and generation
└── README.md           # This documentation file
```

## Components

### JetsonNano

Contains all the code running on the Jetson Nano, which serves as the main computing platform for the vehicle. Includes:

- Vehicle control systems
- Sensor integration
- Communication frameworks
- Autonomous driving capabilities
- Hardware abstraction layers

### RaspberryPi

Contains code for the Raspberry Pi, which manages the instrument cluster and user interfaces:

- Dashboard displays
- System status monitoring

### MicroController

Arduino-based software for hardware-level interactions:

- Speed sensor data collection
- Lighting control systems
- Real-time sensor processing

### MachineLearning

Machine learning models and algorithms for autonomous perception:

- Lane detection models
- Object detection and classification

### Dataset

Tools and resources for dataset management:

- Local track dataset collection tools
- Carla simulator dataset utilities
- Dataset preprocessing pipelines
- Future: Code for generating synthetic datasets using Carla

## Getting Started

Each submodule contains its own documentation with specific setup instructions. To clone the repository with all submodules:

```bash
git clone --recursive https://github.com/yourusername/Team02-Course.git
```
