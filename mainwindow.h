#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

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

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capture;
    QTimer *camera_timer;
    cv::Mat camera_frame;
    QImage camera_image;

    float ratio_speed_setted;
    float ratio_angle_setted;
};

#endif // MAINWINDOW_H
