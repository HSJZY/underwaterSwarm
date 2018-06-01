#include "camera.h"

camera::camera(int ID)
{
    this->m_ID=ID;
}

void camera::OpenCamera()
{
//    VideoCapture vedeo;
//    vedeo.open(0);
//    Mat img;
//    vedeo>>img;
//    imshow("img:",img);
//    waitKey(0);
//    return;
//    this->m_Capture.release();
    this->m_Capture.open(this->m_ID);
    this->m_Capture.set(cv::CAP_PROP_FRAME_WIDTH,352);
    this->m_Capture.set(cv::CAP_PROP_FRAME_HEIGHT,288);
    //Mat edge
//    int i=0;
//    while(i<100)
//    {
//        Mat frame;
//        this->m_Capture.grab();
//        this->m_Capture.retrieve(frame);
// //        this->m_Capture>>frame;
//        imshow("frame:",frame);
//        string str_img="//home//pi//Desktop/underwaterSwarm//img_cal//image"+std::to_string(i)+".jpg";
//        imwrite(str_img,frame);
//        cv::waitKey(0);
//        i++;
//
}

Mat camera::CapturePicture()
{
    Mat frame;
    this->m_Capture.grab();
    this->m_Capture.retrieve(frame);
    return frame;
}

void camera::CloseCamera()
{
    this->m_Capture.release();
//    this->m_Capture.
    this->m_Capture.~VideoCapture();
    //cvReleaseCapture(this->m_Capture);
//    this->m_Capture.release();
}
