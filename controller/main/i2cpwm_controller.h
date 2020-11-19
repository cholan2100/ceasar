#pragma once //designed to include the current source file only once in a single compilation.
#ifndef I2CPWM_CONTROLLER //usd for conditional compiling.
#define I2CPWM_CONTROLLER

#include <map>

namespace i2cpwm_board
{
    typedef struct _servo {
        int servo;
        int value;
    } Servo;

    typedef std::map<int, Servo> ServoArray;

    typedef struct _servo_config {
        int servo;
        int center;
        int range;
        int direction;
        int mode_pos;
    } ServoConfig;

    int init();
    int config(std::map<int, ServoConfig> servo_config);
    void servos_absolute (const ServoArray servos);
    void servos_proportional (const ServoArray servos);
};

#endif // I2CPWM_CONTROLLER