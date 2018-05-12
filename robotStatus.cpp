#include "robotStatus.h"
#include "mpu/demo_dmp.h"

float robotStatus::m_initAngleOfMPU;
float robotStatus::m_curAngleOfMPU;
float robotStatus::m_absAngleOfMPU;
float robotStatus::k_p;
float robotStatus::k_i;
float robotStatus::k_d;

vector<Mat> robotStatus::vec_cur_frames;

robotStatus::robotStatus()
{
}

void robotStatus::setZeroToAllMotor()
{
    this->motor1_speed=0;
    this->motor2_speed=0;
    this->motor3_speed=0;
    this->motor4_speed=0;
}

void robotStatus::setKp(float setted_kp)
{
    this->k_p=setted_kp;
}
void robotStatus::setKi(float setted_ki)
{
    this->k_i=setted_ki;
}
void robotStatus::setKd(float setted_kd)
{
    this->k_d=setted_kd;
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
//获取当前摄像头拍取的图片
vector<Mat> robotStatus::get_cur_frames()
{
    return this->vec_cur_frames;
}
//设定当前摄像头拍取的图片
void robotStatus::set_cur_frames(vector<Mat> vec_frames)
{
    this->vec_cur_frames=vec_frames;
}
