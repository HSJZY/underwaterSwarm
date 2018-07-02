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
#include"utils.h"

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
//    test_split_string();
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
    curCarStatue.setAbsAngleOfMPU(-lastYaw,true);

    std::cout<<"press any char to continue"<<std::endl;
    //cv::waitKey(0);
    //int key;
    //cin>>key;
    std::cout<<"testing";
    thread mpuThread(MPUThread);
    mpuThread.detach();
}

void listen_motor()
{
    robotStatus listen_robot_status;
    listen_robot_status.set_if_motor_is_sleep(motor1_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor2_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor3_pin,true);
    listen_robot_status.set_if_motor_is_sleep(motor4_pin,true);
}

void drive_motor_11(int motor_id,float speed,motor_c motor_1)
{
        switch (motor_id) {
        case 1:
            motor_1.drive_motor(motor1_pin,motor1_clockwise_multi,speed);
            break;
        case 2:
            motor_1.drive_motor(motor2_pin,motor2_clockwise_multi,speed);
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
void update_postion_thread(string addr,int port)
{
    udp_client udp_listener(addr,port);
    robotStatus cur_robot_status;
    while(1)
    {
        string recv_formation=udp_listener.start_listen();
        if(recv_formation=="")
        {
            std::cerr<<"connenction error";
            continue;
        }
        vector<vector<vector<float> > > vec_agents_position=parse_agents_position(recv_formation);
        cur_robot_status.set_agents_position(vec_agents_position);
        delay(10);
    }
    //哈，这一句的监听是我额外加的,和函数名无关，图方便
    cur_robot_status.listen_motors();
}

void init_robot_status()
{
    robotStatus cur_robot_status;
    cur_robot_status.setKp(1.2);
    cur_robot_status.setKi(0);
    cur_robot_status.setKd(0.1);

//    string addr="192.168.1.109";
//    int port=5000;
//    udp_client udp_test(addr,port);
//    udp_test.start_listen();
//    string recv_formation=udp_test.start_listen();
//    vector<vector<vector<float> > > vec_agents_position=parse_agents_position(recv_formation);
//    cur_robot_status.set_agents_position(vec_agents_position);

//    thread agents_postion_listen_thread(update_postion_thread,addr,port);
//    agents_postion_listen_thread.detach();
}

int main(int argc, char *argv[])
{
    wiringPiSetup();
    init_robot_status();

//    test_potential_field_two_points();
//    test_rep_potential_field_line_formation();

//    test_pnp();
//    test_udp();
//    test_parse_str();

    initMPU6050();
//    test_line_formation();
//    test_formation_alg();

//    motor_c motor1;

//    test_camera(0,300);

    ledLattice dianzhen;
    dianzhen.show();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


//    Mat frame0=imread("/home/pi/Desktop/underwaterSwarm/images/test_1.jpg"); //读入图片
//    imageProcess img_process;
//    Mat res_frame;
//    vector<vector<float> > res= img_process.getDistanceFromImage(frame0,res_frame);


//    Point2f
//    [215.712, 157.464]
//    [144.451, 88.5008]
//    [211.979, 18.7217]
//    [283.241, 87.6846]

//    kinematicControl kineTest;

//    kineTest.MoveLateral(0,right_side,0.4,1);
//      kineTest.SelfRotate(-60);
//    kineTest.motor_setup();
//    while(1)
//    {
//        kineTest.MoveForward(0,0.1,100000);
//        kineTest.switchMode();
// //        kineTest.MoveForward(120,0.4,10000);
// //        kineTest.switchMode();
//    }

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

