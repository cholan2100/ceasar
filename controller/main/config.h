#define DEBUG_MODE  true

// #define BLUETOOTH_ENABLED

// I2C configuration - GPIO
#define I2C_MASTER_SCL_IO   22    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO   23    /*!< gpio number for I2C master data  */

// PCA9685 configuration
#define PCA9685_I2C_ADDRESS 0x40    /*!< slave address for PCA9685, DEFAULT: 0x40 */

// ROBOT Servo Configuration
// IMPORTANT: Use servo_calibration_spreadsheet.ods to position and configure the servos
// IMPORTANT: FIXME:: the code has ceratin hard coding which limits using PCA9685 channels only from 0-12
// RF - Right Front Leg
// lower leg
#define RF_LOWER_SERVO_CHANNEL         9      //NOTE: numbered as channel number on PCA9685 + 1 (range: 1-12) (due to indexing thing in old code). FIXME:
#define RF_LOWER_SERVO_CENTER          306
#define RF_LOWER_SERVO_RANGE           385
#define RF_LOWER_SERVO_DIRECTION       1
#define RF_LOWER_SERVO_CENTER_ANG_DEG  99.86f
// upper leg
#define RF_UPPER_SERVO_CHANNEL         8    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define RF_UPPER_SERVO_CENTER          306
#define RF_UPPER_SERVO_RANGE           407
#define RF_UPPER_SERVO_DIRECTION       1
#define RF_UPPER_SERVO_CENTER_ANG_DEG  -31.62f
// shoulder joint
#define RF_HIP_SERVO_CHANNEL         7    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define RF_HIP_SERVO_CENTER          306
#define RF_HIP_SERVO_RANGE           396
#define RF_HIP_SERVO_DIRECTION       -1
#define RF_HIP_SERVO_CENTER_ANG_DEG  1.67f

// RB - Right Back Leg
// lower leg
#define RB_LOWER_SERVO_CHANNEL         3  //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define RB_LOWER_SERVO_CENTER          306
#define RB_LOWER_SERVO_RANGE           369
#define RB_LOWER_SERVO_DIRECTION       1
#define RB_LOWER_SERVO_CENTER_ANG_DEG  95.37f
// upper leg
#define RB_UPPER_SERVO_CHANNEL         2    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define RB_UPPER_SERVO_CENTER          306
#define RB_UPPER_SERVO_RANGE           381
#define RB_UPPER_SERVO_DIRECTION       1
#define RB_UPPER_SERVO_CENTER_ANG_DEG  -37.21f
// shoulder joint
#define RB_HIP_SERVO_CHANNEL         1    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define RB_HIP_SERVO_CENTER          306
#define RB_HIP_SERVO_RANGE           403
#define RB_HIP_SERVO_DIRECTION       1
#define RB_HIP_SERVO_CENTER_ANG_DEG  -3.27f

// LB - Left Back Leg
// lower leg
#define LB_LOWER_SERVO_CHANNEL         6  //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LB_LOWER_SERVO_CENTER          306
#define LB_LOWER_SERVO_RANGE           374
#define LB_LOWER_SERVO_DIRECTION       1
#define LB_LOWER_SERVO_CENTER_ANG_DEG  -92.65f
// upper leg
#define LB_UPPER_SERVO_CHANNEL         5    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LB_UPPER_SERVO_CENTER          306
#define LB_UPPER_SERVO_RANGE           403
#define LB_UPPER_SERVO_DIRECTION       1
#define LB_UPPER_SERVO_CENTER_ANG_DEG  91.23f
// shoulder joint
#define LB_HIP_SERVO_CHANNEL           4    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LB_HIP_SERVO_CENTER            306
#define LB_HIP_SERVO_RANGE             367
#define LB_HIP_SERVO_DIRECTION         -1
#define LB_HIP_SERVO_CENTER_ANG_DEG    -7.20f

// Lf - Left fRONT Leg
// lower leg
#define LF_LOWER_SERVO_CHANNEL         12  //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LF_LOWER_SERVO_CENTER          306
#define LF_LOWER_SERVO_RANGE           385
#define LF_LOWER_SERVO_DIRECTION       1
#define LF_LOWER_SERVO_CENTER_ANG_DEG  -87.43f
// upper leg
#define LF_UPPER_SERVO_CHANNEL         11    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LF_UPPER_SERVO_CENTER          306
#define LF_UPPER_SERVO_RANGE           388
#define LF_UPPER_SERVO_DIRECTION       1
#define LF_UPPER_SERVO_CENTER_ANG_DEG  38.21f
// shoulder joint
#define LF_HIP_SERVO_CHANNEL           10    //NOTE: numbered as channel number on PCA9685 + 1 (due to indexing thing in old code). FIXME:
#define LF_HIP_SERVO_CENTER            306
#define LF_HIP_SERVO_RANGE             388
#define LF_HIP_SERVO_DIRECTION         1
#define LF_HIP_SERVO_CENTER_ANG_DEG    4.67f


// Robot physcial characteristics in meters
// IMPORTANT: DO NOT change any of these unless you are building you own robot model, and know exactly what you are doing!!!
// Kinematics description
#define HIP_LINK_LENGTH         0.0605
#define UPPER_LEG_LINK_LENGTH   0.11126
#define LOWER_LEG_LINK_LENGTH   0.1185 //TODO: include foot length
#define BODY_WIDTH              0.078
#define BODY_LENGTH             0.2075
//# Control Parameters
#define TRANSI_TAU              0.3
#define TRANSIT_RL              0.06
#define TRANSIT_ANGLE_RL        0.35

// Stance parameters,
//TODO: optimiize
#define DEFAULT_STAND_HEIGHT    0.155
#define STAND_FRONT_X_OFFSET    0.015
#define STAND_BACL_X_OFFSET     0.0
#define LIE_DOWN_HEIGHT         0.083
#define LIE_DOWN_FEET_X_OFFSET  0.065

// Gait configuration
#define MAX_FWD_VELOCITY        0.4
#define MAX_SIDE_VELOCITY       0.4
#define MAX_YAW_RATE            0.35
#define Z_CLEARANCE             0.045
#define ALPHA                   0.5
#define BETA                    0.5
#define NUM_PHASES              8 // 8 phse Gait
#define RB_CONTACT_PHASES       {1, 0, 1, 1, 1, 1, 1, 1}
#define RF_CONTACT_PHASES       {1, 1, 1, 0, 1, 1, 1, 1}
#define LF_CONTACT_PHASES       {1, 1, 1, 1, 1, 1, 1, 0}
#define LB_CONTACT_PHASES       {1, 1, 1, 1, 1, 0, 1, 1}
#define OVERLAP_TIME            0.0
#define SWING_TIME              0.20
#define FOOT_HEIGH_TIME_CONST   0.02
#define BODY_SHIFT_PHASES       {1, 2, 3, 4, 5, 6, 7, 8}
#define FWD_BODY_BALANCE_SHIFT  0.025
#define BACK_BODY_BALANCE_SHIFT 0.005
#define SIDE_BODY_BALANCE_SHIFT 0.015
#define GAIT_FRAME_RATE         (1.0f/50.0f) //# 50hz

//IMPORTANT: Enable simulated commands to activate robot on Power on.
//           It stands up and starts walking on power on.
//IMPORTANT: with MG996R servos, robot may not have enough power to lift the robot
//           to stand up position. Help it stand up to avoid Servo stalling and burn up.
#define SIMULATED_COMMANDS