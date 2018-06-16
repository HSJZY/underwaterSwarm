#ifndef LINE_FORMATION_CONTROL_H
#define LINE_FORMATION_CONTROL_H
#include<global_setting.h>
#include<robotStatus.h>
#include<iostream>
#include<vector>
#include<robotStatus.h>
#include<algorithm>
#include<math.h>
#include"kinematiccontrol.h"
#include"motor.h"

using namespace std;

class line_formation_control
{
public:
    line_formation_control();
    line_formation_control(float direction,float inter_distance);
    void start_line_formation();
private:
    vector<vector<float> > calc_boundary(vector<vector<vector<float> > > agents_position);
    vector<vector<vector<float> > > subtract_one_dim(vector<vector<vector<float> > > agents_postion_3D,int dim);
    vector<vector<float> > calc_relative_pos(vector<vector<float> > abs_pos,vector<float> original);

    vector<vector<float> > get_agents_position(vector<vector<vector<float> > > agents_postion_2D);

    vector<vector<float> > convert_2D_dist_ang(vector<vector<float> > relative_pos);
    vector<vector<float> > choose_nearest_two_neighbors_line(vector<vector<float> > vec_total_dis_angle,float direction_angle);
    vector<float> calc_target_dist_direction(vector<vector<float> > two_nearby);
    void start_moving(vector<float> target_dist_ang,struct Robot_PID& successed_pid,int& last_drive_mode);

private:
    float m_direction_angle;
    float m_inter_distance;
};

#endif // LINE_FORMATION_CONTROL_H
