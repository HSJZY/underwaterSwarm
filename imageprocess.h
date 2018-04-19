#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include<vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class imageProcess
{
public:
    imageProcess();
    vector<float> SolveBeaconDistance();
    int test_image(Mat frame0);
    void brightAdjust(Mat src, Mat dst, double dContrast, double dBright); //亮度调节函数
    void getBinImage(Mat src1, Mat src2, Mat dst, int nThre); //二值化
    void cacBoundRectRandomDirection(Mat rlt,Mat frame0);

};

#endif // IMAGEPROCESS_H
