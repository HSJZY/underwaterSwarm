#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace cv;
using namespace std;
struct Robot_PID kinematicControl::first_pid;
//float robotStatus::k_p;
//float robotStatus::k_i;
//float robotStatus::k_d;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera_timer = new QTimer(this);
    ui->advanced_angle->setText("0");
    ui->close_camera->setEnabled(false);
    ui->screen_shot->setEnabled(false);
    ui->cbx_formation_type->addItem("Line_Formation");

    QImage *img=new QImage;
    img->load("//home//pi//Desktop//uitest//images//robot_small.jpg");
    ui->label_3->setPixmap(QPixmap::fromImage(*img));

    this->ratio_angle_setted=0;
    this->ratio_speed_setted=0;
    this->is_stop=false;
    this->is_able_shot=false;
    this->cur_shot_nums=0;

    on_reset_pid_clicked();

    QTimer *status_timer=new QTimer(this);
    status_timer->start(30);
    connect(camera_timer,SIGNAL(timeout()),this,SLOT(videoUpdate()));
    connect(status_timer,SIGNAL(timeout()),this,SLOT(robotStatusUpdate()));
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
    for(int i=0;i<num_cameras;i++)
    {
        camera camera_i(i);
        camera_i.OpenCamera();
        vec_cameras.push_back(camera_i);
    }

//    capture.open(0);
    camera_timer->start(30);
    ui->close_camera->setEnabled(true);
    ui->screen_shot->setEnabled(true);
    ui->open_camera->setEnabled(false);
    cout<<"open camera "<<endl;
}
//读取摄像头并显示在窗口
void MainWindow::videoUpdate()
{
//    vector<Mat> camera_frames;
    this->camera_frames.clear();
    this->is_able_shot=false;
    for(int i=0;i<num_cameras;i++)
    {
        Mat camera_frame=vec_cameras[i].CapturePicture();
        this->camera_frames.push_back(camera_frame);
        if(!camera_frame.empty())
        {
            camera_image=cvMat2QImage(camera_frame);
            QImage camera_result=camera_image.scaled(ui->camera1->width(),ui->camera1->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation).rgbSwapped();
            switch (i) {
            case 0:
                ui->camera1->setPixmap(QPixmap::fromImage(camera_result));
                break;
            case 1:
                ui->camera2->setPixmap(QPixmap::fromImage(camera_result));
                break;
            case 2:
                ui->camera3->setPixmap(QPixmap::fromImage(camera_result));
                break;
            case 3:
                ui->camera4->setPixmap(QPixmap::fromImage(camera_result));
                break;
            default:
                break;
            }
        }
    }
    robotStatus cur_robot_status;
    cur_robot_status.set_cur_frames(camera_frames);
    this->is_able_shot=true;
}
//更新机器人当前状态的数据
void MainWindow::robotStatusUpdate()
{
    robotStatus cur_robot_status;
    float cur_yaw_status=cur_robot_status.getCurAngleOfMPU();
    ui->cur_yaw->setText(QString::number(cur_yaw_status,'f',2));
    ui->motor1_velocity->setText(QString::number(cur_robot_status.motor1_speed,'f',2));
    ui->motor2_velocity->setText(QString::number(cur_robot_status.motor2_speed,'f',2));
    ui->motor3_velocity->setText(QString::number(cur_robot_status.motor3_speed,'f',2));
    ui->motor4_velocity->setText(QString::number(cur_robot_status.motor4_speed,'f',2));
}

