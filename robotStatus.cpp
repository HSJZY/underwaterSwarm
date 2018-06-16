#include "robotStatus.h"
#include "mpu/demo_dmp.h"

float robotStatus::m_initAngleOfMPU;
float robotStatus::m_curAngleOfMPU;
float robotStatus::m_absAngleOfMPU;

float robotStatus::k_p;
float robotStatus::k_i;
float robotStatus::k_d;

bool robotStatus::motor1_is_sleep;
bool robotStatus::motor2_is_sleep;
bool robotStatus::motor3_is_sleep;
bool robotStatus::motor4_is_sleep;

bool robotStatus::formation_stop;

vector<vector<vector<float> > > robotStatus::vec_agents_postion;
vector<Mat> robotStatus::vec_cur_frames;

robotStatus::robotStatus()
{

}


bool robotStatus::get_formation_is_stop_state()
{
    return this->formation_stop;
}
void robotStatus::set_formation_stop(bool is_stop)
{
    this->formation_stop=is_stop;
}

void robotStatus::setZeroToAllMotor()
{
    this->motor1_speed=0;
    this->motor2_speed=0;
    this->motor3_speed=0;
    this->motor4_speed=0;
}

void robotStatus::set_if_motor_is_sleep(int motor_pin, bool is_sleep)
{
    switch (motor_pin) {
    case motor1_pin:
        motor1_is_sleep=is_sleep;
        break;
    case motor2_pin:
        motor2_is_sleep=is_sleep;
        break;
    case motor3_pin:
        motor3_is_sleep=is_sleep;
        break;
    case motor4_pin:
        motor4_is_sleep=is_sleep;
        break;
    default:
        break;
    }
    return;
}

void robotStatus::listen_motors()
{
    robotStatus cur_robot_status;
    while(1)
    {
        if(cur_robot_status.motor1_speed==0)
        {
            cur_robot_status.set_if_motor_is_sleep(motor1_pin,true);
        }
        else
        {
            cur_robot_status.set_if_motor_is_sleep(motor1_pin,false);
        }
        if(cur_robot_status.motor2_speed==0)
        {
            cur_robot_status.set_if_motor_is_sleep(motor2_pin,true);
        }
        else
        {
            cur_robot_status.set_if_motor_is_sleep(motor2_pin,false);
        }
        if(cur_robot_status.motor3_speed==0)
        {
            cur_robot_status.set_if_motor_is_sleep(motor3_pin,true);
        }
        else
        {
            cur_robot_status.set_if_motor_is_sleep(motor3_pin,false);
        }
        if(cur_robot_status.motor4_speed==0)
        {
            cur_robot_status.set_if_motor_is_sleep(motor4_pin,true);
        }
        else
        {
            cur_robot_status.set_if_motor_is_sleep(motor4_pin,false);
        }
        delay(50);
    }
}

bool robotStatus::get_if_motor_is_sleep(int motor_pin)
{
    switch (motor_pin) {
    case motor1_pin:
        return motor1_is_sleep;
        break;
    case motor2_pin:
        return motor2_is_sleep;
        break;
    case motor3_pin:
        return motor3_is_sleep;
        break;
    case motor4_pin:
        return motor4_is_sleep;
        break;
    default:
        return false;
        break;
    }
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

void robotStatus::setAbsAngleOfMPU(float angle,bool is_setup)
{
//    //简单去噪
//    if(abs(angle-this->m_absAngleOfMPU)>80&&is_setup==false)
//        angle=this->m_absAngleOfMPU;
    this->m_absAngleOfMPU=angle;
    float curAngleOfMPU=angle-m_initAngleOfMPU;
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
//    return  m_absAngleOfMPU;
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

//更新全局位置信息
void robotStatus::update_agents_postion()
{
    
}

void robotStatus::set_agents_position(vector<vector<vector<float> > > agents_postion)
{
    this->vec_agents_postion=agents_postion;
}
vector<vector<vector<float> > > robotStatus::get_agents_position()
{
    return this->vec_agents_postion;
}
