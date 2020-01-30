#include "C610.h"
#include "mbed.h"

#define CAN_RD PA_11
#define CAN_TD PA_12
#define CAN_FREQUENCY 1000000

#define MOTOR1 0x201
#define MOTOR2 0x202
#define MOTOR3 0x203
#define MOTOR4 0x204
#define MOTOR5 0x205
#define MOTOR6 0x206
#define MOTOR7 0x207
#define MOTOR8 0x208

///   Nucleo Board STM32F303K8とCANコントローラMCP2561を使っています．
///   MCP2561周りの回路は以下のURL先の画像を参考にしています．
///   https://www.avrfreaks.net/forum/seeking-some-assistance-wire-components

C610 Motor(CAN_RD, CAN_TD, CAN_FREQUENCY);
///   M2006モータを1つ使うとき，C610のCAN IDは0x201
///   M2006モータを2つ使うとき，C610のCAN IDは0x201~x202
///   M2006モータを3つ使うとき，C610のCAN IDは0x201~x203
///   M2006モータを4つ使うとき，C610のCAN IDは0x201~x204
///   M2006モータを5つ使うとき，C610のCAN IDは0x201~x205
///   M2006モータを6つ使うとき，C610のCAN IDは0x201~x206
///   M2006モータを7つ使うとき，C610のCAN IDは0x201~x207
///   M2006モータを8つ使うとき，C610のCAN IDは0x201~x208   に設定してください．

///   C610ライブラリでは，CAN通信を使い，1つのCANバス上で最大8つのM2006モータを扱います．
///   C610ライブラリの引数には，CANのピンと周波数が必要になります．

///   C610ライブラリでは，二つの関数と三つの配列にアクセスできます．
///   Begin(uint16_t number_of_motors)…main関数の最初に実行することによって，出力軸の絶対角計算と更新を開始しています．引数は同時に扱うM2006モータの個数です．
///   Write(uint16_t motor_id, int16_t motor_current)…この関数でモータ出力を指定します．C610において，IDは0x201～0x208，入力電流(?)値は-10000～10000を扱います．
///   angle[8]…0x201～0x208のIDに対応するモータの絶対角が順に格納されています．単位は[deg]．
///   velocity[8]…モータ速度，単位は[rpm].
///   current[8]…モータの出力電流(にオフセットのかかったもの?)

Serial pc(USBTX, USBRX);

int main()
{
    if (Motor.Begin(2))
    {
        pc.printf("Begin Successful\n");
    }
    else
    {
        pc.printf("Begin fault\n");
    }

    while (1)
    {
        for (int i = 0; i < 8; i++)
        {
            pc.printf("%d:%5d[deg] ", i + 1, Motor.angle[i]);
            //            pc.printf("%d:%5d[rpm] ", i+1, Motor.velocity[i]);
            //            pc.printf("%d:%5d[mA] ", i+1, Motor.current[i]);
        }
        pc.printf("\n");
        wait_ms(10);
    }

    //    Motor.Write(0x201, 1000);
    //    wait(0.5);
    //    Motor.Write(0x201, 0);
    //    wait(0.01);
    //    Motor.Write(0x202, 1000);
    //    wait(0.5);
    //    Motor.Write(0x202, 0);
    //    wait(0.01);
    //    Motor.Write(0x203, 1000);
    //    wait(0.5);
    //    Motor.Write(0x203, 0);
    //    wait(0.01);
    //    Motor.Write(0x204, 1000);
    //    wait(0.5);
    //    Motor.Write(0x204, 0);

    //    Motor.Write(0x205, 1000);
    //    wait(0.5);
    //    Motor.Write(0x205, 0);
    //    wait(0.01);
    //    Motor.Write(0x206, 1000);
    //    wait(0.5);
    //    Motor.Write(0x206, 0);
    //    wait(0.01);
    //    Motor.Write(0x207, 1000);
    //    wait(0.5);
    //    Motor.Write(0x207, 0);
    //    wait(0.01);
    //    Motor.Write(0x208, 1000);
    //    wait(0.5);
    //    Motor.Write(0x208, 0);
}
