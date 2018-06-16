#ifndef ROBOTSTATUS_H
#define ROBOTSTATUS_H

#include"mpu/demo_dmp.h"
#include<vector>
#include<opencv2/opencv.hpp>
#include"global_setting.h"
#include<thread>
#include<wiringPi.h>

using namespace std;
using namespace cv;

class robotStatus
{
public:
    robotStatus();
    void setInitAngleOfMPU(float angle);
    void setAbsAngleOfMPU(float angle,bool is_setup=false);
    float getCurAngleOfMPU();

    void setZeroToAllMotor();
    void setKp(float setted_kp);
    void setKi(float setted_ki);
    void setKd(float setted_kd);
    void set_if_motor_is_sleep(int motor_pin,bool is_sleep);
    bool get_if_motor_is_sleep(int motor_pin);

    void set_formation_stop(bool is_stop);
    bool get_formation_is_stop_state();

    void set_cur_frames(vector<Mat> vec_frames);
    vector<Mat> get_cur_frames();
    void  update_agents_postion();

    void set_agents_position(vector<vector<vector<float> > > agents_position);
    vector<vector<vector<float> > > get_agents_position();
    void static listen_motors();

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

    static bool formation_stop;

    static bool motor1_is_sleep;
    static bool motor2_is_sleep;
    static bool motor3_is_sleep;
    static bool motor4_is_sleep;

    static vector<vector<vector<float> > > vec_agents_postion;

    static vector<Mat> vec_cur_frames;
};

#endif // ROBOTSTATUS_H
