#ifndef KINEMATICCONTROL_H
#define KINEMATICCONTROL_H
#include "robotStatus.h"
#include "motor.h"
#include<thread>
#include<iostream>
#include<time.h>
#include<sys/time.h>
#include<math.h>
#include<wiringPi.h>
#include<algorithm>
#include"pid.h"

using namespace std;

class kinematicControl
{
public:
    kinematicControl();
    void motor_setup();
    struct Robot_PID  MoveForward(float angle,float ratio_speed,float duration_ms,struct Robot_PID last_pid=first_pid);
    void SelfRotate(float target_angle);
    struct Robot_PID MoveLateral(float angle,int side,float ratio_speed,float duration_ms,struct Robot_PID last_pid=first_pid);
    void switchMode();
    struct Robot_PID get_first_pid();
    void set_first_pid(float k_p,float k_i,float k_d);
    void back_rotate_two_motor(int side,float ratio_speed,float duration_ms);
private:
    void static drive_motor_thread_fun(int motor_id,float speed,motor_c motor);
    static struct Robot_PID first_pid;
};

#endif // KINEMATICCONTROL_H
