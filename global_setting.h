#ifndef GLOBAL_SETTING_H
#define GLOBAL_SETTING_H

#define PI 3.1415926
#define motor1_pin 0
#define motor2_pin 1
#define motor3_pin 2
#define motor4_pin 3

//以下用来调整电机的连接测试
#define motor1_clockwise_multi 1
#define motor2_clockwise_multi 1
#define motor3_clockwise_multi 1
#define motor4_clockwise_multi 1

//运动方向定义
#define left_side 0
#define right_side 1
#define forward_side 2
#define backward_side 3
#define rotate_side 4

//定义四个摄像头的位置
#define num_cameras 4
#define camera_pos1 -90
#define camera_pos2 90
#define camera_pos3 0
#define camera_pos4 0

//定义球形机器人外壳尺寸
#define radius_robot 80

#define robot_id 3
#define num_robots 4



#endif // GLOBAL_SETTING_H
