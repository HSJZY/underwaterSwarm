#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<thread>
#include"kinematiccontrol.h"
#include"global_setting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_open_camera_clicked();
    void videoUpdate();

    void on_reset_parameter_clicked();

    void on_forward_move_clicked();

    void on_stop_engine_clicked();

    void move_tread(int move_type);

    void on_left_move_clicked();

    void on_right_move_clicked();

    void stopping_all();

    void on_backward_move_clicked();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capture;
    QTimer *camera_timer;
    cv::Mat camera_frame;
    QImage camera_image;

    float ratio_speed_setted;
    float ratio_angle_setted;

    bool is_stop;
};

#endif // MAINWINDOW_H
