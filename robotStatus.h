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

private:
    static float m_initAngleOfMPU;
    static float m_curAngleOfMPU;
    static float m_absAngleOfMPU;
};

#endif // ROBOTSTATUS_H
