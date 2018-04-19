#include "motor.h"
#include<wiringPi.h>
#include<math.h>
#include<iostream>
#include"utils.h"

using namespace std;
motor_c::motor_c()
{

}

void motor_c::motor_setup()
{
    pinMode(motor1_pin,OUTPUT);
    pinMode(motor2_pin,OUTPUT);
    pinMode(motor3_pin,OUTPUT);
    pinMode(motor4_pin,OUTPUT);
    for(int i=0;i<300;i++)
    {
        digitalWrite(motor1_pin,HIGH);
        digitalWrite(motor2_pin,HIGH);
        digitalWrite(motor3_pin,HIGH);
        digitalWrite(motor4_pin,HIGH);
        delayMicroseconds(2000);
        digitalWrite(motor1_pin,LOW);
        digitalWrite(motor2_pin,LOW);
        digitalWrite(motor3_pin,LOW);
        digitalWrite(motor4_pin,LOW);
        delayMicroseconds(500);
    }
   // num2=700;
    for(int i=0;i<300;i++)
    {
        digitalWrite(motor1_pin,HIGH);
        digitalWrite(motor2_pin,HIGH);
        digitalWrite(motor3_pin,HIGH);
        digitalWrite(motor4_pin,HIGH);
        delayMicroseconds(700);
        digitalWrite(motor1_pin,LOW);
        digitalWrite(motor2_pin,LOW);
        digitalWrite(motor3_pin,LOW);
        digitalWrite(motor4_pin,LOW);
        delayMicroseconds(1800);
    }
}
void motor_c::drive_motor(int pin,int clockwise_multi, float duty_speed)
{
    duty_speed=duty_speed*clockwise_multi;
    int speed=0;
    if(duty_speed==0)
    {
        return;
    }
    else if(duty_speed<0)
    {
//        utils<float> util;
//        duty_speed=util.max(-1.0,duty_speed);
        duty_speed=-1.0>duty_speed?-1:duty_speed;
        //逆时针旋转
        int interval=max_speed_anticlockwise-min_speed_anticlockwise;
        speed=min_speed_anticlockwise-abs(interval*duty_speed);

    }
    else
    {
//        utils<float>  util;
//        duty_speed=util.min(1,duty_speed);
        duty_speed=duty_speed>1?1:duty_speed;
        int interval=max_speed_clockwise-min_speed_clockwise;
        speed=min_speed_clockwise+abs(interval*duty_speed);
    }
//    int speed=duty_speed;
    digitalWrite(pin,HIGH);
    delayMicroseconds(speed);
    digitalWrite(pin,LOW);
    delayMicroseconds(this->motor_full-speed);
}

//void motor_c::motor1_c(int speed)
//{
//    //this->motor1=m;
////    pinMode(motor1_pin,OUTPUT);
//    digitalWrite(motor1_pin,HIGH);
//    delayMicroseconds(speed);
//    digitalWrite(motor1_pin,LOW);
//    delayMicroseconds(this->motor_full-speed);
//}

//void motor_c::motor2_c(int speed)
//{
////    pinMode(motor2_pin,OUTPUT);
//    digitalWrite(motor2_pin,HIGH);
//    delayMicroseconds(speed);
//    digitalWrite(motor2_pin,LOW);
//    delayMicroseconds(this->motor_full-speed);
//}

//void motor_c::motor3_c(int speed)
//{
////     pinMode(motor3_pin,OUTPUT);
//    digitalWrite(motor3_pin,HIGH);
//    delayMicroseconds(speed);
//    digitalWrite(motor3_pin,LOW);
//    delayMicroseconds(this->motor_full-speed);
//}

//void motor_c::motor4_c(int speed)
//{
////    pinMode(motor4_pin,OUTPUT);
//    digitalWrite(motor4_pin,HIGH);
//    delayMicroseconds(speed);
//    digitalWrite(motor4_pin,LOW);
//    delayMicroseconds(this->motor_full-speed);
//}

