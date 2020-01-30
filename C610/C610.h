#ifndef MBED_C610_H_
#define MBED_C610_H_

#include "mbed.h"

#define C610_CAN_MESSAGE_LENGTH 8
#define C610_MAX_NUMBER_OF_MOTORS 8

class C610
{
public:
    C610(PinName can_rd, PinName can_td, int can_frequency)
        : can(can_rd, can_td, can_frequency),
          motor1to4_actuation_msg(0x200, new char[C610_CAN_MESSAGE_LENGTH](), C610_CAN_MESSAGE_LENGTH, CANData, CANStandard),
          motor5to8_actuation_msg(0x1FF, new char[C610_CAN_MESSAGE_LENGTH](), C610_CAN_MESSAGE_LENGTH, CANData, CANStandard),
          gear_ratio(36),
          hallsensor_resolution(8192),
          hallsensor_threshold(4096),
          angle_value_temp_max(8192 * 36 - 1),
          sangle_value_temp_offet(360.0 / (8192 * 36)),
          angle_value_temp(){};
    uint16_t angle[C610_MAX_NUMBER_OF_MOTORS];
    int16_t velocity[C610_MAX_NUMBER_OF_MOTORS];
    int16_t current[C610_MAX_NUMBER_OF_MOTORS];
    bool Begin(uint16_t number_of_motors);
    void Write(uint16_t motor_id, int16_t motor_current);

private:
    CAN can;
    CANMessage received_sensors_msg;
    CANMessage motor1to4_actuation_msg;
    CANMessage motor5to8_actuation_msg;

    uint16_t gear_ratio;
    int16_t hallsensor_resolution;
    int16_t hallsensor_threshold;
    int32_t angle_value_temp_max;
    float sangle_value_temp_offet;

    uint16_t raw_angle_value_prev[C610_MAX_NUMBER_OF_MOTORS];
    int32_t angle_value_temp[C610_MAX_NUMBER_OF_MOTORS];
    int16_t angle_diff;
    void Read();
    void AngleUpdate(uint16_t motor_array_id, uint16_t raw_angle_value);
};

#endif