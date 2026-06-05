Zorrobots FTC 9164 — Autonomous Vehicle Engineering — Full Development Report
WRO Future Engineers 2026


Project Overview

Zorrobots is a student robotics team competing in the WRO Future Engineers 2026 category under team number FTC 9164. The team designed, built, and programmed a fully autonomous vehicle from the ground up — with no commercial robotics kits involved. Every component was either salvaged from repurposed electronics, sourced from school lab supplies, or acquired individually. The project represents a complete engineering cycle: ideation, mechanical prototyping, electronics integration, software development, and iterative refinement under real competition constraints.

The challenge set by WRO 2026 Future Engineers requires the robot to navigate a closed track autonomously, complete three laps, identify and avoid colored traffic pillars (red and green), and return to its starting position. The robot must fit within a 300mm width limit and operate entirely without human intervention during a run. These constraints drove every major design and engineering decision throughout the project.

From the very beginning, the team's philosophy was rooted in resourcefulness and learning-by-doing. Rather than purchasing a ready-made platform, the members chose to understand every part of their machine — from the gear ratios in the drivetrain to the pixel thresholds in the camera detection algorithm. This decision made the process significantly harder, but also significantly more educational and authentic.


Development Phases

The robot's development was divided into eleven distinct phases, each addressing specific mechanical, electronic, or software challenges. The following sections describe each phase in detail, combining internal progress notes with the team's engineering portfolio documentation.


Phase 1 — Initial Prototype and Drivetrain Concept

The project began with a fundamental question: could a functional autonomous vehicle be built entirely from repurposed and recycled components? The first physical prototype was constructed using basic plastic parts and simple wheels. At this early stage, the team's primary goal was not to build a competition-ready robot, but rather to validate the concept of a recycled-parts drivetrain. The chassis was assembled using plastic structural elements, and propulsion relied on a motor salvaged from a disassembled RC toy car connected to the rear axle through a basic two-gear system.

The wheel system at this stage was extremely rudimentary — simple plastic wheels with no adequate traction or geometry for sharp turns. More critically, the front-wheel steering mechanism had not yet been implemented, making the prototype a straight-line runner only. Despite these limitations, this phase confirmed that the core drivetrain concept was viable: the salvaged motor could produce consistent forward motion, and the gear connection to the rear axle provided enough torque to move the chassis reliably.


Phase 2 — Structural Reinforcement and Motor Upgrade

With the drivetrain concept validated, the team's attention shifted to improving durability and power. The original plastic base was replaced with a chassis made from more rigid plastic materials capable of holding components in place under movement. The drive motor was also upgraded to a more powerful unit to support the increased weight of a four-wheeled build. Larger wheels were installed on the rear axle to improve ground contact and stability.

However, the front steering system remained unresolved. The team had a working rear drivetrain, but the robot still lacked any directional control. This phase was characterized by structural progress — a more solid foundation was established — while the steering problem was identified as the most critical challenge to address in upcoming iterations.


Phase 3 — Steering System Research and Prototyping

The team researched how modern RC cars handle steering, particularly newer-generation remote control vehicles that use dedicated front-wheel turning mechanisms. Drawing inspiration from RC designs that employ a separate motor or servo for the front axle, the team began prototyping a steering assembly. The approach involved mounting a front axle capable of angular rotation, driven by a gear connected to a secondary actuator.

The main obstacle encountered was finding an appropriate motor or actuator to drive the steering rotation. The team experimented with strings, different gear configurations, and various motor setups, but achieving consistent and repeatable turning angles proved elusive. Despite the mechanical challenges, this phase produced the first four-wheeled version of the robot and gave the team a much clearer picture of what needed to be solved — both mechanically and in terms of component sourcing.


Phase 4 — New Chassis Base and Component Layout

After finding a solution for the steering rotation mechanism — eventually settling on a goBILDA 300-degree torque servo to replace the unreliable motor-based approach — the team decided to redesign the chassis entirely. The new base was chosen for its greater structural rigidity and its modularity, making it easier to add, remove, or reposition components for future modifications.

This iteration also marked the first time electrical connections were formally established. An L298N motor driver was wired to control the drive motor, and the servo was connected directly to an Arduino UNO. The combination of a proper servo for steering and a motor driver for propulsion made this the first version of the robot capable of autonomous movement. The team used this phase to plan the layout of each electronic component on the chassis, though the robot still exceeded the 300mm width competition limit.


Phase 5 — Wheel Refinement and Initial Programming

With a functioning chassis and steering system in place, the team began refining the wheel geometry to maximize available space and comply with the 300mm width constraint. Wheel sizing and positioning were adjusted to reduce the overall footprint without compromising stability. Simultaneously, the first serious programming effort began: the Arduino was connected to the drive motor and servo, and basic motor control logic was written.

This phase represented the transition from a purely mechanical project to an electro-mechanical one. Motors were connected, power was routed, and the team began thinking about how software would govern the robot's behavior. It was also during this phase that weight became a significant concern — unnecessary components were removed or replaced with lighter alternatives to keep the robot within manageable mass limits.


Phase 6 — Motor Calibration and Manual Testing with Joystick

