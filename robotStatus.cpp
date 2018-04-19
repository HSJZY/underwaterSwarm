#include "robotStatus.h"
#include "mpu/demo_dmp.h"

float robotStatus::m_initAngleOfMPU;
float robotStatus::m_curAngleOfMPU;
float robotStatus::m_absAngleOfMPU;

robotStatus::robotStatus()
{
}

void robotStatus::setInitAngleOfMPU(float angle)
{
    this->m_initAngleOfMPU=angle;
}

void robotStatus::setAbsAngleOfMPU(float angle)
{
    this->m_absAngleOfMPU=angle;
    float curAngleOfMPU=m_absAngleOfMPU-m_initAngleOfMPU;
    if(curAngleOfMPU>180)
    {
        curAngleOfMPU=curAngleOfMPU-360;
    }
    else if(curAngleOfMPU<-180)
    {
        curAngleOfMPU=curAngleOfMPU+360;
    }
    this->m_curAngleOfMPU=curAngleOfMPU;
}

float robotStatus::getCurAngleOfMPU()
{
    return this->m_curAngleOfMPU;
}
