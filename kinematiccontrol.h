#ifndef KINEMATICCONTROL_H
#define KINEMATICCONTROL_H
#include "robotStatus.h"
#include "motor.h"
#include<thread>
#include<iostream>
#include<time.h>
#include<sys/time.h>
#include<math.h>

using namespace std;

class kinematicControl
{
public:
    kinematicControl();
    void MoveForward(float angle,float ratio_speed,float duration_ms);
    void SelfRotate(float target_angle);
    void MoveLateral(float angle,int side,float ratio_speed,float duration_ms);
private:
    void static drive_motor_thread_fun(int motor_id,float speed,motor_c motor);
};

#endif // KINEMATICCONTROL_H
