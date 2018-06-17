#include "line_formation_control.h"

line_formation_control::line_formation_control()
{
    this->m_direction_angle=0;
    this->m_inter_distance=700;
}
line_formation_control::line_formation_control(float direction,float inter_distance)
{
    this->m_direction_angle=direction;
    this->m_inter_distance=inter_distance;
}

void line_formation_control::start_line_formation()
{
    robotStatus cur_robot_statue;
    kinematicControl kine_control;
    struct Robot_PID first_pid=kine_control.get_first_pid();
    int last_drive_side=5;
    while(1)
    {
        if (cur_robot_statue.get_formation_is_stop_state()==true)break;
        vector<vector<vector<float> > > agents_postion_3D=cur_robot_statue.get_agents_position();
        if(agents_postion_3D.size()==0) continue;
        vector<vector<vector<float> > > agents_postion_2D=subtract_one_dim(agents_postion_3D,1);
        vector<vector<float> > boundary_sorted=calc_boundary(agents_postion_2D);

        vector<vector<float> > agents_position=get_agents_position(agents_postion_2D);
        if(boundary_sorted.empty())
        {
            cerr<<"检测出错啦！！！！";
            break;
        }
        vector<vector<float> > agents_relative_self_2D=calc_relative_pos(agents_position,agents_position[robot_id-1]);
        vector<vector<float> > agents_dist_ang=convert_2D_dist_ang(agents_relative_self_2D);
        vector<vector<float>> neighbor_2_agents=choose_nearest_two_neighbors_line(agents_dist_ang,this->m_direction_angle);
        vector<float> target_dist_ang=calc_target_dist_direction(neighbor_2_agents);
        start_moving(target_dist_ang,first_pid,last_drive_side);
        cout<<"pause...";
    }
}

vector<vector<float> > line_formation_control::get_agents_position(vector<vector<vector<float> > > agents_postion_2D)
{
    vector<vector<float> > agents_postion;
     for(int i=0;i<agents_postion_2D.size()-1;i++)
     {
         for(int j=0;j<agents_postion_2D[i].size();j++)
         {
             agents_postion.push_back(agents_postion_2D[i][j]);
         }
     }
     return agents_postion;
}

vector<vector<vector<float> > > line_formation_control::subtract_one_dim(vector<vector<vector<float> > > agents_postion_3D, int dim)
{
    vector<vector<vector<float> > >  agents_postion_subtracted;
    for(int i=0;i<agents_postion_3D.size();i++)
    {
        vector<vector<float> > agent_i_2D;
        for(int j=0;j<agents_postion_3D[i].size();j++)
        {
            vector<float> agent_2D;
            for(int k=0;k<agents_postion_3D[i][j].size();k++)
            {
                if(k!=dim)
                    agent_2D.push_back(agents_postion_3D[i][j][k]);
            }
            agent_i_2D.push_back(agent_2D);
        }
        agents_postion_subtracted.push_back(agent_i_2D);
    }
    return agents_postion_subtracted;
}

/// 傻瓜排序，我也是醉了！！！
/// \brief line_formation_control::calc_boundary
/// sort the agents postition to the sequence as follow
///   4..3    或者: ....  或者:   ....
///   .  .         .  .         .  .
///   1..2         1..2         1...
/// \param agents_position_2D
/// \return [1,2,3,4]或者[1,2]
///
vector<vector<float> > line_formation_control::calc_boundary(vector<vector<vector<float> > > agents_position_2D)
{
    vector<vector<float> > boundary_pos=agents_position_2D[agents_position_2D.size()-1];//只适用于4个或2个的情况
    vector<vector<float> > boundary_sorted;
    if(boundary_pos.size()==0)
    {
        cerr<<"without boundary";
        return boundary_sorted;
    }

    if(boundary_pos.size()==1)
    {
        return boundary_pos;
    }
    else if(boundary_pos.size()==2)
    {
        if(boundary_pos[0][0]<boundary_pos[1][0])
        {
            boundary_sorted.push_back(boundary_pos[0]);
            boundary_sorted.push_back(boundary_pos[1]);
        }
        else
        {
            boundary_sorted.push_back(boundary_pos[1]);
            boundary_sorted.push_back(boundary_pos[0]);
        }
        return boundary_sorted;
    }
    else if(boundary_pos.size()==4)
    {
        vector<float> bound_1;
        vector<float> bound_2;
        vector<float> bound_3;
        vector<float> bound_4;
        int min_x_1=0,min_x_2=1;

        for(int i=2;i<4;i++)
        {
            if(boundary_pos[min_x_1][0]>boundary_pos[min_x_2][0])
            {
                int tmp=min_x_1;
                min_x_1=min_x_2;
                min_x_2=tmp;
            }
            if(boundary_pos[i][0]<boundary_pos[min_x_2][1])
            {
                min_x_2=i;
            }
        }
        if(boundary_pos[min_x_1][1]<boundary_pos[min_x_2][1])
        {
            bound_1=boundary_pos[min_x_1];
            bound_4=boundary_pos[min_x_2];
        }
        else
        {
            bound_1=boundary_pos[min_x_2];
            bound_4=boundary_pos[min_x_1];
        }
        vector<vector<float> > pos_2_3;
        for(int i=0;i<4;i++)
        {
            if(i!=min_x_1&&i!=min_x_2)
            {
                pos_2_3.push_back(boundary_pos[i]);
            }
        }
        if(pos_2_3[0][1]<pos_2_3[1][1])
        {
            bound_2=pos_2_3[0];
            bound_3=pos_2_3[1];
        }
        else
        {
            bound_2=pos_2_3[1];
            bound_3=pos_2_3[0];
        }
        boundary_sorted.push_back(bound_1);
        boundary_sorted.push_back(bound_2);
        boundary_sorted.push_back(bound_3);
        boundary_sorted.push_back(bound_4);
        return boundary_sorted;
    }
    else
    {
        return boundary_sorted;
    }
}

