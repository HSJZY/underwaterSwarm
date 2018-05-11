#ifndef FORMATION_CONTROL_H
#define FORMATION_CONTROL_H
#include<math.h>
#include"camera.h"
#include"imageprocess.h"
#include"robotStatus.h"
#include"global_setting.h"
#include"kinematiccontrol.h"

class formation_control
{
public:
    formation_control();
    void line_formation_control(float direction_angle,float inter_distance);
private:
    vector<Mat>  grab_pictures();
    vector<vector<float>> calc_displacement(vector<Mat> frames);
    vector<vector<float>> choose_nearest_two_neighbors_line(vector<vector<float>> vec_total_dis_ang,float direction_angle);
    void start_move_line(vector<vector<float>>  both_nearest_agents,float direction_angle, float inter_distance);
private:
    vector<Mat> cur_frames;
};

#endif // FORMATION_CONTROL_H
