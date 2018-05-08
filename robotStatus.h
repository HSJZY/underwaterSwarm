#ifndef ROBOTSTATUS_H
#define ROBOTSTATUS_H

#include"mpu/demo_dmp.h"

class robotStatus
{
public:
    robotStatus();
    void setInitAngleOfMPU(float angle);
    void setAbsAngleOfMPU(float angle);
    float getCurAngleOfMPU();

    void setZeroToAllMotor();
    void setKp(float setted_kp);
    void setKi(float setted_ki);
    void setKd(float setted_kd);

public:
    static float motor1_speed;
    static float motor2_speed;
    static float motor3_speed;
    static float motor4_speed;

    static float k_p;
    static float k_i;
    static float k_d;

private:
    static float m_initAngleOfMPU;
    static float m_curAngleOfMPU;
    static float m_absAngleOfMPU;
};

#endif // ROBOTSTATUS_H
