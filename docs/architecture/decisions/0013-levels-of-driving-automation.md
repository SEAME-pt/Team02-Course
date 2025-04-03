@page architecture_decisions13 13. Record architecture decisions
Date: 2024-04-03

# ADR 0012: Levels of Driving Automation

## Status
Accepted

## Context
With the purpooise of studying of the variuous levels of automation in the car, it makes sense to define what should be demonstrated in the car. This levels can be also shown in a demonstration using Carla.

## Decision
We have decided to use define 3 levels of SAE. Ideally, leve 4 SAE will be implemented by the end of object detection module.
 
For SAE1 (Driver Assistance):
- Lane Departure Warning/Correction: The system monitors lane markings and alerts or gently corrects the vehicle if it begins to drift out of the lane.
- Adaptive Cruise Control (Longitudinal Control): The vehicle adjusts speed based on obstacles. It can stop if there is an obstacle in the way, the user can takeover and force acceleration after initial brake warning. The vehicle maintains a set speed or safe distance from the car ahead, adjusting speed automatically.

For SAE2 (Partial Automation):
- The system simultaneously keeps the vehicle centered in its lane (lateral control) and manages speed/spacing with other vehicles (longitudinal control), control is made while seeing both lines, and without additional traffic. In the absense of lanes, the car prompts the user to take over. If not responding the car breaks. This should be performed in a straight line and slight curves.

For SAE3 (Conditional Automation):
- The system takes full control of the car and manages sterring, braking and acceleration in the lanes including tight corners, it does not consider any additional information besides lines and stops in the event of an obstacle in the way.

For SAE4 (limited implementation)
- The vehicle can navigate, make decisions, and handle all driving functions (steering, accelerating, braking, etc.) without human input as long as it remains within its predefined operational boundaries (such as a specific urban area, highway segment, or geofenced environment).  If the vehicle detects that conditions are moving outside its ODD, it’s designed to transition to a safe state (such as pulling over safely or stopping). We have no way to define end-point so the car is not necessarily in SAE4, this level expects the use of GPS and other sensors to position the car within the operational area. We consider now that we can stop at traffic lights and go around small objects on the road as a way to show some of the capabilities in this level.


## Consequences
- **Pros:**
1. Incremental Development: Implementing automation in levels allows for gradual development and testing without overwhelming the system
2. Clear Feature Boundaries: Each SAE level has well-defined capabilities which focuses development efforts
3. Safety-First Approach: Starting with lower levels of automation provides a foundation for safety-critical features
4. Educational Value: Demonstrates the progression of autonomous technology in a structured manner
5. Industry Alignment: Follows standardized SAE classifications recognized throughout the automotive industry
6. Demonstration Flexibility: Different levels can be showcased separately during presentations
7. Modular Testing: Each level can be tested and validated independently
- **Cons:**
1. Mode Transition Challenges: Managing safe transitions between automation levels adds complexity
2. Increasing Complexity: Implementation complexity grows exponentially with each automation level

## References
- [SAE J3016 Levels of Driving Automation] https://www.sae.org/standards/content/j3016_202104/