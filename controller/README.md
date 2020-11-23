# Ceasar ESP32 Firmware
Quadruped controller for Spot Micro using ESP32.

## Features:
- [] Motion Control
    - [*] Kinematics and programmed Gait based motion
    - [] Trotting Gait
    - [] Obstacle detection
    - [] Obstacle avoidance
    - [] MPU feedback integration for better motion control
    - [] Foot contact switch feedback loop integration
    - [] Reinforecement learning, would be a LOT of work to get AI models ported to ESP32, may be on app?.
- [] OTA firmware update
- [] Bluetooth link to smartphone app 
- [] Battery monitoring and charge control
    - [] Automated standby mode(power save)
    - [] Wake up on full charge, and stay in power save mode
- [] Voice recognition
- [] Automated tasks
- [] Firmware update from phone app

## TODO: app is still under development
## CAUTION: be extremely careful when using this beta state controller firmware, and do it at your own risk.

## Configure
Configure your robot using GPIO wiring and calibration data as mentioned in calibration documentation.
NOTE: In future calibration tool(which is under development) will be able to produce this for you.

<b>src/config.h</b>
```c
// I2C configuration - GPIO
#define I2C_MASTER_SCL_IO 22 /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 23 /*!< gpio number for I2C master data  */

// PCA9685 configuration
#define PCA9685_I2C_ADDRESS 0x40 /*!< slave address for PCA9685, DEFAULT: 0x40 */

// ROBOT Servo Configuration
// IMPORTANT: Use servo_calibration_spreadsheet.ods to position and configure the servos
// NOTE: the servos are number from 1-12, but the PCA9685 channels can be configured to any arbitrary channel number
// RF - Right Front Leg
// lower leg
#define RF_LOWER_SERVO_CHANNEL 8
#define RF_LOWER_SERVO_CENTER 306
#define RF_LOWER_SERVO_RANGE 385
#define RF_LOWER_SERVO_DIRECTION 1
#define RF_LOWER_SERVO_CENTER_ANG_DEG 99.86f
// upper leg
#define RF_UPPER_SERVO_CHANNEL 7
#define RF_UPPER_SERVO_CENTER 306
#define RF_UPPER_SERVO_RANGE 407
#define RF_UPPER_SERVO_DIRECTION 1
#define RF_UPPER_SERVO_CENTER_ANG_DEG -31.62f
// shoulder joint
#define RF_HIP_SERVO_CHANNEL 6
#define RF_HIP_SERVO_CENTER 306
#define RF_HIP_SERVO_RANGE 396
#define RF_HIP_SERVO_DIRECTION -1
#define RF_HIP_SERVO_CENTER_ANG_DEG 1.67f

// RB - Right Back Leg
// lower leg
#define RB_LOWER_SERVO_CHANNEL 2
#define RB_LOWER_SERVO_CENTER 306
#define RB_LOWER_SERVO_RANGE 369
#define RB_LOWER_SERVO_DIRECTION 1
#define RB_LOWER_SERVO_CENTER_ANG_DEG 95.37f
// upper leg
#define RB_UPPER_SERVO_CHANNEL 1
#define RB_UPPER_SERVO_CENTER 306
#define RB_UPPER_SERVO_RANGE 381
#define RB_UPPER_SERVO_DIRECTION 1
#define RB_UPPER_SERVO_CENTER_ANG_DEG -37.21f
// shoulder joint
#define RB_HIP_SERVO_CHANNEL 0
#define RB_HIP_SERVO_CENTER 306
#define RB_HIP_SERVO_RANGE 403
#define RB_HIP_SERVO_DIRECTION 1
#define RB_HIP_SERVO_CENTER_ANG_DEG -3.27f

// LB - Left Back Leg
// lower leg
#define LB_LOWER_SERVO_CHANNEL 5
#define LB_LOWER_SERVO_CENTER 306
#define LB_LOWER_SERVO_RANGE 374
#define LB_LOWER_SERVO_DIRECTION 1
#define LB_LOWER_SERVO_CENTER_ANG_DEG -92.65f
// upper leg
#define LB_UPPER_SERVO_CHANNEL 4
#define LB_UPPER_SERVO_CENTER 306
#define LB_UPPER_SERVO_RANGE 403
#define LB_UPPER_SERVO_DIRECTION 1
#define LB_UPPER_SERVO_CENTER_ANG_DEG 91.23f
// shoulder joint
#define LB_HIP_SERVO_CHANNEL 3
#define LB_HIP_SERVO_CENTER 306
#define LB_HIP_SERVO_RANGE 367
#define LB_HIP_SERVO_DIRECTION -1
#define LB_HIP_SERVO_CENTER_ANG_DEG -7.20f

// Lf - Left fRONT Leg
// lower leg
#define LF_LOWER_SERVO_CHANNEL 11
#define LF_LOWER_SERVO_CENTER 306
#define LF_LOWER_SERVO_RANGE 385
#define LF_LOWER_SERVO_DIRECTION 1
#define LF_LOWER_SERVO_CENTER_ANG_DEG -87.43f
// upper leg
#define LF_UPPER_SERVO_CHANNEL 10
#define LF_UPPER_SERVO_CENTER 306
#define LF_UPPER_SERVO_RANGE 388
#define LF_UPPER_SERVO_DIRECTION 1
#define LF_UPPER_SERVO_CENTER_ANG_DEG 38.21f
// shoulder joint
#define LF_HIP_SERVO_CHANNEL 9
#define LF_HIP_SERVO_CENTER 306
#define LF_HIP_SERVO_RANGE 388
#define LF_HIP_SERVO_DIRECTION 1
#define LF_HIP_SERVO_CENTER_ANG_DEG 4.67f

// ........................... //

//IMPORTANT: Enable simulated commands to activate robot on Power on.
//           It stands up and starts walking on power on.
//IMPORTANT: with MG996R servos, robot may not have enough power to lift the robot
//           to stand up position. Help it stand up to avoid Servo stalling and burn up.
#define SIMULATED_COMMANDS
```