To properly tune the motor behavior and understand the real-world relationship between software commands and physical movement, the team introduced a joystick-based testing method. By controlling the robot manually in small increments, they were able to measure how speed changes translated into actual distance and turning radius. This incremental approach allowed the team to calibrate the rear-wheel motor precisely and define the servo angles required for left and right turns.

The joystick was not intended as a permanent control method, but as a diagnostic tool to gather data for the autonomous algorithm. During this phase the team also continued reducing weight wherever possible, removing redundant hardware and re-routing cables to minimize bulk. The result was a lighter, more manageable robot with a clearer understanding of its physical behavior.


Phase 7 — Power Management with L298N and 5V Regulator

As testing progressed, inconsistencies in motor behavior were traced back to voltage fluctuations in the power supply. To address this, the team formally integrated an L298N motor driver — ensuring that motor direction and speed could be regulated through clean digital signals rather than direct power switching. A 5V voltage stabilizer was also added to the system to protect the Arduino and sensitive electronics from the voltage spikes generated by the motors.

This was a pivotal moment in the electronics architecture. By separating the motor power line from the logic power line, the team eliminated a major source of unpredictable behavior. The robot's movement became noticeably more consistent, and the Arduino's responses to sensor input and timed commands became more reliable. This power isolation strategy — using separate supply paths for high-current and low-current components — carried forward into all future iterations.


Phase 8 — Redesigned Steering Mechanism with Servo and Gear System

The front wheel assembly underwent its most significant redesign in this phase. The previous steering approach was replaced with a purpose-built mechanism consisting of two small sub-frames, each holding one of the front wheels. These two sub-frames were connected by a longer central crossbar. At the center of this crossbar, a gear and the goBILDA servo were mounted together — when the servo rotated, it turned the entire crossbar assembly, pivoting both front wheels simultaneously.

The wheels themselves were also replaced during this phase with larger, higher-traction alternatives. This new steering geometry dramatically reduced the friction and mechanical slop that had plagued earlier iterations. The servo now had a direct, low-friction path to the front axle, and the turning angles became predictable and repeatable. The robot finally fit within the 300mm width limit — a critical milestone that opened the door to legitimate competition testing.


Phase 9 — Camera Integration and Autonomous Vision Programming

With the mechanical and power systems stable, the team shifted its focus to perception and autonomy. An ESP32-CAM module was selected as the vision system, chosen for its integrated camera, Wi-Fi capability, and relatively low cost. The camera was programmed to capture frames at 160x120 resolution and analyze only a central region of each image — specifically 35 to 72 percent of the height and 30 to 70 percent of the width — to detect red and green blocks using RGB threshold comparisons.

Every 120 milliseconds, the ESP32-CAM sends a single-line serial message to the Arduino via GPIO pin 14 at 115200 baud. The message format is straightforward: R0;G0 for no detection, R1;G0 for a red pillar, and R0;G1 for a green pillar. This lightweight protocol minimizes communication overhead and allows the Arduino to act quickly on visual data. In parallel, the motor control code was updated to respond to these color signals and adjust the robot's steering accordingly.


Phase 10 — Camera Installation and Cable Management

The ESP32-CAM was physically mounted at the front of the robot to provide an unobstructed field of view. During this phase, the team debugged the vision pipeline, correcting errors in color threshold calibration and ensuring the serial communication between the ESP32-CAM and the Arduino was stable. The physical layout of all components on the chassis was also finalized — sensors, motor driver, batteries, and the camera module were repositioned for optimal balance and accessibility.

A significant part of this phase involved cable management. All ground connections were unified to a common GND rail shared across both power lines — the 7.4V line for the motor system and the 6V line for the servo. Proper cable routing reduced electrical noise, minimized shorts, and made future maintenance significantly easier. The HC-SR04 ultrasonic distance sensor was also connected during this phase, wired to the Arduino's analog pins A0 and A1 for distance measurement.


Phase 11 — Final Software Completion and Motor Booster

The final development phase brought two major completions: the software and the power system for the drive motor. On the software side, the full six-state autonomous program was finalized and flashed to the ESP32-CAM. The Arduino was programmed with the complete state machine governing the robot's behavior throughout a competition run. After thorough testing, both firmware images were confirmed stable and the serial communication between the two microcontrollers was verified to be reliable under real movement conditions.

On the hardware side, an XL6009 boost converter was added between the L298N motor driver output and the drive motor. Because the motor required higher voltage than the driver's direct output provided, the boost converter stepped up the voltage to ensure consistent torque across varying battery charge levels. Together with the MINI 560 step-down converter — which regulated the 7.4V battery voltage down to 5V for the ESP32-CAM — the power system reached its final architecture: clean, isolated, and robust.


Electronics Architecture

The electronics system was designed around the principle of power isolation: sensitive logic components are kept separate from high-current motor circuits to prevent noise-induced malfunctions. Two independent batteries supply the robot's two power domains.

