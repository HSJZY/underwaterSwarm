#include "kinematiccontrol.h"

float robotStatus::motor1_speed;
float robotStatus::motor2_speed;
float robotStatus::motor3_speed;
float robotStatus::motor4_speed;

kinematicControl::kinematicControl()
{
    robotStatus curRobotStatue;
    pidInit(&this->first_pid,curRobotStatue.k_p,curRobotStatue.k_i,curRobotStatue.k_d);
}

void kinematicControl::set_first_pid(float k_p, float k_i, float k_d)
{
    pidInit(&this->first_pid,k_p,k_i,k_d);
}

void kinematicControl::drive_motor_thread_fun(int motor_id,float speed,motor_c motor)
{
        switch (motor_id) {
        case 0:
            motor.drive_motor(motor1_pin,motor1_clockwise_multi,speed);
//            motor_1.motor1_c(speed);
            break;
        case 1:
            motor.drive_motor(motor2_pin,motor2_clockwise_multi,speed);
//            motor_1.motor2_c(speed);
            break;
        case 2:
            motor.drive_motor(motor3_pin,motor3_clockwise_multi,speed);
            break;
        case 3:
            motor.drive_motor(motor4_pin,motor4_clockwise_multi,speed);
            break;
        default:
            break;
        }
}
void kinematicControl::motor_setup()
{
    motor_c motor_left;
    motor_left.motor_setup();
}
void kinematicControl::switchMode()
{
    delay(1500);
    this->motor_setup();
}

struct Robot_PID kinematicControl::get_first_pid()
{
    return this->first_pid;
}

struct Robot_PID kinematicControl::MoveForward(float target_angle, float ratio_speed,float duration_ms,Robot_PID last_pid)
{
    robotStatus curRobotStatue;

    struct timeval timerBreakStart,timerBreakEnd;
    gettimeofday(&timerBreakStart,NULL);
    long int startTime=timerBreakStart.tv_sec*1000+timerBreakStart.tv_usec/1000;

    long int lastTime=startTime;
    motor_c motor_left,motor_right,motor_third;


    Robot_PID robot_pid;
    robot_pid=last_pid;

    float diff_speed=0;
    float last_diff_speed=0;

