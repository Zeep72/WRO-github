# Zorrobots #9164

### WRO Future Engineers 2026 — Engineering Portfolio

---

## Table of Contents

- [Introduction](#introduction)
  - [Mission](#mission)
  - [Vision](#vision)
  - [Team History](#team-history)
  - [We Inspire Youth](#we-inspire-youth)
- [Engineering Process](#engineering-process)
  - [Initial Brainstorming & First Design Decisions](#initial-brainstorming--first-design-decisions)
  - [Drivetrain](#drivetrain)
  - [Steering System](#steering-system)
  - [First Autonomous Movement](#first-autonomous-movement)
  - [Major Redesign](#major-redesign)
  - [Final Build](#final-build)
- [Electronics](#electronics)
- [Color Sensor & Distance Sensing](#color-sensor--distance-sensing)
- [Software Architecture](#software-architecture)
- [Control System](#control-system)
- [6-State Machine](#6-state-machine)
- [Obstacle Detection System](#obstacle-detection-system)
- [Strategy](#strategy)
  - [Navigation & Lap Management](#navigation--lap-management)
  - [Traffic & Obstacle Handling](#traffic--obstacle-handling)
  - [Round 2 & Parking Strategy](#round-2--parking-strategy)
  - [Fail-Safe System](#fail-safe-system)

---

## Introduction

### Mission

Zorrobots is a team passionate about innovation, engineering, and community impact. We designed, built, and programmed an autonomous vehicle to compete in **WRO Future Engineers 2026**, applying STEM principles, teamwork, and creative problem-solving **entirely from scratch, without commercial kits**.

Our mission goes beyond the competition field — we aim to inspire others through outreach, community connections, and accessible educational initiatives. Through engineering, programming, and experimentation, we develop solutions while sharing our knowledge with our community.

### Vision

Our vision is to become a catalyst for STEM education in Baja California, inspiring and empowering students to explore science, technology, engineering, and mathematics.

Access to quality STEM education remains limited for many students in our region, making it difficult for them to develop the skills needed in a rapidly evolving technological world. As a team, we aim to reduce this gap by promoting hands-on learning, sharing knowledge, and creating experiences that encourage future generations to explore STEM fields.

### Team History

Our story began in **September 2014**, when FTC team The Ducks #8174 introduced FIRST Robotics at our school. A group of students became interested, found mentors, and created a team to compete in *Cascade Effect*.

During that first season, we proudly represented our state as the **first team in our region to reach the World Championship**. More than a decade later, that same spirit of curiosity, teamwork, and perseverance continues to drive us in WRO Future Engineers 2026.

### We Inspire Youth

Zorrobots is more than a robotics team — it is a movement that encourages young minds to innovate, learn, and lead.

Through experiences in engineering, programming, and community outreach, we promote interest in STEM and demonstrate how technology can be created through creativity, dedication, and teamwork.

---

## Engineering Process

### Initial Brainstorming & First Design Decisions

When we first reviewed the WRO Future Engineers 2026 challenge, we knew our approach would be different from the beginning: **no commercial kits, only recycled and repurposed materials**.

Our first discussions focused on the drivetrain, because every other subsystem depended on having a reliable chassis. We decided to use motors from old RC toy cars, parts from unused electronic kits, and mechanical components from our school's robotics laboratory.

Once we had a drivetrain concept, we focused on the perception system. Our first idea was to use computer vision with an **ESP32-CAM** to detect the red and green traffic pillars, which allowed us to explore image processing and autonomous detection.

However, during testing, we identified limitations with the camera system:
- Detection distance was very short
- Recognition became unreliable when the robot was moving
- Recognition became unreliable when the pillars were farther away

Because of these limitations, we decided to replace the camera with a **dedicated color sensor**, improving detection speed, reliability, and consistency. These decisions established the foundation for our final design.

### Drivetrain

Our first prototype focused only on the drive system. We selected **rear-wheel drive** because we believed it would provide better control for obstacle avoidance in future iterations.

The chassis was built using metal laboratory parts, and propulsion came from a simple two-gear system: one gear connected to the rear axle and another driven by a motor recovered from a disassembled RC toy car.

At this stage, the robot had no steering system. The objective was to verify that our recycled-parts drivetrain could generate consistent forward movement.

### Steering System

With the drivetrain working, we focused on developing the steering mechanism.

Inspired by RC car designs that use separate motors for propulsion and steering, we attempted to replicate this concept. We built a front axle using the same recycled metal components and added a second motor to control direction.

Finding a reliable steering method was our biggest challenge during this stage. We tested different solutions, including strings, gears, and multiple mechanical configurations, but none provided consistent results.

Wheel dimensions also created difficulties, and the robot exceeded the **300 mm width limitation**. Despite these problems, this iteration helped us understand the improvements needed for the next design.

### First Autonomous Movement

To solve the steering problems from the previous iteration, we replaced the motor-based steering system with a **goBILDA 300-degree torque servo**. This change provided immediate improvement in turning consistency and control.

We also completed our first electrical connections by integrating an **L298N motor driver** for the drive motor and connecting the servo directly to an **Arduino UNO**. This was the first version of our robot capable of autonomous movement.

During this iteration, we used the experience gained to improve the component layout and reduce the overall size of the robot. However, the chassis still exceeded the 300 mm width limit.

The remaining steering problems were no longer related to the mechanism itself, but to the chassis structure — some bolts and external hardware around the front wheels limited the turning angle and affected performance.

### Major Redesign

This iteration represented one of our biggest improvements.

We completely replaced the wheels, which solved the turning restrictions caused by the previous hardware configuration. The steering mechanism was also simplified by connecting it directly to the goBILDA servo, reducing friction points and improving mechanical reliability.

On the electronics side, we added a **7.4V battery** to power the Arduino UNO, motor driver, and sensors. A separate **6V battery** was dedicated to the steering servo to prevent voltage drops during sharp turns.

We also installed an **infrared distance sensor** at the front of the robot to improve obstacle awareness. By the end of this iteration, the robot finally met the 300 mm width requirement — a major milestone in our development process.

### Final Build

For our final iteration, we focused on improving the robot's detection system and overall reliability.

Initially, we mounted an ESP32-CAM at the front of the robot to perform vision-based pillar detection. Although the camera allowed us to experiment with image processing, testing showed that the system had limitations: detection range was too short, and color recognition became inconsistent when the robot was moving or when lighting conditions changed.

To solve this problem, we replaced the camera with a **dedicated color sensor**, providing faster and more stable detection of the red and green pillars and allowing the robot to make autonomous decisions with greater reliability.

The color sensor was integrated with the Arduino UNO and combined with the distance sensor to create a more efficient obstacle detection system.

We also relocated the drive motor to the rear of the chassis to create more space for the batteries and improve weight distribution. Finally, we added a voltage booster for the main motor and completed cable management, resulting in a cleaner, more reliable, and competition-ready robot.

---

## Electronics

Our electronics system is built around **two separate power lines** to isolate sensitive components from motor noise.

| Component | Voltage | Purpose |
|---|---|---|
| Drive motor system | 7.4V LiPo | Feeds the L298N motor driver, which controls the main drive motor |
| XL6009 boost converter | — | Placed between the L298N output and the motor to maintain consistent torque, since the drive motor requires higher voltage than the driver's output provides |
| Steering servo (goBILDA) | 6V (dedicated) | Prevents voltage drops during sharp turns from affecting the rest of the system |

---

## Color Sensor & Distance Sensing

The **Arduino UNO** is the main controller of the robot. It manages motor direction through the L298N, servo steering, color detection, and distance measurement.

During early development, an ESP32-CAM was used for pillar recognition. However, after testing, we replaced it because camera detection was limited to very short distances and produced inconsistent results during movement.

The new color sensor provides direct color readings to the Arduino UNO, allowing the robot to quickly identify red and green pillars and execute the correct avoidance maneuver.

The distance sensor complements the color sensor by detecting obstacles and providing information about the robot's surroundings. Together, both sensors create a more reliable autonomous navigation system.

---

## Software Architecture

The autonomous system is divided into two main components:

1. **Arduino UNO** — controls movement and decision-making
2. **Color sensor** — provides real-time information about the detected pillars

After replacing the ESP32-CAM, the perception system became simpler and more reliable. Instead of processing images, the robot receives direct color information from the sensor, reducing processing time and improving detection consistency.

The color sensor continuously analyzes the environment and sends the detected color information to the Arduino UNO. The Arduino combines this information with the distance measurements from the sensor to determine the correct avoidance action.

---

## Control System

The Arduino UNO acts as the main controller and manages all autonomous behaviors:

- Motor control through the L298N driver
- Steering control through the goBILDA servo
- Distance measurements through the obstacle detection sensor
- Color recognition through the color sensor
- Execution of the autonomous navigation algorithm

The robot follows a **state-based control system** that allows it to scan, navigate, avoid obstacles, return to position, and complete the challenge autonomously.

---

## 6-State Machine

```
ST_SCAN -> ST_LAP -> ST_TURN -> ST_AVOID -> ST_HOME -> ST_DONE
```

| State | Description |
|---|---|
| **ST_SCAN** | The robot rotates 360° while measuring distances to understand its surroundings and establish an initial reference of the environment. The color sensor remains active during this process to identify possible pillar colors. |
| **ST_LAP** | The robot moves forward while continuously monitoring the distance sensor and color sensor. If an obstacle is detected, the system uses the color information to determine the correct direction for avoidance. |
| **ST_TURN** | The robot performs a controlled turn to complete a lap around the central track area. After completing the required number of laps, the system transitions to the next stage. |
| **ST_AVOID** | When an obstacle is detected, the robot analyzes both sensors: the distance sensor detects the object, the color sensor determines whether it's red or green, and the Arduino executes the corresponding steering action — **left for green**, **right for red**. If no color is detected, the robot follows a default safety maneuver to prevent collisions. |
| **ST_HOME** | After completing the required laps, the robot uses its navigation data to return to the starting area, controlling speed and direction to achieve a controlled finish. |
| **ST_DONE** | Once the challenge is completed, the robot stops all motors and maintains its final position. |

---

## Obstacle Detection System

Obstacle detection combines information from both sensors. The distance sensor identifies physical objects in front of the robot, while the color sensor provides additional information to classify the obstacle.

This combination improves decision-making because the robot does not rely only on visual detection or distance measurements.

The previous camera-based system required image processing and was limited by short detection distances. The new color sensor provides faster responses and more stable detection during autonomous movement.

---

## Strategy

Our strategic approach for the WRO Future Engineers 2026 competition focuses on **consistency, reliability, and autonomous decision-making**.

Instead of prioritizing maximum speed, our goal is to achieve controlled movements, accurate obstacle avoidance, and reliable runs. The replacement of the camera system with a dedicated color sensor improved the robot's ability to react quickly and consistently during the competition.

### Navigation & Lap Management

The robot combines sensor information to estimate its position and maintain its route inside the track.

Lap tracking is managed through the combination of distance measurements, movement timing, and autonomous decision-making. The system recognizes completed rotations around the central track area and uses this information to determine lap progress.

### Traffic & Obstacle Handling

The robot processes color information in real time to execute predefined overtaking maneuvers.

| Obstacle | Action |
|---|---|
| Green Pillar | Turns **left** to avoid the obstacle and continue along the optimal racing path |
| Red Pillar | Turns **right** to bypass the obstacle while maintaining its trajectory |
| Unknown Obstacle | If the distance sensor detects an object but the color sensor cannot identify a color, the robot performs a cautious default avoidance maneuver |

### Round 2 & Parking Strategy

For advanced challenges, the robot continues using active sensing to locate the parking area after completing the required laps. The system performs controlled deceleration and uses navigation adjustments to achieve a precise final position within the designated parking area.

### Fail-Safe System

To improve reliability during unexpected situations, the robot includes an **autonomous recovery routine**. If the system detects that it is not progressing correctly, the Arduino activates a recovery sequence: the robot reverses, reassesses the environment using its sensors, and continues movement once a clear path is detected.

---

**Zorrobots #9164** · WRO Future Engineers 2026
