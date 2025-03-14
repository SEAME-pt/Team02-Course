@page architecture_decisions10 10. System Control Diagram defined for Lane Detection
Date: 2024-12-18

# ADR 0010: System Control Diagram defined for Lane Detection

## Status
Accepted
## Context
The control system for lane detection in the vehicle initiates with a lane detection algorithm that defines the lane in which the vehicle will circulate. This lane detection algorithm is obtained by testing and training various algorithms designed for lane detection, based on concepts of segmentation, anchors, and parameter definition.

## Decision
To address the lane detection and control system, we decided to implement a multi-step process involving lane detection, centerline projection, trajectory definition, and control behavior determination. The chosen methods include:

1. Lane detection using a trained algorithm based on segmentation, anchors, and parameter definition.
2. Centerline projection using inverse perspective mapping.
3. Trajectory definition using polynomial fitting, with optional noise reduction and data stabilization via a Kalman filter.
4. Control behavior determination using model predictive control (MPC) or alternative methods like pure pursuit controller.

## Consequences

1. Improved accuracy in lane detection and vehicle control due to the use of advanced algorithms and mathematical models.
2. Enhanced stability and noise reduction in trajectory definition through the use of a Kalman filter.
3. Flexibility in control behavior determination, allowing for the selection of the most suitable control method based on specific requirements.
4. Potential increase in computational complexity and resource requirements due to the implementation of multiple advanced algorithms and control methods.


<img src="../diagrams/Control_diagram_LaneDetection.jpg" alt="Control Diagram" style="max-width:100%; height:auto;">

