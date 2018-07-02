#ifndef MOTOR_H
#define MOTOR_H

#include <iostream>
#include<wiringPi.h>
#include"global_setting.h"
#include"utils.h"
using namespace std;


class motor_c
{
public:
    int motorid=0;
    int motor_full=2500;
private:
//    int min_speed_anticlockwise=1400;
//    int max_speed_anticlockwise=1290;
//    int min_speed_clockwise=1430;
//    int max_speed_clockwise=1540;

        int min_speed_anticlockwise=1390;
        int max_speed_anticlockwise=1290;
        int min_speed_clockwise=1430;
        int max_speed_clockwise=1520;
public:
    motor_c();
    void motor_setup();
    void single_motor_setup(int motor_pin);

    //void motor_drive(int speed_1,int speed_2,int speed_3,int speed_4);
//    void motor1_c(int speed);
//    void motor2_c(int speed);
//    void motor3_c(int speed);
//    void motor4_c(int speed);

    void drive_motor(int pin,int clockwise_multi,float duty_speed);
};

//class motor_c
//{
//public:
//    int motor1=0;
//    int motor2=0;
//    int motor3=0;
//    int motor4=0;
//    int motor_full=2500;
//public:
//    void motor_setup();

//    void motor_drive(int speed_1,int speed_2,int speed_3,int speed_4);
//    void motor1_c(int speed);
//    void motor2_c(int speed);
//    void motor3_c(int speed);
//    void motor4_c(int speed);
//};

//class motor
//{
//public:
//    int motor_pin;
//    int motor_full=2500;
//public:
//    motor(int pin);
//    void motor_setup();
//    void drive_motor(int speed);
//    void TurnoffMotor();
//    void DirveAtSpeed(float speed);

// //    void motor1_c(int speed);
// //    void motor2_c(int speed);
// //        void motor3_c(int speed);
// //        void motor4_c(int speed);
//};

#endif // MOTOR_H
