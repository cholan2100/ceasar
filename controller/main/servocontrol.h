#pragma once              //designed to include the current source file only once in a single compilation.
#ifndef I2CPWM_CONTROLLER //usd for conditional compiling.
#define I2CPWM_CONTROLLER

#include <map>
#include <vector>

namespace servocontrol
{
    typedef struct _servo
    {
        int servo;
        int value;
    } Servo;

    typedef std::vector<Servo> ServoArray;

    typedef struct _servo_config
    {
        int channel;
        int center;
        int range;
        int direction;
    } ServoConfig;

    int init();
    int config(std::map<int, ServoConfig> _servo_config);
    void servos_absolute(ServoArray &servos_pos);
    void servos_proportional(int servo, float value);
} // namespace servocontrol
#endif // I2CPWM_CONTROLLER