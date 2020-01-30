#include "C610.h"
#include "mbed.h"

#define CAN_WRITE_BITMASK 0x0FF

///   センサからのCANメッセージの取得のたびに実行され，ブラシレスモータの絶対角からギヤヘッドの出力軸の絶対角を計算し，更新した角度を並列へ格納します．
void C610::AngleUpdate(uint16_t motor_array_id, uint16_t raw_angle_value)
{
    angle_diff = raw_angle_value - raw_angle_value_prev[motor_array_id];

    if (angle_diff > hallsensor_threshold)
    {
        angle_value_temp[motor_array_id] -= hallsensor_resolution - angle_diff;
    }
    else if (angle_diff < -hallsensor_threshold)
    {
        angle_value_temp[motor_array_id] += hallsensor_resolution + angle_diff;
    }
    else
    {
        angle_value_temp[motor_array_id] += angle_diff;
    }

    if (angle_value_temp[motor_array_id] > angle_value_temp_max)
    {
        angle_value_temp[motor_array_id] -= angle_value_temp_max;
    }
    if (angle_value_temp[motor_array_id] < 0)
    {
        angle_value_temp[motor_array_id] += angle_value_temp_max;
    }

    raw_angle_value_prev[motor_array_id] = raw_angle_value;
    angle[motor_array_id] = angle_value_temp[motor_array_id] * sangle_value_temp_offet;
}

///   センサからのCANメッセージを受け取ったときに実行され，角度を計算・更新し，速度，電流値を配列にそれぞれ格納します．
void C610::Read()
{
    can.read(received_sensors_msg);
    if ((0x201 <= received_sensors_msg.id) && (received_sensors_msg.id <= 0x208))
    {
        AngleUpdate((uint16_t)(received_sensors_msg.id - 0x201), (uint16_t)(received_sensors_msg.data[0] << 8 | received_sensors_msg.data[1]));
        velocity[received_sensors_msg.id - 0x201] = ((int16_t)(received_sensors_msg.data[2] << 8 | received_sensors_msg.data[3])) / gear_ratio;
        current[received_sensors_msg.id - 0x201] = (int16_t)(received_sensors_msg.data[4] << 8 | received_sensors_msg.data[5]);
    }
}

///   モータ角度の計算に必要なモータ角度の初期値を取得し，角度の計算をセンサからのCANメッセージ取得のたびに行う割込みを設定しています．
bool C610::Begin(uint16_t number_of_motors)
{
    for (int motor_array_id = 0; motor_array_id < number_of_motors; motor_array_id++)
    {
        bool flag = true;
        while (flag)
        {
            can.read(received_sensors_msg);
            if ((received_sensors_msg.id - 0x201) == motor_array_id)
            {
                raw_angle_value_prev[motor_array_id] = (uint16_t)(received_sensors_msg.data[0] << 8 | received_sensors_msg.data[1]);
                flag = false;
            }
        }
    }

    can.attach(callback(this, &C610::Read), CAN::RxIrq);
    return true;
}

///   IDに対応した入力をCANメッセージに格納し，バスに送信します．
void C610::Write(uint16_t motor_id, int16_t motor_current)
{
    if ((0x201 <= motor_id) && (motor_id <= 0x204))
    {
        motor1to4_actuation_msg.data[(motor_id - 0x201) * 2] = ((motor_current >> 8) & CAN_WRITE_BITMASK);
        motor1to4_actuation_msg.data[(motor_id - 0x201) * 2 + 1] = (motor_current & CAN_WRITE_BITMASK);
        can.write(motor1to4_actuation_msg);
    }
    else if ((0x205 <= motor_id) && (motor_id <= 0x208))
    {
        motor5to8_actuation_msg.data[(motor_id - 0x205) * 2] = ((motor_current >> 8) & CAN_WRITE_BITMASK);
        motor5to8_actuation_msg.data[(motor_id - 0x205) * 2 - 1] = (motor_current & CAN_WRITE_BITMASK);
        can.write(motor5to8_actuation_msg);
    }
}