void MainWindow::on_reset_parameter_clicked()
{
    ui->reset_parameter->setEnabled(false);
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
    kine_move.switchMode();
    struct Robot_PID last_pid=kine_move.get_first_pid();
//    kine_move.motor_setup();
    while(1)
    {
        float angle_transfer=this->ratio_angle_setted;
        switch (move_type) {
        case forward_side:
            last_pid=kine_move.MoveForward(this->ratio_angle_setted,this->ratio_speed_setted,1000,last_pid);
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
            last_pid=kine_move.MoveForward(angle_transfer,this->ratio_speed_setted,1000,last_pid);
            break;
        case left_side:
            last_pid=kine_move.MoveLateral(this->ratio_angle_setted,left_side,this->ratio_speed_setted,1000,last_pid);
            break;
        case right_side:
            last_pid=kine_move.MoveLateral(this->ratio_angle_setted,right_side,this->ratio_speed_setted,1000,last_pid);
            break;
        case rotate_side:
            kine_move.SelfRotate(this->ratio_angle_setted);
            this->is_stop=true;
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
    switch (move_type) {
    case forward_side:
        ui->forward_move->setEnabled(true);
        break;
    case backward_side:
        ui->backward_move->setEnabled(true);
        break;
    case left_side:
        ui->left_move->setEnabled(true);
        break;
    case right_side:
        ui->right_move->setEnabled(true);
        break;
    case rotate_side:
        ui->self_rotate->setEnabled(true);
        break;
    default:
        break;
    }
    robotStatus cur_robot_status;
    cur_robot_status.setZeroToAllMotor();
}
void MainWindow::stopping_all()
{
    while(1)
    {
        if(ui->forward_move->isEnabled()&&ui->backward_move->isEnabled()&&ui->left_move->isEnabled()&&ui->right_move->isEnabled()&&ui->self_rotate->isEnabled()){
            this->is_stop=false;
            ui->forward_move->setEnabled(true);
            ui->backward_move->setEnabled(true);
            ui->left_move->setEnabled(true);
            ui->right_move->setEnabled(true);
            ui->self_rotate->setEnabled(true);
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

void MainWindow::on_self_rotate_clicked()
{
    stopping_all();
    ui->self_rotate->setEnabled(false);
    std::thread rotate_thread(&MainWindow::move_tread,this,rotate_side);
    rotate_thread.detach();
}

void MainWindow::on_ratio_speed_textChanged(const QString &arg1)
{
    ui->reset_parameter->setEnabled(true);
}

void MainWindow::on_reset_pid_clicked()
{
    robotStatus curRobotStatus;
    curRobotStatus.setKp(ui->k_p->text().toFloat());
    curRobotStatus.setKi(ui->k_i->text().toFloat());
    curRobotStatus.setKd(ui->k_d->text().toFloat());
    stopping_all();
}

void MainWindow::on_close_camera_clicked()
{
    cout<<"closing camera"<<endl;
    for(int i=0;i<num_cameras;i++)
    {
        vec_cameras[i].CloseCamera();
        switch (i) {
        case 0:
            ui->camera1->clear();
            break;
        case 1:
            ui->camera2->clear();
            break;
        case 2:
            ui->camera3->clear();
            break;
        case 3:
            ui->camera4->clear();
            break;
        default:
            break;
        }
    }
    vec_cameras.clear();
    ui->open_camera->setEnabled(true);
    ui->close_camera->setEnabled(false);
    ui->screen_shot->setEnabled(false);
    camera_timer->stop();
    cout<<"cmaeras are closed"<<endl;
}

void MainWindow::on_screen_shot_clicked()
{
    while(1)
    {
        if(this->is_able_shot==false) continue;
        for(int i=0;i<this->camera_frames.size();i++)
        {
            string str_img="//home//pi//Desktop//uitest//images//camera"+std::to_string(i+1)+"//img"+std::to_string(cur_shot_nums)+".jpg";
            imwrite(str_img,camera_frames[i]);
        }
        break;
    }
    this->cur_shot_nums+=1;
}

void MainWindow::on_btn_start_formation_clicked()
{
    if(ui->cbx_formation_type->currentText()=="Line_Formation")
    {
        formation_control line_formation_control;
        line_formation_control.line_formation_control(this->ratio_angle_setted,1000);
    }
}
