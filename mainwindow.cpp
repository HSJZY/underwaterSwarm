#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera_timer = new QTimer(this);
    ui->advanced_angle->setText("0");
    ui->close_camera->setEnabled(false);
    ui->screen_shot->setEnabled(false);

    this->ratio_angle_setted=0;
    this->ratio_speed_setted=0;

    connect(camera_timer,SIGNAL(timeout()),this,SLOT(videoUpdate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//Mat转换成Qimage1
QImage cvMat2QImage(Mat& mat)
{
    QImage* qimg = new QImage;
    *qimg = QImage((unsigned char*)mat.data, // uchar* data
            mat.cols, mat.rows, // width height
            QImage::Format_RGB888); //format存入格式为R, G, B 对应 0,1,2
    return *qimg;
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_open_camera_clicked()
{
    capture.open(0);
    camera_timer->start(30);
    ui->close_camera->setEnabled(true);
    ui->screen_shot->setEnabled(true);
    ui->open_camera->setEnabled(false);
    cout<<"open camera "<<endl;
}
//读取摄像头并显示在窗口
void MainWindow::videoUpdate()
{
    capture>>camera_frame;
    if(!camera_frame.empty())
    {
        camera_image=cvMat2QImage(camera_frame);
        QImage camera_result=camera_image.scaled(ui->camera1->width(),ui->camera1->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation).rgbSwapped();
        ui->camera1->setPixmap(QPixmap::fromImage(camera_result));
    }
}


void MainWindow::on_reset_parameter_clicked()
{
//    cout<<"ratio_speed:"<<this->ratio_speed_setted<<endl;
    float setted_speed=ui->ratio_speed->text().toFloat();
    float setted_angle=ui->advanced_angle->text().toFloat();
    if(setted_speed>1)
    {
        setted_speed=1;
        ui->ratio_speed->setText("1");
    }
    else if(setted_speed<-1)
    {
        setted_speed=-1;
        ui->advanced_angle->setText("-1");
    }
    this->ratio_speed_setted=setted_speed;
    this->ratio_angle_setted=setted_angle;
}
