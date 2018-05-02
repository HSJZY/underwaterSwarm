#ifndef CAMERA_H
#define CAMERA_H
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/video.hpp>
#include<opencv2/videoio.hpp>
using namespace cv;
using namespace  std;


class camera
{
public:
    camera(int ID);
    void OpenCamera();
    Mat CapturePicture();
    void CloseCamera();
private:
    int m_ID;
    VideoCapture m_Capture;
};

#endif // CAMERA_H
