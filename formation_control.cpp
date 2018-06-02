#include "formation_control.h"

formation_control::formation_control()
{
    Log log_file;
    log_file.Open(this->log_name);
//    log_file.CommonLogInit();
    log_file.Clear();
    log_file.Close();
    std::cout<<"duandian";
}

void formation_control::line_formation_control(float direction_angle, float inter_distance)
{
    while(1)
    {
        vector<vector<float>> total_dis_ang;
        total_dis_ang=calc_displacement(grab_pictures());
        vector<vector<float>> agents_both_sides=choose_nearest_two_neighbors_line(total_dis_ang,direction_angle);
        neighbor2Log(agents_both_sides);
        start_move_line(agents_both_sides,direction_angle,inter_distance);
    }
}

void formation_control::line_formation_control_for_test(float direction_angle, float inter_distance, vector<vector<float> > total_info)
{
    while(1)
    {
        vector<vector<float>> total_dis_ang;
        total_dis_ang=calc_displacement_test(total_info);
        vector<vector<float>> agents_both_sides=choose_nearest_two_neighbors_line(total_dis_ang,direction_angle);
        neighbor2Log(agents_both_sides);
        start_move_line(agents_both_sides,direction_angle,inter_distance);
    }
}
//计算当前机器人左右两端其他机器人相对于自身的位置信息
vector<vector<float>> formation_control::calc_displacement_test(vector<vector<float>> frames)
{
    vector<vector<float>> lattic_dis_ang;

    vector<vector<float>> img_displacement;


    img_displacement=frames;
    for(int j=0;j<img_displacement.size();j++)
    {
        vector<float> cur_dis_ang;
        //计算图片中点阵板相对于机器人运动的距离和角度位置
        float camera_pos;
        switch (j) {
        case 0:
            camera_pos=camera_pos1;
            break;
        case 1:
            camera_pos=camera_pos2;
        case 2:
            camera_pos=camera_pos3;
            break;
        case 3:
            camera_pos=camera_pos4;
        default:
            break;
        }

        float angle_lattic=-atan2(img_displacement[j][0],img_displacement[j][2])+camera_pos/180.0*PI;
        float distance_lattic=sqrt(pow(img_displacement[j][0],2)+pow(img_displacement[j][2],2))+radius_robot*1.8;//这里的1.8倍只是大概估计的值
        cur_dis_ang.push_back(distance_lattic);
        cur_dis_ang.push_back(angle_lattic);
        lattic_dis_ang.push_back(cur_dis_ang);
    }

    return lattic_dis_ang;
}


vector<Mat> formation_control::grab_pictures()
{
    robotStatus cur_robot_status;
    return(cur_robot_status.get_cur_frames());
}

//计算当前机器人左右两端其他机器人相对于自身的位置信息
vector<vector<float>> formation_control::calc_displacement(vector<Mat> frames)
{
    imageProcess img_process;
    vector<vector<float>> lattic_dis_ang;

    for(int i=0;i<frames.size();i++)
    {
        vector<vector<float>> img_displacement;
        Mat calc_frame=frames[i];
        Mat res_frame;
        if(!calc_frame.empty())
        {
            img_displacement=img_process.getDistanceFromImage(calc_frame,res_frame);
            for(int j=0;j<img_displacement.size();j++)
            {
                vector<float> cur_dis_ang;
                //计算图片中点阵板相对于机器人运动的距离和角度位置
                float camera_pos;
                switch (i) {
                case 0:
                    camera_pos=camera_pos1;
                    break;
                case 1:
                    camera_pos=camera_pos2;
                case 2:
                    camera_pos=camera_pos3;
                    break;
                case 3:
                    camera_pos=camera_pos4;
                default:
                    break;
                }

                float angle_lattic=-atan2(img_displacement[j][0],img_displacement[j][2])+camera_pos/180.0*PI;

                cout<<"test_atan2:"<<atan2(-4,140);
                float distance_lattic=sqrt(pow(img_displacement[j][0],2)+pow(img_displacement[j][2],2))+radius_robot*1.8;//这里的1.8倍只是大概估计的值
                cur_dis_ang.push_back(distance_lattic);
                cur_dis_ang.push_back(angle_lattic);
                lattic_dis_ang.push_back(cur_dis_ang);
                cout<<"target_distance_amgle"<<distance_lattic<<angle_lattic<<endl;
            }
        }
    }
     return lattic_dis_ang;
}
//挑选出直线编队中左右最近的机器人
vector<vector<float>> formation_control::choose_nearest_two_neighbors_line(vector<vector<float> > vec_total_dis_angle,float direction_angle)
{
    vector<vector<float>> both;
    vector<float> nearest_left;
    vector<float> nearest_right;
    //设定最小值
    float nearest_left_x=1000000000;
    float nearest_right_x=1000000000;
    robotStatus cur_robot_status;
    float cur_yaw=cur_robot_status.getCurAngleOfMPU();

    for(int i=0;i<vec_total_dis_angle.size();i++)
    {
        float cur_distance=vec_total_dis_angle[i][0];
        //给定指定方向，计算当前点相对于给定方向的角度
        float cur_angle=vec_total_dis_angle[i][1]+(cur_yaw-direction_angle)/180.0*PI;
        //以下cur_x代表垂直于给定方位上的分量，cur_y代表给定方向上的分量
        float cur_x=cur_distance*sin(cur_angle);
        float cur_y=cur_distance*cos(cur_angle);
        if(cur_angle>0)
        {
            //此时是在左边的情况
            if(abs(cur_x)<nearest_left_x)
            {
                nearest_left_x=abs(cur_x);
                if(nearest_left.size()==2)
                {
                    nearest_left[0]=cur_distance;
                    nearest_left[1]=cur_angle;
                }
                else
                {
                    nearest_left.push_back(cur_distance);
                    nearest_left.push_back(cur_angle);
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
                    nearest_right[1]=cur_angle;
                }
                else
                {
                    nearest_right.push_back(cur_distance);
                    nearest_right.push_back(cur_angle);
                }
            }
        }
    }
    both.push_back(nearest_left);
    both.push_back(nearest_right);
    return both;
}

