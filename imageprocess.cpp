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
//用于计算两点之间的距离
float imageProcess::calc_distance(Point2f verticle1,Point2f verticle2)
{
    float distance=sqrt(pow(verticle1.x-verticle2.x,2)+pow(verticle1.y-verticle2.y,2));
    return distance;
}
//通过三个点来计算矩形面积
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
        if (area_rect[i]<300) continue;//去除噪声
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
        }
        if(is_in_unique==false)
        {
            unique_rect.push_back(cur_rect);
        }
    }
    return unique_rect;
}

vector<vector<Point2f>> imageProcess::cacBoundRectRandomDirection(Mat eroDst, Mat frame0,Mat &res_frame)
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
    res_frame=frame0;


//    cv::namedWindow("resultMat2",(400,600));
//    imshow("resultMat2",frame0);//在原图显示矩形框
//    waitKey(0);


    return unique_rect;
//   Mat resultMat=Mat::zeros(eroDst.size(),CV_8UC3);
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

vector<vector<float>>  imageProcess::getDistanceFromImage(Mat frame0,Mat &res_frame0)
{
    vector<vector<float>> dist2agents;
    if(!frame0.data)
    {
        cout<<"unable to open"<<endl;
        return dist2agents;
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
    Mat gray;
    cvtColor(rawImg,gray,COLOR_BGR2GRAY);
    threshold(gray,binary,70,255,THRESH_BINARY);

//    imshow("binary",binary);
//    Mat bgr[3];
//    split(rawImg, bgr); //将三个通道的像素值分离
//    bImage = bgr[0];
//    gImage = bgr[1];
//    rImage = bgr[2];
//    getBinImage(gImage, rImage, binary, 40);    //如果像素G值-R值大于40，则返回的二值图像的值为255，否则为0

    dilate(binary, dilDst, Mat(), Point(-1,-1), 2);   //图像膨胀
    erode(dilDst, eroDst, Mat(), Point(-1,-1), 1);  //图像腐蚀，先膨胀在腐蚀属于闭运算
//    cv::namedWindow("dil_eroDst",(400,600));
//    imshow("dil_eroDst",eroDst);
    vector<vector<Point2f>> v_bound_rects=cacBoundRectRandomDirection(eroDst,frame0,res_frame0);
    dist2agents=solveBeaconDistance(v_bound_rects, 40);
    return dist2agents;
}

vector<vector<float>> imageProcess::solveBeaconDistance(vector<vector<Point2f> > unique_rect, float edge_length)
{
    vector<vector<float>> res;
    cout<<"testing..."<<endl;
    for(int i=0;i<unique_rect.size();i++)
    {

        vector<Point2f> cur_rect=unique_rect[i];
        Point2f verticle_1=cur_rect[0];
        map<float,int> map_distance_point;
        vector<float> distance_verticle1_others;
        //將距离压入向量中用于排序，并获取距离和点的映射关系
        distance_verticle1_others.push_back(calc_distance(verticle_1,cur_rect[1]));
        pair<int,float> cur_dist_2_index(1,calc_distance(verticle_1,cur_rect[1]));
        map_distance_point.insert(cur_dist_2_index);

        distance_verticle1_others.push_back(calc_distance(verticle_1,cur_rect[2]));
        cur_dist_2_index=make_pair(2,calc_distance(verticle_1,cur_rect[2]));
        map_distance_point.insert(cur_dist_2_index);

        distance_verticle1_others.push_back(calc_distance(verticle_1,cur_rect[3]));
        cur_dist_2_index=make_pair(3,calc_distance(verticle_1,cur_rect[3]));
        map_distance_point.insert(cur_dist_2_index);

        //排序，取得两条短边
        sort(distance_verticle1_others.begin(),distance_verticle1_others.end());
        //获取真实距离和像素的比率，用于后续3D左边点的获取
        float long_edge_pix=distance_verticle1_others[1];
        float ratio_len2pixel=edge_length/long_edge_pix;

        //获取到的中点坐标,也就是世界坐标系的坐标原点
        Point2f middle((cur_rect[0].x+cur_rect[2].x)/2.0,(cur_rect[0].y+cur_rect[2].y)/2.0);
        cout<<"middle.x:"<<middle.x<<"middle.y:"<<middle.y<<endl;
        //真实场景下的3D坐标点
        vector<Point3f> objP;

        for(int i=0;i<4;i++)
        {
            float z;
            if(map_distance_point[i]>long_edge_pix)
            {
                z=1*sqrt(abs(2*pow(long_edge_pix,2)-pow(map_distance_point[i],2)))*ratio_len2pixel;
            }
            else{
                z=1*sqrt(abs(pow(long_edge_pix,2)-pow(map_distance_point[i],2)))*ratio_len2pixel;
            }
            Point3f point((cur_rect[i].x-middle.x)*ratio_len2pixel,ratio_len2pixel*(cur_rect[i].y-middle.y),0);
            objP.push_back(point);
            cout<<"Point  x:"<<point.x<<"y:"<<point.y<<endl;
        }

        //相机参数
        vector<Point2f> imgP=cur_rect;
        //陆地上相机参数
//        double camD[9]={189.62 , 0,739.0365,0,271.3572,737.8754,0,0,1};
//        double distCoeffD[5]={0.0454,-0.394,0.4363,-0.0053,0.00078};

        //100万像素陆相机参数
//        double camD[9]={853.2785 , 0,307.2291,0,852.4253,227.778,0,0,1};
//        double distCoeffD[5]={0.1792,-0.2934,-0.0012,0.0026,-2.4351};

        //水里200万像素上相机参数
        double camD[9]={553.5941 , 0,178.661,0,569.6427,158.612,0,0,1};
        double distCoeffD[5]={0.5842,-0.8683,0.00056,-0.0074,1.086};

        //水里标定的参数
        //double camD[9]={1003.75, 0,301.4028,0,1000.65,227.5788,0,0,1};
        //double distCoeffD[5]={0.4368,-0.2404,-0.0161,0.0175,-10.2254};

        Mat camera_matrix=Mat(3,3,CV_64FC1,camD);
        Mat distortion_coefficients=Mat(5,1,CV_64FC1,distCoeffD);

        Mat r_matrix;
        Mat t_matrix;
        double buff[2];
        vector<float> output_series;
        cout<<"camera_matrix"<<endl;
        cout<<"distortion:"<<endl;
        cout<<"camera_matrix:"<<camera_matrix<<endl<<distortion_coefficients<<endl<<"Mat(imageP):"<<Mat(imgP)<<"Mat(pos)"<<endl;
        cout<<"Mat(objP):"<<Mat(objP);
        solvePnP(Mat(objP),Mat(imgP),camera_matrix,distortion_coefficients,r_matrix,t_matrix);
        cout<<"r_matrix"<<r_matrix<<endl;
        cout<<"t_matrix:"<<t_matrix<<endl;

        //旋转向量变旋转矩阵
        //提取旋转矩阵
        double rm[9];
        cv::Mat rotM(3, 3, CV_64FC1, rm);
        cv::Rodrigues(r_matrix, rotM);
        cout<<"rotM:"<<rotM<<endl;
        double r11 = rotM.ptr<double>(0)[0];
        double r12 = rotM.ptr<double>(0)[1];
        double r13 = rotM.ptr<double>(0)[2];
        double r21 = rotM.ptr<double>(1)[0];
        double r22 = rotM.ptr<double>(1)[1];
        double r23 = rotM.ptr<double>(1)[2];
        double r31 = rotM.ptr<double>(2)[0];
        double r32 = rotM.ptr<double>(2)[1];
        double r33 = rotM.ptr<double>(2)[2];

        /*************************************此处计算出相机的旋转角**********************************************/
        //计算出相机坐标系的三轴旋转欧拉角，旋转后可以转出世界坐标系。
        //旋转顺序为z、y、x
        //原理见帖子：
        double thetaz = atan2(r21, r11) / CV_PI * 180;
        double thetay = atan2(-1 * r31, sqrt(r32*r32 + r33*r33)) / CV_PI * 180;
        double thetax = atan2(r32, r33) / CV_PI * 180;

        //fout << -1 * thetax << endl << -1 * thetay << endl << -1 * thetaz << endl;
        cout << "相机的三轴旋转角：" << -1 * thetax << ", " << -1 * thetay << ", " << -1 * thetaz << endl;
        //fout.close();
        /*************************************此处计算出相机的旋转角END**********************************************/

        for(int i=0;i<3;i++)
        {
            buff[1]=t_matrix.at<double>(0,i);
            t_matrix.at<double>(0,i)=buff[1];
            cout<<"t_i"<<i<<":"<<buff[1]<<endl;
            output_series.push_back(t_matrix.at<double>(0,i));
        }
        cout<<"output_series:"<<output_series[0]<<" "<<output_series[1]<<output_series[2];

        res.push_back(output_series);
    }
    return res;
}