vector<vector<float> > line_formation_control::calc_relative_pos(vector<vector<float> > abs_pos, vector<float> original)
{
    vector<vector<float> > relative_pos;
    for(int i=0;i<abs_pos.size();i++)
    {
        vector<float> relative_one_point;
        for(int j=0;j<abs_pos[i].size();j++)
        {
            relative_one_point.push_back(abs_pos[i][j]-original[j]);
        }
        relative_pos.push_back(relative_one_point);
    }
    return relative_pos;
}

vector<vector<float> > line_formation_control::convert_2D_dist_ang(vector<vector<float> > relative_pos)
{
    vector<vector<float> > vec_2D_dist_ang;
    for(int i=0;i<relative_pos.size();i++)
    {
        // remove it_self
        if(i==robot_id-1)
            continue;
        vector<float> single_dist_ang;
        float angle=atan2(relative_pos[i][1],relative_pos[i][0]);
        if(angle>=0 && angle<=PI)
        {
            angle=angle-PI/2.0;
//            if(relative_pos[i][0]>=0)
//            {
//                angle=-(PI/2.0-angle);
//            }
//            else
//            {
//                angle=-PI/2.0+angle;
//            }
        }
        else
        {
            if(relative_pos[i][0]<=0)
            {
                angle=angle+3*PI/2.0;
            }
            else
            {
                angle=angle-PI/2;
            }
        }

        float distance=sqrt(pow(relative_pos[i][0],2)+pow(relative_pos[i][1],2));
        single_dist_ang.push_back(distance);
        single_dist_ang.push_back(angle);
        vec_2D_dist_ang.push_back(single_dist_ang);
    }
    return vec_2D_dist_ang;
}

//挑选出直线编队中左右最近的机器人
vector<vector<float>> line_formation_control::choose_nearest_two_neighbors_line(vector<vector<float> > vec_total_dis_angle,float direction_angle)
{
    vector<vector<float>> both;
    vector<float> nearest_left;
    vector<float> nearest_right;
    //设定最小值
    float nearest_left_x=1000000000;
    float nearest_right_x=1000000000;
    robotStatus cur_robot_status;
//    float cur_yaw=cur_robot_status.getCurAngleOfMPU();

    for(int i=0;i<vec_total_dis_angle.size();i++)
    {
        float cur_distance=vec_total_dis_angle[i][0];
        float cur_angle_org=vec_total_dis_angle[i][1];
        //给定指定方向，计算当前点相对于给定方向的角度
        float cur_angle=cur_angle_org+(-direction_angle)/180.0*PI;
        //以下cur_x代表垂直于给定方位上的分量，cur_y代表给定方向上的分量
        float cur_x=-cur_distance*sin(cur_angle);
//        float cur_y=cur_distance*cos(cur_angle);
        if(cur_angle>0)
        {
            //此时是在左边的情况
            if(abs(cur_x)<nearest_left_x)
            {
                nearest_left_x=abs(cur_x);
                if(nearest_left.size()==2)
                {
                    nearest_left[0]=cur_distance;
                    nearest_left[1]=cur_angle_org;
                }
                else
                {
                    nearest_left.push_back(cur_distance);
                    nearest_left.push_back(cur_angle_org);
                }
            }
        }
        else
        {
            //此时是在右边的情况
            if(abs(cur_x)<nearest_right_x)
            {
                nearest_right_x=abs(cur_x);
                if(nearest_right.size()==2)
                {
                    nearest_right[0]=cur_distance;
                    nearest_right[1]=cur_angle_org;
                }
                else
                {
                    nearest_right.push_back(cur_distance);
                    nearest_right.push_back(cur_angle_org);
                }
            }
        }
    }
    both.push_back(nearest_left);
    both.push_back(nearest_right);
    return both;
}

