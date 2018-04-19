#include "imageprocess.h"

imageProcess::imageProcess()
{

}

void imageProcess::brightAdjust(Mat src, Mat dst, double dContrast, double dBright)
{
    int nVal;
    for (int nI = 0; nI<src.rows; nI++)
    {
        Vec3b* p1 = src.ptr<Vec3b>(nI);
        Vec3b* p2 = dst.ptr<Vec3b>(nI);
        for (int nJ = 0; nJ <src.cols; nJ++)
        {
            for (int nK = 0; nK < 3; nK++)
            {
                nVal = (int)(dContrast * p1[nJ][nK] + dBright);     //每个像素的每个通道的值都进行线性变换
                if (nVal < 0)
                    nVal = 0;
                if (nVal > 255)
                    nVal = 255;
                p2[nJ][nK] = nVal;
            }
        }
    }
}

void imageProcess::cacBoundRectRandomDirection(Mat eroDst, Mat frame0)
{
    vector<vector<Point>> contour;
    findContours(eroDst, contour, RETR_CCOMP , CHAIN_APPROX_SIMPLE); //在二值图像中寻找轮廓
    vector<Vec4i> hierarchy;
    vector<RotatedRect> minRect(contour.size());
    for(int i=0;i<contour.size();i++)
    {
        minRect[i]=minAreaRect(Mat(contour[i]));

    }
 //  Mat resultMat=Mat::zeros(eroDst.size(),CV_8UC3);
    for(int i=0;i<contour.size();i++)
    {
        Scalar color=CV_RGB(rand()&255,rand()&255,rand()&255);
        drawContours(frame0,contour,i,color,1,8,hierarchy);
        Point2f rect_points[4];
        minRect[i].points(rect_points);//计算二维矩形框的顶点
        for(int j=0;j<4;j++)
        {
            cout<<rect_points[j]<<endl;
           // line(resultMat,rect_points[j],rect_points[(j+1)%4],color,1,8);
            line(frame0,rect_points[j],rect_points[(j+1)%4],color,1,8);
        }
    }
   // imshow("resultMat1",resultMat);
    imshow("resultMat2",frame0);//在原图显示矩形框
    waitKey(0);
}

void imageProcess::getBinImage(Mat src1, Mat src2, Mat dst, int nThre)
{
    for (int nI = 0; nI<src1.rows; nI++)
    {
        uchar* pchar1 = src1.ptr<uchar>(nI);
        uchar* pchar2 = src2.ptr<uchar>(nI);
        uchar* pchar3 = dst.ptr<uchar>(nI);
        for (int nJ = 0; nJ <src1.cols; nJ++)
        {
            if (pchar1[nJ] - pchar2[nJ]> nThre)
            {
                pchar3[nJ] = 255;
            }
            else
            {
                pchar3[nJ] = 0;
            }
        }
    }
}

int imageProcess::test_image(Mat frame0)
{
    if(!frame0.data)
    {
        cout<<"unable to open"<<endl;
        return -1;
    }
    Size imgSize=frame0.size();
    Mat rawImg = Mat(imgSize, CV_8UC3);
    Mat dilDst = Mat(imgSize, CV_8UC1);
    Mat rImage = Mat(imgSize, CV_8UC1);
    Mat gImage = Mat(imgSize, CV_8UC1);
    Mat bImage = Mat(imgSize, CV_8UC1);
    Mat binary = Mat(imgSize, CV_8UC1);
    Mat eroDst= Mat(imgSize, CV_8UC1);
    brightAdjust(frame0, rawImg, 1, -120);  //每个像素每个通道的值都减去120
    Mat bgr[3];
    split(rawImg, bgr); //将三个通道的像素值分离
    bImage = bgr[0];
    gImage = bgr[1];
    rImage = bgr[2];
    getBinImage(gImage, rImage, binary, 40);    //如果像素G值-R值大于40，则返回的二值图像的值为255，否则为0
    dilate(binary, dilDst, Mat(), Point(-1,-1), 2);   //图像膨胀
    erode(dilDst, eroDst, Mat(), Point(-1,-1), 1);  //图像腐蚀，先膨胀在腐蚀属于闭运算
    imshow("dil_eroDst",eroDst);
    cacBoundRectRandomDirection(eroDst,frame0);
    return 0;
}
