#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include <stdint.h>
#include <math.h>
#include <opencv/cv.h>
#include <iostream>
#include <complex>
#include <cmath>

using namespace cv;
using namespace std;


double x_calculations(double i)
{
    double x = 205457 * i*i*i / 2350879200 - 123717 * i*i / 23508792 + 128769947 * i / 261208800 + 162 / 5;
    x /= 20;
    return x;
}

double y_calculations(double j, double x)
{
    double alpha;
    if (j < 80)
    {
        j = 80 - j;
        alpha = (j / 360.0)*(37.0*CV_PI / 180.0);
    }
    else
    {
        j = j - 80;
        alpha = -(j / 360.0)*(37.0*CV_PI / 180.0);
    }

    double y = x*tan(alpha);

    y -= 30;
    y *= 1;

    return y;
}


int lab5 ( )
{
    int graphicCoefficient = 2;

    Mat scanImg(500* graphicCoefficient, 500 * graphicCoefficient, CV_8UC3, Scalar(0, 0, 0));

    int dist = 10 * graphicCoefficient;

    int width = scanImg.size().width;

    int height = scanImg.size().height;

    for (int i = 0; i < height; i += dist)
        line(scanImg, Point(0, i), Point(width, i), Scalar(255, 255, 255));

    for (int i = 0; i < width; i += dist)
        line(scanImg, Point(i, 0), Point(i, height), Scalar(255, 255, 255));

    Mat startScanImg = scanImg.clone();

    VideoCapture myVideo;

//    myVideo.open("../../../tasks/5/Video/0.avi");
//    myVideo.open("../../../tasks/5/Video/calib_1.avi");
    myVideo.open("../../../tasks/5/Video/1.avi");

    Mat inputImg;
//    Mat inputImg = imread("video/calib_1_0.jpg");

    Mat outputImg, hsvInputImg;

    while (1)
    {
        scanImg = startScanImg.clone();

        Mat frame;
        myVideo >> frame;
        inputImg = frame.clone();

        cvtColor(inputImg, hsvInputImg, CV_BGR2HSV);
//        namedWindow ( "out", 1 );
//            int h = 84;
//            int s = 72;
//            int v = 61;
//            int hMax = 156;
//            int sMax = 147;
//            int vMax = 150;
//            createTrackbar ( "h", "out", &h, 180 );
//            createTrackbar ( "s", "out", &s, 255 );
//            createTrackbar ( "v", "out", &v, 255 );
//            createTrackbar ( "hMax", "out", &hMax, 180 );
//            createTrackbar ( "sMax", "out", &sMax, 255 );
//            createTrackbar ( "vMax", "out", &vMax, 255 );
//            while ( waitKey ( 100 ) != 27 )
//            {
//                inRange ( hsvInputImg, Scalar ( h, s, v ), Scalar ( hMax, sMax, vMax ), outputImg );
//                imshow ( "hsvImg", outputImg );
//            }
        inRange(hsvInputImg, Vec3b(17, 0, 65), Vec3b(118, 189, 255), outputImg);

        for (int i = 0; i < outputImg.rows; i++)
        {
            for (int j = 0; j < outputImg.cols; j++)
            {
                if (outputImg.at<uchar>(i, j) == 255)
                {
                    double xPoint = x_calculations(i);
                    double yPoint = y_calculations(j, xPoint);

                    circle(scanImg, Point(scanImg.rows / 2.0 - yPoint * graphicCoefficient, (xPoint)*graphicCoefficient), 2, Scalar(255, 0, 0), -1);
                }
            }
        }


        imshow("mat", scanImg);
        imshow("out", outputImg);
        imshow("img", inputImg);

        if (waitKey(1) >= 0) break;
    }
    waitKey();

    return 0;
}