A 7.4V LiPo battery powers the motor domain. From this battery, current flows to the L298N motor driver, which controls the direction and speed of the main 25GA370 drive motor via Arduino pins D3, D5, and D6. Because the motor requires more voltage than the L298N output directly provides, an XL6009 boost converter is placed between the driver's output and the motor terminals. The same 7.4V battery also feeds a MINI 560 step-down converter, which regulates voltage down to 5V to safely power the ESP32-CAM.

A separate 6V battery powers the goBILDA steering servo exclusively. Isolating the servo on its own supply prevents voltage drops during sharp turns — when the servo draws peak current — from destabilizing the rest of the electronics. Two independent power switches, one per battery, allow the team to power each domain separately.

The main microcontroller is an Arduino UNO R3. It controls the L298N via digital pins 3, 5, and 6; the servo via pin 9; and the HC-SR04 ultrasonic sensor via analog pins A0 (echo) and A1 (trigger). The ESP32-CAM communicates with the Arduino over a two-wire serial connection using GPIO 14 and 15 on the ESP32 side and digital pins 10 and 11 on the Arduino side. All grounds across both power lines are unified at a common GND rail.


Software Architecture

The robot's software is split across two independent firmwares running on separate microcontrollers that communicate in real time. The ESP32-CAM handles all visual processing, while the Arduino UNO serves as the central controller managing navigation, motor control, and sensor fusion.

Vision Firmware — ESP32-CAM

The ESP32-CAM firmware captures frames at 160x120 resolution and analyzes only the central window of each image (35–72% of height, 30–70% of width) to focus on the area most likely to contain traffic pillars. Color detection uses simple RGB threshold comparisons to classify pixels as red or green. Every 120 milliseconds, the result is sent to the Arduino as a one-line serial message: R0;G0 (no pillar), R1;G0 (red detected), or R0;G1 (green detected).

Navigation Firmware — Arduino Six-State Machine

The Arduino runs a six-state machine that governs the robot's complete behavior during a competition run. Each state has a defined entry condition, action set, and exit transition.

ST_SCAN: The robot begins by rotating 360 degrees and measuring distances with the ultrasonic sensor to establish a baseline map of the environment. This initial scan sets reference values used throughout the run.

ST_LAP: The robot drives forward, continuously monitoring the ultrasonic sensor for sudden distance drops greater than 12cm (indicating an obstacle) and reading the ESP32-CAM serial data for color detections.

ST_TURN: When the sonar indicates a corner has been reached (after approximately 2.2 seconds of clear forward travel), the robot turns left for 2.7 seconds to complete one segment of the lap. After three full laps, the state machine transitions to ST_HOME.

ST_AVOID: When an obstacle is detected, the robot reverses if it is closer than 18cm, then steers left for a green pillar or right for a red pillar before advancing to clear the obstacle. A 2-second cooldown prevents the avoidance routine from triggering repeatedly on the same pillar.

ST_HOME: After completing three laps, the robot uses time-based odometry to navigate back to its starting position.

ST_DONE: The robot stops all motors and holds its final position.

Obstacle detection uses sensor fusion: a sudden sonar drop signals a physical object ahead, while the camera confirms its color to determine which direction to avoid. If only the sonar triggers with no color confirmed by the camera, the Arduino defaults to a cautious right turn.


Competition Strategy

The team's competition strategy centers on three pillars: consistency, structural reliability, and adaptive autonomous decision-making. Rather than optimizing purely for speed, the approach prioritizes clean, penalty-free runs. Lap counting relies on a hybrid odometry approach combining ultrasonic sensor data and ESP32-CAM signals to estimate position within the track, with a lap counted each time the system calculates that the vehicle has completed a full 360-degree rotation around the central barrier.

For obstacle handling, green pillars trigger an immediate left-steering maneuver, while red pillars trigger a right-steering response to maintain an optimal line through the course. In the advanced parking challenge, the robot applies its standard active sensing approach to locate the parking zone, executing a controlled deceleration and time-based odometry correction to finish precisely within the designated boundaries.

A fail-safe rescue subsystem is also implemented. If the ESP32-CAM detects zero pixel movement over a defined timeout period — indicating the robot may be stalled — the Arduino triggers a recovery sequence: reverse away from the obstruction, reassess surroundings with both sensors, and resume forward navigation on a clear path.


Conclusion

The Zorrobots FTC 9164 autonomous vehicle is the result of eleven progressive development phases, each building on the lessons of the last. What began as a simple plastic-wheeled chassis powered by a salvaged toy car motor evolved into a fully integrated autonomous system combining a purpose-built steering mechanism, a dual-battery electronics architecture, computer vision, ultrasonic sensing, and a sophisticated six-state control algorithm.

Every major challenge — from the unreliable early steering attempts to the voltage inconsistencies in the motor system — was solved through research, experimentation, and iteration rather than by purchasing pre-engineered solutions. The team's constraint of using no commercial robotics kits forced a deeper understanding of every subsystem and produced a robot that is genuinely the team's own creation in every meaningful sense.

The final robot demonstrates that high-quality autonomous engineering is achievable with recycled materials, salvaged electronics, and a disciplined iterative process. The Zorrobots team enters the WRO 2026 Future Engineers competition with a machine that reflects their full engineering journey — and the knowledge, skills, and resilience gained through building it.
