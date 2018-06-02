#ifndef FORMATION_CONTROL_H
#define FORMATION_CONTROL_H
#include<math.h>
#include<string>
#include"camera.h"
#include"imageprocess.h"
#include"robotStatus.h"
#include"global_setting.h"
#include"kinematiccontrol.h"
#include"log.h"
using namespace std;

class formation_control
{
public:
    formation_control();
    void line_formation_control(float direction_angle,float inter_distance);
    void line_formation_control_for_test(float direction_angle,float inter_distance,vector<vector<float>> total_info);
    vector<vector<float>> calc_displacement_test(vector<vector<float>> frames);
private:
    vector<Mat>  grab_pictures();
    vector<vector<float>> calc_displacement(vector<Mat> frames);
    vector<vector<float>> choose_nearest_two_neighbors_line(vector<vector<float>> vec_total_dis_ang,float direction_angle);
    void start_move_line(vector<vector<float>>  both_nearest_agents,float direction_angle, float inter_distance);

    void neighbor2Log(vector<vector<float>> both_nearest_agents);
private:
    vector<Mat> cur_frames;

    string log_name="/home/pi/Desktop/underwaterSwarm/images/test_2.log";//定义日志文件名
};

#endif // FORMATION_CONTROL_H
