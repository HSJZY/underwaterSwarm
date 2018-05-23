#include"test_program.h"

void test_pnp()
{
    Mat frame0=imread("/home/pi/Desktop/uitest/images/img71.jpg"); //读入图片
    imageProcess img_process;
    Mat res_frame;
    vector<vector<float> > res= img_process.getDistanceFromImage(frame0,res_frame);
    cout<<"pasuing...";
}
void test_formation_alg()
{
    float direction_angle=0;
    float inter_distance=1000;
    vector<vector<float>> vec_total;
    vector<float> vec_single;
    vec_single.push_back(-20);
    vec_single.push_back(-28);
    vec_single.push_back(214);
    vec_total.push_back(vec_single);

    vec_single.clear();
    vec_single.push_back(-20);
    vec_single.push_back(-28);
    vec_single.push_back(214);
    vec_total.push_back(vec_single);

    vec_single.clear();
    vec_single.push_back(20);
    vec_single.push_back(-28);
    vec_single.push_back(214);
    vec_total.push_back(vec_single);

    vec_single.clear();
    vec_single.push_back(20);
    vec_single.push_back(-68);
    vec_single.push_back(254);
    vec_total.push_back(vec_single);
    formation_control formation;
    formation.line_formation_control_for_test(direction_angle,inter_distance,vec_total );

}