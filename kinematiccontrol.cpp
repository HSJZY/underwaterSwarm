#include "kinematiccontrol.h"

kinematicControl::kinematicControl()
{

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

void kinematicControl::MoveForward(float target_angle, float ratio_speed,float duration_ms)
{
    robotStatus curRobotStatue;

    struct timeval timerBreakStart,timerBreakEnd;
    gettimeofday(&timerBreakStart,NULL);
    long int startTime=timerBreakStart.tv_sec*1000+timerBreakStart.tv_usec/1000;

    motor_c motor_left,motor_right;
    motor_left.motor_setup();
    while(1){
        float cur_angle=curRobotStatue.getCurAngleOfMPU();



        int motor_pin_left,motor_pin_right;
        float left_speed,right_speed,angle_diff;
        if(target_angle>=-90 &&target_angle<=90)
        {
            motor_pin_left=motor3_pin;
            motor_pin_right=motor1_pin;
            angle_diff=target_angle-cur_angle;
        }
        else
        {
            motor_pin_left=motor2_pin;
            motor_pin_right=motor4_pin;

            if (target_angle<0){target_angle+=360;}
            float cur_angle_tranfer=cur_angle+180;
            angle_diff=target_angle-cur_angle_tranfer;
        }
        float kp=0.2;
        left_speed=ratio_speed-angle_diff/90.0*kp;
        right_speed=ratio_speed+angle_diff/90.0*kp;

        cout<<"current yaw...:"<<cur_angle<<"left_speed:"<<left_speed<<"right speed:"<<right_speed<<endl;
        thread left_motor_thread(this->drive_motor_thread_fun,motor_pin_left,left_speed,motor_left);
        thread right_motor_thread(this->drive_motor_thread_fun,motor_pin_right,right_speed,motor_right);

        left_motor_thread.join();
        right_motor_thread.join();

        gettimeofday(&timerBreakEnd,NULL);
        long int endTime=timerBreakEnd.tv_sec*1000+timerBreakEnd.tv_usec/1000;
        if(endTime-startTime>duration_ms)break;
    }
}

void kinematicControl::SelfRotate(float target_angle)
{
    robotStatus curRobotStatue;
    motor_c motor_1,motor_2;
    motor_1.motor_setup();
    float ratio_speed=0.6;
    while(1)
    {

        int motor_pin_1=motor1_pin;
        int motor_pin_2=motor4_pin;
        thread first_motor_thread(this->drive_motor_thread_fun,motor_pin_1,ratio_speed,motor_1);
        thread second_motor_thread(this->drive_motor_thread_fun,motor_pin_2,ratio_speed,motor_2);
        first_motor_thread.join();
        second_motor_thread.join();

        float cur_angle=curRobotStatue.getCurAngleOfMPU();

        cout<<"current angle:"<<cur_angle<<"target andle:"<<target_angle<<"diff:"<<abs(target_angle-cur_angle)<<endl;

        if(abs(target_angle-cur_angle)<2)
            break;
    }
}

void kinematicControl::MoveLateral(float target_angle,int side, float ratio_speed, float duration_ms)
{
//    float target_angle;
    float angle_diff,left_speed,right_speed;
    robotStatus cur_robot_statue;
    motor_c left_motor,right_motor;
    left_motor.motor_setup();
    int motor_pin_left,motor_pin_right;
    while(1){
        float cur_angle=cur_robot_statue.getCurAngleOfMPU();
        if (side==right_side){
            angle_diff=target_angle-cur_angle;
            motor_pin_left=motor1_pin;
            motor_pin_right=motor2_pin;
        }
        else if(side==left_side){
            angle_diff=target_angle-cur_angle;
            motor_pin_left=motor4_pin;
            motor_pin_right=motor3_pin;
        }

        float kp=0.2;
        left_speed=ratio_speed-angle_diff/90.0*kp;
        right_speed=ratio_speed+angle_diff/90.0*kp;

        cout<<"current yaw...:"<<cur_angle<<"left_speed:"<<left_speed<<"right speed:"<<right_speed<<endl;
        thread left_motor_thread(this->drive_motor_thread_fun,motor_pin_left,left_speed,left_motor);
        thread right_motor_thread(this->drive_motor_thread_fun,motor_pin_right,right_speed,right_motor);

        left_motor_thread.join();
        right_motor_thread.join();
    }
}
