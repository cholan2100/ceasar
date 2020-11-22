# Ceasar ESP32
Quadruped controller for Spot Micro using ESP32.
Includes controller firmware for ESP32, calibration utility to help build the robot, and an app for mobile phones to control the robot.

## Action video
[![voice](https://img.youtube.com/vi/YhH5K681pzY/0.jpg)](https://www.youtube.com/watch?v=YhH5K681pzY "App")
[![walk](https://img.youtube.com/vi/QZe1R5wRUV8/0.jpg)](https://www.youtube.com/watch?v=QZe1R5wRUV8 "Walk")

## Motivation
Michael Kubina's motivation to build a economic version of Spot Micro using ESP32 with limited entry barrier for people interested to build a Quadruped. Please refer to Michael's 3D models repo for 3D printable Spot Micro model
3D model: https://github.com/michaelkubina/SpotMicroESP32

Initial developments on ESP32 came from Maarteen Weyn who ported the Spot Micro's python version of Inverse Kinematics to ESP32 and user interface on Android phones using Cordova framework. User inteface for this Spot ESP32 controller is heavily derived from Maarten's app using Cordova.
Maarten's Code: https://github.com/maartenweyn/SpotMicro_ESP32

Mike implemented the first version of software controller for original Spot Micro project on ROS framework using research papers on Kinematics and 8 phase Gait, which is still the popular software for Spot Micro project. ESP32 controller for Spot Micro is heavily derived from Mike's implementation.
Mike's Spot Micro: https://github.com/mike4192/spotMicro


## TODO: STILL UNDER development