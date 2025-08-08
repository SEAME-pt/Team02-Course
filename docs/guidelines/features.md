# 🚗 JetRacer Driving Features

This document showcases all the autonomous features developed for the JetRacer, each illustrated with a short demo GIF.

---

## 🔁 Adaptive Cruise Control (ACC)

The car maintains a safe distance from the vehicle ahead by adjusting its speed automatically.

<p align="center">
  <img src="../videos/acc.gif" alt="ACC Demo" width="600"/>
</p>

---

## 🛣️ Lane Keeping Assist System (LKAS)

Keeps the car centered in its lane using lane detection and steering control.

<p align="center">
  <img src="../videos/LKAS.gif" alt="LKAS Demo" width="600"/>
</p>

<p align="center">
  <img src="../videos/LKAS-car.gif" alt="LKAS Demo" width="600"/>
</p>
---

## 🚶 Crosswalk Speed Reduction

Automatically reduces speed when approaching a pedestrian crosswalk.

<p align="center">
  <img src="../videos/crosswalk-speed-reduction.gif" alt="Crosswalk Speed Reduction Demo" width="600"/>
</p>

---

## 🛑 Stop Sign Detection

Detects stop signs and makes a full stop before continuing.

<p align="center">
  <img src="../videos/stop-sign.gif" alt="Stop Sign Demo" width="600"/>
</p>

---

## 🚦 Traffic Light Handling

Recognizes traffic light colors and responds accordingly (stop on red, go on green).

<p align="center">
  <img src="../videos/traffic-lights.gif" alt="Traffic Lights Demo" width="600"/>
</p>

---

## 🚧 Obstacle Avoidance

Avoids static or dynamic obstacles on the road while keeping the lane.

<p align="center">
  <img src="../videos/obstacle-avoidance.gif" alt="Obstacle Avoidance Demo" width="600"/>
</p>

---

## 🆘 Emergency Braking

Performs an emergency stop when a sudden obstacle is detected.

<p align="center">
  <img src="../videos/emergency-breaking.gif" alt="Emergency Braking Demo" width="600"/>
</p>

---

## ⛔ Trajectory Feasibility Check

Prevents the car from taking a path that is geometrically impossible or dangerous.

<p align="center">
  <img src="../videos/trajectory-impossible.gif" alt="Trajectory Feasibility Demo" width="600"/>
</p>

---

## 🔒 Safety Conditions

Ensures basic safety rules are always respected.

<p align="center">
  <img src="../videos/safety-conditions.gif" alt="Safety Conditions Demo" width="600"/>
</p>

---

## 📌 Notes

- All features were tested in a controlled environment using the JetRacer.
- CAN protocol was used for communication between perception, control modules and UI instrument cluster.
