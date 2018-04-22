#include "imageprocess.h"
//#include<math.h>

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

float imageProcess::calc_distance(Point2f verticle1,Point2f verticle2)
{
    float distance=sqrt(pow(verticle1.x-verticle2.x,2)+pow(verticle1.y-verticle2.y,2));
    return distance;
}
float imageProcess::calc_area(Point2f verticle1, Point2f verticle2, Point2f verticle3)
{
    float dis_a=this->calc_distance(verticle1,verticle2);
    float dis_b=this->calc_distance(verticle2,verticle3);
    float dis_c=this->calc_distance(verticle1,verticle3);
    float s=(dis_a+dis_b+dis_c)/2.0;
    float area=sqrt(s*(s-dis_a)*(s-dis_b)*(s-dis_c));
    return area;
}

vector<vector<Point2f>> imageProcess::separateBeacon(vector<RotatedRect> minRect)
{
    vector<vector<Point2f>> all_rect;
    vector<vector<Point2f>> unique_rect;
    vector<float> area_rect;
    map<float,int> area2index;
    //存储给定的矩形框并计算出来相应的面积
    for(int i=0;i<minRect.size();i++)
    {
        vector<Point2f> cur_rect;
        Point2f rect_points[4];
        minRect[i].points(rect_points);//计算二维矩形框的顶点
        Point2f verticle_1=rect_points[0];
        cur_rect.push_back(verticle_1);

        vector<float> distance_2_other_verticle;
        for(int j=1;j<4;j++)
        {
            cur_rect.push_back(rect_points[j]);
            float distance_verticles=sqrt(pow(rect_points[j].x-verticle_1.x,2)+pow(rect_points[j].y-verticle_1.y,2));
            distance_2_other_verticle.push_back(distance_verticles);
        }
        all_rect.push_back(cur_rect);
        float cur_area;
        sort(distance_2_other_verticle.begin(),distance_2_other_verticle.end());

        cur_area=distance_2_other_verticle[0]*distance_2_other_verticle[1];
        area_rect.push_back(cur_area);
        pair<float,int> cur_area_2_index(cur_area,i);
        area2index.insert(cur_area_2_index);
    }
    sort(area_rect.begin(),area_rect.end(),greater<float>());

    for(int i=0;i<area_rect.size();i++)
    {
        map<float,int>::iterator iter=area2index.find(area_rect[i]);
        int index=iter->second;
//        float cur_area=area_rect[i];
        vector<Point2f> cur_rect=all_rect[index];//当前矩形框的四个交点
        if(unique_rect.empty()){
            unique_rect.push_back(cur_rect);
            continue;
        }
        //当判断当前矩形框上的点有没在唯一性矩阵里边，有的话pass
        bool is_in_unique=false;
        for(int j=0;j<unique_rect.size();j++)
        {
            vector<Point2f> cur_unique=unique_rect[j];
            float unique_area=calc_area(cur_unique[0],cur_unique[1],cur_unique[2])*2.0;

            //当前矩形框的四个角点需要一一比较
            for (int k=0;k<4;k++)
            {
                Point2f verticle=cur_rect[k];

                float area0=calc_area(verticle,cur_unique[0],cur_unique[1]);
                float area1=calc_area(verticle,cur_unique[1],cur_unique[2]);
                float area2=calc_area(verticle,cur_unique[2],cur_unique[3]);
                float area3=calc_area(verticle,cur_unique[0],cur_unique[3]);
                float area_sum=area0+area1+area2+area3;
                if(abs(area_sum-unique_area)<0.1)
                {
                    is_in_unique=true;
                    break;
                }
            }
            if(is_in_unique==false)
            {
                unique_rect.push_back(cur_rect);
            }
        }

    }
    return unique_rect;
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
    vector<vector<Point2f>> unique_rect=this->separateBeacon(minRect);
    for(int i=0;i<unique_rect.size();i++)
    {
        Scalar color=CV_RGB(rand()&255,rand()&255,rand()&255);
//        drawContours(frame0,contour,i,color,1,8,hierarchy);
        vector<Point2f> rect_points;
        rect_points=unique_rect[i];
//        minRect[i].points(rect_points);//计算二维矩形框的顶点
        for(int j=0;j<4;j++)
        {
            cout<<rect_points[j]<<endl;
           // line(resultMat,rect_points[j],rect_points[(j+1)%4],color,1,8);
            line(frame0,rect_points[j],rect_points[(j+1)%4],color,1,8);
        }
    }
 //  Mat resultMat=Mat::zeros(eroDst.size(),CV_8UC3);
//    for(int i=0;i<minRect.size();i++)
//    {
//        Scalar color=CV_RGB(rand()&255,rand()&255,rand()&255);
//        drawContours(frame0,contour,i,color,1,8,hierarchy);
//        Point2f rect_points[4];
//        minRect[i].points(rect_points);//计算二维矩形框的顶点
//        for(int j=0;j<4;j++)
//        {
//            cout<<rect_points[j]<<endl;
//           // line(resultMat,rect_points[j],rect_points[(j+1)%4],color,1,8);
//            line(frame0,rect_points[j],rect_points[(j+1)%4],color,1,8);
//        }
//    }
   // imshow("resultMat1",resultMat);
    cv::namedWindow("resultMat2",(400,600));
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
    cv::namedWindow("dil_eroDst",(400,600));
    imshow("dil_eroDst",eroDst);
    cacBoundRectRandomDirection(eroDst,frame0);
    return 0;
}
