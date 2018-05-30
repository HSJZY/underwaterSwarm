#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<thread>
#include <utility>
#include"kinematiccontrol.h"
#include"global_setting.h"
#include"camera.h"
#include"formation_control.h"
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void static capture_thread(camera obj_camera,int i,Mat& frame);
    void static formation_thread_fun(float ratio_angle,float ratio_distance);
private slots:
//    void on_pushButton_clicked();

    void on_open_camera_clicked();
    void videoUpdate();
    void video1Update();
    void video2Update();
    void video3Update();
    void video4Update();


//    void videoUpdate2();
    void robotStatusUpdate();

    void on_reset_parameter_clicked();

    void on_forward_move_clicked();

    void on_stop_engine_clicked();

    void move_tread(int move_type);

    void on_left_move_clicked();

    void on_right_move_clicked();

    void stopping_all();

    void on_backward_move_clicked();

    void on_self_rotate_clicked();

    void on_ratio_speed_textChanged(const QString &arg1);

    void on_reset_pid_clicked();

    void on_close_camera_clicked();

    void on_screen_shot_clicked();

    void on_pushButton_clicked();

    void on_btn_start_formation_clicked();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capture;
    QTimer *camera_timer;

    cv::Mat camera_frame;
    QImage camera_image;
    vector<Mat> camera_frames;

    vector<camera> vec_cameras;

    Mat camera1_img;
    Mat camera2_img;
    Mat camera3_img;
    Mat camera4_img;
    QTimer *camera_timer1;
    QTimer *camera_timer2;
    QTimer *camera_timer3;
    QTimer *camera_timer4;

    float ratio_speed_setted;
    float ratio_angle_setted;

    bool is_stop;
    bool is_able_shot;
    int cur_shot_nums;
};

#endif // MAINWINDOW_H