void formation_control::start_move_line(vector<vector<float>> both_nearest_agents,float direction_angle, float inter_distance)
{
    float move_x,move_y;
    int move_lateral_side;
    kinematicControl kine_control;

    vector<float> nearest_left=both_nearest_agents[0];
    vector<float> nearest_right=both_nearest_agents[1];

    float middle_x,middle_y;

    if(nearest_left.empty()&&nearest_right.empty())
    {
        //此时该机器人属于失联状态,保持不动
        return;
    }
    else if(nearest_left.empty())
    {
        //此时机器人在最左侧
        middle_x=-inter_distance+(inter_distance+abs(nearest_right[0]*sin(nearest_right[1])))/2.0;
        middle_y=nearest_right[0]*cos(nearest_right[1]);
//        if(middle_x>0) move_lateral_side=
//        move_lateral_side=left_side;
    }
    else if(nearest_right.empty())
    {
        //此时机器人在最右侧
        middle_x=inter_distance-(inter_distance+abs(nearest_left[0]*sin(nearest_left[1])))/2.0;
//        middle_x=nearest_left[0]*sin(nearest_left[1])-inter_distance;
        middle_y=nearest_left[0]*cos(nearest_left[1]);
//        move_lateral_side=right_side;
    }
    else
    {
        //此时机器人两侧都有邻近机器人
        middle_x=(nearest_left[0]*sin(nearest_left[1])+nearest_right[0]*sin(nearest_right[1]))/2;
        middle_y=(nearest_left[0]*cos(nearest_left[1])+nearest_right[0]*cos(nearest_right[1]))/2;
    }
    if(middle_x<0)
    {
        //小于0说明当前机器人在中点的左边，需要向右边移动
        move_lateral_side=left_side;
    }
    else
    {
        //大于0部分，说明机器人在中点的右边，需要向左边移动
        move_lateral_side=right_side;
    }

    move_x=abs(middle_x);
    move_y=abs(middle_y);

    //插补的方式进行运动，先横向，后纵向
    float kp=3;
    float ratio_speed=0.4;
    float move_lateral_time=3000*move_x/10000*kp;
    float move_forward_time=3000*move_y/10000*kp;

    bool is_moved=false;
    //此处设定一个阈值为20mm，20mm之内不进行移动
    if(move_x>20)
    {
            kine_control.switchMode();
            kine_control.MoveLateral(direction_angle,move_lateral_side,ratio_speed,move_lateral_time);
            is_moved==true;
    }
    if(move_y<20)
    {
        if(is_moved==false)
        {
            delay(2000);
            kine_control.switchMode();
        }
        return;
    }
    else
    {
        if(middle_y>0)
        {
            kine_control.switchMode();
            kine_control.MoveForward(direction_angle,ratio_speed,move_forward_time);

        }
        else
        {
            float move_angle=direction_angle+180;
            if(move_angle>180)
            {
                move_angle-=360;
            }
            kine_control.switchMode();
            kine_control.MoveForward(move_angle,ratio_speed,move_forward_time);

        }
    }
}

void formation_control::neighbor2Log(vector<vector<float>> both_nearest_agents)
{
    Log log_file;
//    log_file.CommonLogInit();
    string file_name=this->log_name;
    log_file.Open(file_name);

    vector<float> nearest_left=both_nearest_agents[0];
    vector<float> nearest_right=both_nearest_agents[1];

    string current_time=log_file.GetTimeStr();
    log_file<<current_time;
    if(nearest_left.empty())
    {
        log_file<<" left: NULL NULL";

    }
    else
    {
        log_file<<" left: ";
        log_file<<to_string(nearest_left[0]);
        log_file<<" ";
        log_file<<to_string(nearest_left[1]);
    }

    if(nearest_right.empty())
    {
        log_file<<" right: NULL NULL";

    }
    else
    {
        log_file<<" right: ";
        log_file<<to_string(nearest_right[0]);
        log_file<<" ";
        log_file<<to_string(nearest_right[1]);
    }
    log_file.LogOutLn();
    log_file.Close();
}

