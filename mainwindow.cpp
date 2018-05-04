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

    this->is_stop=false;

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
    float setted_speed=ui->ratio_speed->text().toFloat();
    float setted_angle=ui->advanced_angle->text().toFloat();

//    cout<<"ratio_speed:"<<setted_speed<<endl;

    if(setted_speed>1)
    {
        setted_speed=1;
        ui->ratio_speed->setText("1");
    }
    else if(setted_speed<-1)
    {
        setted_speed=-1;
        ui->ratio_speed->setText("-1");
    }

    if(setted_angle>180){
        setted_angle=float(int(setted_angle)%180);
        ui->advanced_angle->setText(QString::number(setted_angle));

    }
    else if(setted_angle<-180)
    {
        setted_angle=float(int(setted_angle)%180);
        ui->advanced_angle->setText(QString::number(setted_angle));
    }
    this->ratio_speed_setted=setted_speed;
    this->ratio_angle_setted=setted_angle;

    return;
}

void MainWindow::move_tread(int move_type)
{
    kinematicControl kine_move;
    kine_move.motor_setup();
    while(1)
    {
        float angle_transfer=this->ratio_angle_setted;
        switch (move_type) {
        case forward_side:
            kine_move.MoveForward(this->ratio_angle_setted,this->ratio_speed_setted,1000);
            break;
        case backward_side:
            if(angle_transfer<0)
            {
                angle_transfer+=180;
            }
            else
            {
                angle_transfer-=180;
            }
            kine_move.MoveForward(angle_transfer,this->ratio_speed_setted,1000);
            break;
        case left_side:
            kine_move.MoveLateral(this->ratio_angle_setted,left_side,this->ratio_speed_setted,1000);
            break;
        case right_side:
            kine_move.MoveLateral(this->ratio_angle_setted,right_side,this->ratio_speed_setted,1000);
            break;
        default:
            break;
        }

        if(this->is_stop)
        {
            this->is_stop=false;
            break;
        }
    }
    ui->forward_move->setEnabled(true);
    ui->backward_move->setEnabled(true);
    ui->left_move->setEnabled(true);
    ui->right_move->setEnabled(true);
}
void MainWindow::stopping_all()
{
    while(1)
    {
        if(ui->forward_move->isEnabled()&&ui->backward_move->isEnabled()&&ui->left_move->isEnabled()&&ui->right_move->isEnabled()){
            this->is_stop=false;
            ui->forward_move->setEnabled(true);
            ui->backward_move->setEnabled(true);
            ui->left_move->setEnabled(true);
            ui->right_move->setEnabled(true);
            break;
        }
        else
        {
            this->is_stop=true;
        }
    }
    return;
}

void MainWindow::on_forward_move_clicked()
{
    stopping_all();
    ui->forward_move->setEnabled(false);
    std::thread forward_tread(&MainWindow::move_tread,this,forward_side);
    forward_tread.detach();
}

void MainWindow::on_stop_engine_clicked()
{
    this->is_stop=true;
}

void MainWindow::on_left_move_clicked()
{
    stopping_all();
    ui->left_move->setEnabled(false);
    std::thread left_move_thread(&MainWindow::move_tread,this,left_side);
    left_move_thread.detach();
}

void MainWindow::on_right_move_clicked()
{
    stopping_all();
    ui->right_move->setEnabled(false);
    std::thread right_move_thread(&MainWindow::move_tread,this,right_side);
    right_move_thread.detach();
}

void MainWindow::on_backward_move_clicked()
{
    stopping_all();
    ui->backward_move->setEnabled(false);
    std::thread back_move_thread(&MainWindow::move_tread,this,backward_side);
    back_move_thread.detach();
}
