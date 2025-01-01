# Vision based Telerobot Arm with Digital Twin


## Features
- **Digital Twin**: 3D Unity Replica.
- **Real-Time Vision Feedback**: Camera integration for precise monitoring.
- **Telerobotics**: Operate the robot arm remotely.
- **Hand Tracking Control**: Leverages OpenCV and MediaPipe for intuitive hand gesture control.
- **Smooth Servo Control**: Uses MQTT and NodeMCU for seamless motor operation.

## Technologies
- **Python**: For hand tracking and vision feedback.
- **C++**: NodeMCU program.
- **NodeMCU**: MQTT-based control of servo motors.
- **OpenCV & MediaPipe**: Hand gesture recognition.
- **Unity**: Visualization of real-time robot arm movements.

## Hardware
- **Robot Arm**: 6-DOF robotic arm.
- **Camera**: Laptop Camera.
- **NodeMCU**: Microcontroller for servo control.
- **Servo Motors**: Servo motors for arm movement.

## Software
- **Python Script**: Handles hand tracking and vision feedback.
- **MQTT Communication**: Publishes control commands and receives feedback to main server.
- **Websocket** : For main server client communication 
- **Unity3D**: Used to build the digital twin environment.


