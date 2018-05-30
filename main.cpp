#include <QCoreApplication>
#include "global_setting.h"
#include"motor.h"
#include<wiringPi.h>
#include<thread>
#include <iostream>
#include"mpu/demo_dmp.h"
#include"robotStatus.h"
#include<math.h>
#include <opencv2/core.hpp>
#include"kinematiccontrol.h"
#include"ledlattice.h"
#include"imageprocess.h"
#include"camera.h"
#include"test_program.h"

#include "mainwindow.h"
#include <QApplication>
using namespace std;
using namespace cv;

void MPUThread()
{
    robotStatus curCarStatus;
    while(1)
    {
        float t_yaw=0;
        // the we multiply -1 to the loop since clockwise rotate is positive in my definition
        t_yaw=-loop();
        float yaw;
        if(t_yaw==365)
            continue;
        yaw=t_yaw;
        delay(20);
        curCarStatus.setAbsAngleOfMPU(yaw);
    }
}

void initMPU6050()
{
    setup();
    delay(1000);
    //float yaw=loop();
    //cout<<yaw;
    float lastYaw=0;
    float yaw=0;
    cout<<"justifying the MPU6050,please waiting...";
    while(abs(yaw-lastYaw)>0.05||lastYaw==0)
    {
        float t_yaw=0;
        t_yaw=loop();
        if(t_yaw==-365)
            continue;
        lastYaw=yaw;
        yaw=t_yaw;
        delay(1000);
        std::cout<<"yaw:"<<yaw<<std::endl;
    }
    robotStatus curCarStatue;
    curCarStatue.setInitAngleOfMPU(-lastYaw);
    curCarStatue.setAbsAngleOfMPU(-lastYaw);

    std::cout<<"press any char to continue"<<std::endl;
    //cv::waitKey(0);
    //int key;
    //cin>>key;
    std::cout<<"testing";
    thread mpuThread(MPUThread);
    mpuThread.detach();
}


//void listen_motor_thread()
//{
//    robotStatus listen_robot_status;

//    struct timeval timerBreakStart,timerBreakEnd;
//    gettimeofday(&timerBreakStart,NULL);
//    long int startTime1=timerBreakStart.tv_sec*1000+timerBreakStart.tv_usec/1000;
//    long int startTime2=startTime1;
//    long int startTime3=startTime1;
//    long int startTime4=startTime1;

//    while(1)
//    {
//        gettimeofday(&timerBreakEnd,NULL);
//        long int endTime=timerBreakEnd.tv_sec*1000+timerBreakEnd.tv_usec/1000;

//        float cur_motor1_speed=listen_robot_status.motor1_speed;
//        float cur_motor2_speed=listen_robot_status.motor2_speed;
//        float cur_motor3_speed=listen_robot_status.motor3_speed;
//        float cur_motor4_speed=listen_robot_status.motor4_speed;

//        if(cur_motor1_speed!=0.0)
//        {
//            listen_robot_status.set_if_motor_is_sleep(motor1_pin,false);
//        }
//        else
//        {
//            if(endTime-startTime1>=1000)
//            {
//                listen_robot_status.set_if_motor_is_sleep(motor1_pin,true);
//                startTime1=endTime;
//            }
//        }

//        if(cur_motor2_speed!=0.0)
//        {
//            listen_robot_status.set_if_motor_is_sleep(motor2_pin,false);
//        }
//        else
//        {
//            if(endTime-startTime2>=1000)
//            {
//                listen_robot_status.set_if_motor_is_sleep(motor2_pin,true);
//                startTime2=endTime;
//            }
//        }

//        if(cur_motor3_speed!=0.0)
//        {
//            listen_robot_status.set_if_motor_is_sleep(motor3_pin,false);
//        }
//        else
//        {
//            if(endTime-startTime3>=1000)
//            {
//                listen_robot_status.set_if_motor_is_sleep(motor3_pin,true);
//                startTime3=endTime;
//            }
//        }

//        if(cur_motor4_speed!=0.0)
//        {
//            listen_robot_status.set_if_motor_is_sleep(motor4_pin,false);
//        }
//        else
//        {
//            if(endTime-startTime4>=1000)
//            {
//                listen_robot_status.set_if_motor_is_sleep(motor4_pin,true);
//                startTime1=endTime;
//            }
//        }


//    }
//
//}

void listen_motor()
{
    robotStatus listen_robot_status;
    listen_robot_status.set_if_motor_is_sleep(motor1_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor2_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor3_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor4_pin,true);

//    thread listen_motor_thread(&listen_motor_thread);
//    listen_motor_thread.detach();
}

void drive_motor_11(int motor_id,float speed,motor_c motor_1)
{
        switch (motor_id) {
        case 1:
            motor_1.drive_motor(motor1_pin,motor1_clockwise_multi,speed);
//            motor_1.motor1_c(speed);
            break;
        case 2:
            motor_1.drive_motor(motor2_pin,motor2_clockwise_multi,speed);
//            motor_1.motor2_c(speed);
            break;
        case 3:
            motor_1.drive_motor(motor3_pin,motor3_clockwise_multi,speed);
            break;
        case 4:
            motor_1.drive_motor(motor4_pin,motor4_clockwise_multi,speed);
            break;
        default:
            break;
        }
}

void test_camera(int ID,int num_pictures)
{
    camera camera1(ID);
//    camera1.CloseCamera();
    camera1.OpenCamera();
    for(int i=0;;i++)
    {
//

        Mat frame=camera1.CapturePicture();
//        break;
//        if(!frame)break;

        imshow("frame:",frame);
        string str_img="//home//pi//Desktop/underwaterSwarm//img_cal//image"+std::to_string(i)+".jpg";
        imwrite(str_img,frame);
        cv::waitKey(30);
    }
    camera1.CloseCamera();
}

int main(int argc, char *argv[])
{
    wiringPiSetup();
//    test_formation_alg();

//    motor_c motor1;
    initMPU6050();
//    test_camera(0,300);

    ledLattice dianzhen;
    dianzhen.show();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


    Mat frame0=imread("/home/pi/Desktop/underwaterSwarm/images/test_1.jpg"); //读入图片
    imageProcess img_process;
    Mat res_frame;
    vector<vector<float> > res= img_process.getDistanceFromImage(frame0,res_frame);
//    Point2f
//    [215.712, 157.464]
//    [144.451, 88.5008]
//    [211.979, 18.7217]
//    [283.241, 87.6846]
    kinematicControl kineTest;
//    kineTest.MoveLateral(0,right_side,0.4,1);
//      kineTest.SelfRotate(-60);
    kineTest.motor_setup();
    while(1)
    {
        kineTest.MoveForward(0,0.1,100000);
        kineTest.switchMode();
//        kineTest.MoveForward(120,0.4,10000);
//        kineTest.switchMode();
    }

    /*motor_c motor1,motor2,motor3,motor4;
    motor1.motor_setup();

//    motor1.motor_setup();
    while (1) {
        thread drive_motor_1(drive_motor_11,1,-0.2,motor1);
        thread drive_motor_2(drive_motor_11,2,-0.2,motor2);
        thread drive_motor_3(drive_motor_11,3,-0.2,motor3);
        thread drive_motor_4(drive_motor_11,4,-0.2,motor4);
        drive_motor_1.join();
        drive_motor_2.join();
        drive_motor_3.join();
        drive_motor_4.join();
        robotStatus statusTest;
        cout<<"yawing..."<<statusTest.getCurAngleOfMPU()<<endl;
    }*/
    return 0;
}

