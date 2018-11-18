#include "lab2.h"
//#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/// Remake somehow
void lab2()
{
    Mat img = imread("../4/ig_0.jpg", CV_LOAD_IMAGE_COLOR);
    Mat lib_blur = img.clone();
    Mat cust_blur = img.clone();
    Mat compare = img.clone();

    Size filter_size(3, 3);

    TickMeter msec_timer;

    msec_timer.reset();
    msec_timer.start();
    blur(lib_blur, lib_blur, filter_size);
    msec_timer.stop();
    imshow("OpenCV_Filter", lib_blur);
    cout << "OpenCV time = " << msec_timer.getTimeMilli() << " msec" << endl;

    msec_timer.reset();
    msec_timer.start();
    custom_blur(cust_blur, cust_blur, filter_size);
    msec_timer.stop();
    imshow("Custom_Filter", cust_blur);
    cout << "Custom time = " << msec_timer.getTimeMilli() << " msec" << endl;

    float percentage = 0;
    percentage = compare_blurs(lib_blur, cust_blur);
    cout << "Similarity = " << percentage*100 << "%" << endl;

    Mat diff = lib_blur - cust_blur;
    imshow("dif", diff);
//    imshow("Original", img);

    waitKey(0);
}

/// May use locateROY()
/// use Mat.ptr to get start of row
void custom_blur(const Mat orig, Mat res, Size filter_size)
{
    // Need to expand original image and make non-filtering frame
    // After processing delete this frame
    int width = res.cols - 2;
    int imgSize = (res.cols - 2) * (res.rows - 2);

    Point startROI(0, 0);
    Rect ROI = Rect(startROI, filter_size);
    int sizeROI = filter_size.height * filter_size.width;

    for (int i = 0; i < imgSize; i++)
    {
        ROI.x = i % width;
        ROI.y = i / width;
        Mat image_ROI = res(ROI);

        int blue = 0;
        int green = 0;
        int red = 0;

        int xInROI = 0;
        int yInROI = 0;
        for (int j = 0; j < sizeROI; j++)
        {
            xInROI = j % filter_size.width;
            yInROI = j / filter_size.height;
            blue += image_ROI.at<Vec3b>(yInROI, xInROI)[0];
            green += image_ROI.at<Vec3b>(yInROI, xInROI)[1];
            red += image_ROI.at<Vec3b>(yInROI, xInROI)[2];
        }
        blue /= 9;
        green /= 9;
        red /= 9;
        image_ROI.at<Vec3b>(1, 1)[0] = blue;
        image_ROI.at<Vec3b>(1, 1)[1] = green;
        image_ROI.at<Vec3b>(1, 1)[2] = red;
    }
}


float compare_blurs(Mat compare1, Mat compare2)
{
    int width = compare1.cols;
    int height = compare1.rows;
    float blue1 = 0, green1 = 0, red1 = 0;
    float blue2 = 0, green2 = 0, red2 = 0;
    float simil;

    for (int i=0; i<width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Gaining intensity of first image
            blue1  += compare1.at<Vec3b>(i, j)[0]/10.0;
            green1 += compare1.at<Vec3b>(i, j)[1]/10.0;
            red1   += compare1.at<Vec3b>(i, j)[2]/10.0;

            // Getting ratio of intensity with second image
            blue2  += compare2.at<Vec3b>(i, j)[0]/10.0;
            green2 += compare2.at<Vec3b>(i, j)[1]/10.0;
            red2   += compare2.at<Vec3b>(i, j)[2]/10.0;
        }
    }
    simil = (blue1+green1+red1)/((blue2+green2+red2));
    simil = (simil < 1) ? simil : (2 - simil);
    return simil;
}
