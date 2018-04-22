#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include<vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<map>
#include<math.h>
#include<algorithm>
using namespace std;
using namespace cv;

class imageProcess
{
public:
    imageProcess();
    vector<vector<Point2f>> separateBeacon(vector<RotatedRect> minRect);
    vector<vector<float>> solveBeaconDistance();
    int test_image(Mat frame0);
    void brightAdjust(Mat src, Mat dst, double dContrast, double dBright); //亮度调节函数
    void getBinImage(Mat src1, Mat src2, Mat dst, int nThre); //二值化
    void cacBoundRectRandomDirection(Mat rlt,Mat frame0);
private:
    float calc_distance(Point2f verticle1,Point2f verticle2);
    float calc_area(Point2f verticle1,Point2f verticle2,Point2f verticle3);
};

#endif // IMAGEPROCESS_H
