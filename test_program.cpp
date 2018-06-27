#include"test_program.h"

void test_pnp()
{
    Mat frame0=imread("/home/pi/Desktop/underwaterSwarm/images/saved_img/test_image/contain_distance/img4.jpg"); //读入图片
    Mat framw_resized;

//    resize(frame0,framw_resized,Size(640,480));
    imageProcess img_process;
    Mat res_frame;
    vector<vector<float> > res= img_process.getDistanceFromImage(frame0,res_frame);
    cout<<"pasuing...";
}
void test_formation_alg()
{
    float direction_angle=0;
    float inter_distance=600;
    vector<vector<float>> vec_total;
    vector<float> vec_single;
    vec_single.push_back(-20);
    vec_single.push_back(-28);
    vec_single.push_back(214);
    vec_total.push_back(vec_single);

//    vec_single.push_back(-20);
//    vec_single.push_back(-28);
//    vec_single.push_back(160);
//    vec_total.push_back(vec_single);

//    vec_single.clear();
//    vec_single.push_back(-20);
//    vec_single.push_back(-28);
//    vec_single.push_back(214);
//    vec_total.push_back(vec_single);


//    vec_single.clear();
//    vec_single.push_back(20);
//    vec_single.push_back(-68);
//    vec_single.push_back(254);
//    vec_total.push_back(vec_single);
    formation_control formation;
    formation.line_formation_control_for_test(direction_angle,inter_distance,vec_total );

}
void test_split_string()
{
    string s="marker1_pos:(2,3,4).marker2_pos:None.marker3_pos:None.marker4_pos:None.marker5_pos:(1,2,3);(4,5,6).";
    vector<string> vec_res;
    split_string(s,vec_res,".");
    return;
}
void test_udp()
{
    string address="192.168.1.101";
    int port=5000;
    udp_client udp_test(address,port);
    string res= udp_test.start_listen();
    cout<<"res:"<<res;
}

void test_parse_str()
{
    string parse_str="marker1_pos:(2,3,4).marker2_pos:None.marker3_pos:None.marker4_pos:None.marker5_pos:(1,2,3);(4,5,6).";
    parse_agents_position(parse_str);
}

void test_line_formation()
{
    line_formation_control test_line_formation;
    test_line_formation.start_line_formation();
}

void test_potential_field_two_points()
{
    float in_range=200;
    vector<float> self={100,100};
    vector<float> obstacle={0,0};
    potential_field_two_point(self,obstacle,in_range);

}


void test_rep_potential_field_line_formation()
{
    line_formation_control line_control;
    vector<vector<float> > boundary={{0,0},{3000,0}};
    vector<float> self_pos={200,300};
//    vector<float> rep=line_control.artifical_potential_rep_field(boundary,self_pos,false);
    cout<<endl;
}