    //以下用于读取机器人此刻的速度
    float last_left_speed=0;
    float last_right_speed=0;
    while(1){
        float cur_angle=curRobotStatue.getCurAngleOfMPU();

        int motor_pin_left,motor_pin_right,motor_pin_left_back,motor_pin_right_back;
        float left_speed,right_speed,angle_diff;
        //确定需要往前或后走的电机
        if(target_angle>=-90 &&target_angle<=90)
        {
            motor_pin_left=motor3_pin;
            motor_pin_right=motor1_pin;
            motor_pin_left_back=motor4_pin;
            motor_pin_right_back=motor2_pin;

            angle_diff=target_angle-cur_angle;

            curRobotStatue.motor1_speed=last_right_speed;
            curRobotStatue.motor3_speed=last_left_speed;
        }
        else
        {
            motor_pin_left=motor2_pin;
            motor_pin_right=motor4_pin;
            motor_pin_left_back=motor1_pin;
            motor_pin_right_back=motor3_pin;

            if (target_angle<0){target_angle+=360;}
            float cur_angle_tranfer=cur_angle+180;
            angle_diff=target_angle-cur_angle_tranfer;

            curRobotStatue.motor2_speed=last_left_speed;
            curRobotStatue.motor4_speed=last_right_speed;
        }
        angle_diff/=90.0;

        //当时间超过设定的时间，跳出循环，结束这部分的程序
        gettimeofday(&timerBreakEnd,NULL);
        long int endTime=timerBreakEnd.tv_sec*1000+timerBreakEnd.tv_usec/1000;
        if(endTime-startTime>duration_ms)break;
        //
        float dt=0.1;
        if((endTime-lastTime)>dt*1000)
        {
            diff_speed=pidUpdate_err(&robot_pid,angle_diff,(endTime-lastTime)/1000.0);
            lastTime=endTime;
            last_diff_speed=diff_speed;
        }

        left_speed=ratio_speed-last_diff_speed;
        right_speed=ratio_speed+last_diff_speed;

        left_speed=std::max(std::min(left_speed,float(1)),float(0.01));
        right_speed=std::max(std::min(right_speed,float(1)),float(0.01));

        //添加第三个电机的转动来平衡产生的力差
        float left_right_diff_speed=left_speed-right_speed;
        int third_motor_pin;
        float third_motor_speed;
        if(left_right_diff_speed>0)
        {
            //左边速度大于右边，转动右后方的电机
            third_motor_pin=motor_pin_right_back;
            if(abs(left_right_diff_speed)>0.1)
                third_motor_speed=0.2*abs(left_right_diff_speed);
            else
                third_motor_speed=0;

            if(motor_pin_right_back==motor2_pin)
            {
                curRobotStatue.motor2_speed=third_motor_speed;

            }
            else if(motor_pin_right_back==motor3_pin)
            {
                curRobotStatue.motor3_speed=third_motor_speed;
            }

        }
        else
        {
            //右边速度大于左边，转动左后方的电机
            third_motor_pin=motor_pin_left_back;
            if(abs(left_right_diff_speed)>0.1)
            {
                third_motor_speed=0.2*abs(left_right_diff_speed);
            }
            else
            {
                third_motor_speed=0;
            }

            if(motor_pin_left_back==motor4_pin)
            {
                curRobotStatue.motor4_speed=third_motor_speed;
            }
            else if(motor_pin_left_back==motor1_pin)
            {
                curRobotStatue.motor1_speed=third_motor_speed;
            }
        }

        last_left_speed=left_speed;
        last_right_speed=right_speed;

        cout<<"kp:"<<curRobotStatue.k_p<<"ki:"<<curRobotStatue.k_i<<"kd:"<<curRobotStatue.k_d<<endl;
        cout<<"current yaw...:"<<cur_angle<<"left_speed:"<<left_speed<<"right speed:"<<right_speed<<endl;
        thread left_motor_thread(this->drive_motor_thread_fun,motor_pin_left,left_speed,motor_left);
        thread right_motor_thread(this->drive_motor_thread_fun,motor_pin_right,right_speed,motor_right);
        thread third_motor_thread(this->drive_motor_thread_fun,third_motor_pin,third_motor_speed,motor_third);

        left_motor_thread.join();
        right_motor_thread.join();
        third_motor_thread.join();
    }
    return robot_pid;
}

void kinematicControl::SelfRotate(float target_angle)
{
    robotStatus curRobotStatue;
    motor_c motor_1,motor_2;
//    motor_1.motor_setup();
    float ratio_speed=0.2;
    while(1)
    {
        int motor_pin_1,motor_pin_2;
        float cur_angle=curRobotStatue.getCurAngleOfMPU();
        float cur_ratio_speed=ratio_speed*(0.5+abs(cur_angle-target_angle)/30);
        if(cur_angle<target_angle)
        {
            motor_pin_1=motor1_pin;
            motor_pin_2=motor4_pin;
            curRobotStatue.motor1_speed=cur_ratio_speed;
            curRobotStatue.motor4_speed=cur_ratio_speed;
        }
        else
        {
            motor_pin_1=motor2_pin;
            motor_pin_2=motor3_pin;
            curRobotStatue.motor2_speed=cur_ratio_speed;
            curRobotStatue.motor3_speed=cur_ratio_speed;
        }

        thread first_motor_thread(this->drive_motor_thread_fun,motor_pin_1,cur_ratio_speed,motor_1);
        thread second_motor_thread(this->drive_motor_thread_fun,motor_pin_2,cur_ratio_speed,motor_2);
        first_motor_thread.join();
        second_motor_thread.join();

        cout<<"current angle:"<<cur_angle<<"target andle:"<<target_angle<<"diff:"<<abs(target_angle-cur_angle)<<endl;

        if(abs(target_angle-cur_angle)<6)
        {
            delay(1000);
            break;
        }
    }
    curRobotStatue.motor1_speed=0;
    curRobotStatue.motor2_speed=0;
    curRobotStatue.motor3_speed=0;
    curRobotStatue.motor4_speed=0;
}