vector<float> line_formation_control::calc_target_dist_direction(vector<vector<float> > two_nearby)
{
    float move_x,move_y;
    vector<float> nearest_left=two_nearby[0];
    vector<float> nearest_right=two_nearby[1];
    vector<float> target_dist_ang;

//    nearest_right[0]=876.920105;
//    nearest_right[1]=-1.344329;

    if(nearest_left.empty()&&nearest_right.empty())
    {
        //此时该机器人属于失联状态
        return target_dist_ang;
    }
    else if(nearest_left.empty())
    {
        //此时机器人在最左侧，切换下坐标系，以其右边机器人为原点
        float self_x=-nearest_right[0]*sin(abs(nearest_right[1]));
        float self_y=-nearest_right[0]*cos(nearest_right[1]);
        float target_x=-(this->m_inter_distance)*cos(this->m_direction_angle);
        float target_y=(this->m_inter_distance)*sin(-this->m_direction_angle);
        move_x=target_x-self_x;
        move_y=target_y-self_y;
    }
    else if(nearest_right.empty())
    {
        //此时机器人在最右侧,切换下坐标系，以其右边机器人为原点
        float self_x=nearest_left[0]*sin(nearest_left[1]);
        float self_y=-nearest_left[0]*cos(nearest_left[1]);
        float target_x=this->m_inter_distance*cos(this->m_direction_angle);
        float target_y=this->m_inter_distance*sin(this->m_direction_angle);
        move_x=target_x-self_x;
        move_y=target_y-self_y;
    }
    else
    {
        //此时机器人两侧都有邻近机器人,以自身为坐标系
        float target_x=(nearest_right[0]*abs(sin(nearest_right[1]))-nearest_left[0]*sin(nearest_left[1]))/2;
        float target_y=(nearest_left[0]*cos(nearest_left[1])+nearest_right[0]*cos(nearest_right[1]))/2.0;
        move_x=target_x;
        move_y=target_y;
    }
    float target_dist=sqrt(pow(move_x,2)+pow(move_y,2));
    float target_ang=atan2(move_y,move_x);
    if(move_x>0&&move_y>0)
    {
        //第一象限
        target_ang=target_ang-1/2.0*PI;
    }
    else if(move_x>0&&move_y<0)
    {
        //第四象限
        target_ang=target_ang-PI/2;
    }
    else if(move_x<0&&move_y>0)
    {
        //第二象限
        target_ang=target_ang-PI/2;
    }
    else
    {
        //第三象限
        target_ang=3*PI/2+target_ang;
    }

    target_dist_ang.push_back(target_dist);
    target_dist_ang.push_back(target_ang);
    return target_dist_ang;
}

void line_formation_control::start_moving(vector<float> target_dist_ang,struct Robot_PID& successed_pid,int& last_drive_side)
{
    kinematicControl kine_control;
    float target_distance=target_dist_ang[0];
    float target_angle=target_dist_ang[1]*180/PI;//convert to degree
    robotStatus cur_robot_status;

    if(target_distance<=50)
    {
        if(last_drive_side!=5)
            kine_control.back_rotate_two_motor(last_drive_side,0.2,1000);
        last_drive_side=5;
        return;
    }
    int move_side;
    if(target_angle<-45 &&target_angle>=-135)
    {
        move_side=right_side;
    }
    else if(target_angle>=-45 && target_angle<=45)
    {
        move_side=forward_side;
    }
    else if(target_angle>45&&target_angle<135)
    {
        move_side=left_side;
    }
    else
    {
        move_side=backward_side;
    }

    float move_ratio_speed=std::min(target_distance,float(1000))/1000*0.5;
    float move_angle;

    if(last_drive_side!=move_side)
    {
        motor_c motor;
        switch (move_side) {
        case left_side:
            if(cur_robot_status.get_if_motor_is_sleep(motor3_pin))
                motor.single_motor_setup(motor3_pin);
            if(cur_robot_status.get_if_motor_is_sleep(motor4_pin))
                motor.single_motor_setup(motor4_pin);
            break;
        case right_side:
            if(cur_robot_status.get_if_motor_is_sleep(motor1_pin))
                motor.single_motor_setup(motor1_pin);
            if(cur_robot_status.get_if_motor_is_sleep(motor2_pin))
                motor.single_motor_setup(motor2_pin);
            break;
        case forward_side:
            if(cur_robot_status.get_if_motor_is_sleep(motor1_pin))
                motor.single_motor_setup(motor1_pin);
            if(cur_robot_status.get_if_motor_is_sleep(motor3_pin))
                motor.single_motor_setup(motor3_pin);
            break;
        case backward_side:
            if(cur_robot_status.get_if_motor_is_sleep(motor2_pin))
                motor.single_motor_setup(motor2_pin);
            if(cur_robot_status.get_if_motor_is_sleep(motor4_pin))
                motor.single_motor_setup(motor4_pin);
            break;
        default:
            break;
        }
    }

    switch (move_side) {
    case left_side:
        move_angle=target_angle-90;
        successed_pid=kine_control.MoveLateral(move_angle,left_side,move_ratio_speed,101,successed_pid);
        break;
    case right_side:
        move_angle=target_angle+90;
        successed_pid=kine_control.MoveLateral(move_angle,left_side,move_ratio_speed,101,successed_pid);
        break;
    case forward_side:
        successed_pid=kine_control.MoveForward(target_angle,move_ratio_speed,101,successed_pid);
        break;
    case backward_side:
        successed_pid=kine_control.MoveForward(target_angle,move_ratio_speed,101,successed_pid);
        break;
    default:
        break;
    }
    last_drive_side=move_side;

}