struct Robot_PID kinematicControl::MoveLateral(float target_angle,int side, float ratio_speed, float duration_ms,struct Robot_PID last_pid)
{
    //这一段用于实现在角度过大情况下的最优运动选择
    if(target_angle>90|| target_angle<-90)
    {
        int trans_side=1-side;
        float trans_ang;
        if(target_angle>90){
            trans_ang=180-target_angle;
        }
        else{
            trans_ang=180+target_angle;
        }
        this->MoveLateral(trans_ang,trans_side,ratio_speed,duration_ms);
        return last_pid;
    }
    float angle_diff,left_speed,right_speed;
    robotStatus cur_robot_statue;
    motor_c left_motor,right_motor;
//    left_motor.motor_setup();
    int motor_pin_left,motor_pin_right;

    struct timeval timerBreakStart,timerBreakEnd;
    gettimeofday(&timerBreakStart,NULL);
    long int startTime=timerBreakStart.tv_sec*1000+timerBreakStart.tv_usec/1000;

    //用于pid调速
    Robot_PID robot_pid=last_pid;
//    pidInit(&robot_pid,cur_robot_statue.k_p,cur_robot_statue.k_i,cur_robot_statue.k_d);
    float diff_speed=0;
    float last_diff_speed=0;
    long int lastTime=startTime;
    //以下用于读取机器人此刻的速度
    float last_left_speed=0;
    float last_right_speed=0;
    while(1){
        float cur_angle=cur_robot_statue.getCurAngleOfMPU();
        if (side==right_side){
            angle_diff=target_angle-cur_angle;
            motor_pin_left=motor1_pin;
            motor_pin_right=motor2_pin;
            cur_robot_statue.motor1_speed=last_left_speed;
            cur_robot_statue.motor2_speed=last_right_speed;
        }
        else if(side==left_side){
            angle_diff=target_angle-cur_angle;
            motor_pin_left=motor4_pin;
            motor_pin_right=motor3_pin;
            cur_robot_statue.motor3_speed=last_right_speed;
            cur_robot_statue.motor4_speed=last_left_speed;
        }
        angle_diff/=90.0;

        //当时间超过设定的时间，跳出循环，结束这部分的程序
        gettimeofday(&timerBreakEnd,NULL);
        long int endTime=timerBreakEnd.tv_sec*1000+timerBreakEnd.tv_usec/1000;
        if(endTime-startTime>duration_ms)break;
        //每隔100ms，进行一次pid速度的跟新
        float dt=0.1;
        if((endTime-lastTime)>dt*1000)
        {
            diff_speed=pidUpdate_err(&robot_pid,angle_diff,(endTime-lastTime)/1000.0);
            lastTime=endTime;
            last_diff_speed=diff_speed;
        }

        left_speed=ratio_speed+last_diff_speed;
        right_speed=ratio_speed-last_diff_speed;

        left_speed=std::max(std::min(left_speed,float(1)),float(0.01));
        right_speed=std::max(std::min(right_speed,float(1)),float(0.01));

        last_left_speed=left_speed;
        last_right_speed=right_speed;

        cout<<"current yaw...:"<<cur_angle<<" target_angle: "<<target_angle<<" left_speed:"<<left_speed<<"right speed:"<<right_speed<<"angle diff: "<<angle_diff<<"last diff speed"<<last_diff_speed<<endl;
        thread left_motor_thread(this->drive_motor_thread_fun,motor_pin_left,left_speed,left_motor);
        thread right_motor_thread(this->drive_motor_thread_fun,motor_pin_right,right_speed,right_motor);

        left_motor_thread.join();
        right_motor_thread.join();
    }
    cur_robot_statue.motor1_speed=0;
    cur_robot_statue.motor2_speed=0;
    cur_robot_statue.motor3_speed=0;
    cur_robot_statue.motor4_speed=0;

    return robot_pid;
}
