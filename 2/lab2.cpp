#include "lab2.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/// Remake somehow
void lab2()
{
    Mat img = imread("../2/flowers.jpg", CV_LOAD_IMAGE_COLOR);
    resize(img, img, Size(0, 0), 0.5, 0.5);
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
    lab_box_filter(cust_blur, cust_blur, filter_size);
    msec_timer.stop();
    imshow("Custom_Filter", cust_blur);
    cout << "Custom time = " << msec_timer.getTimeMilli() << " msec" << endl;

    Mat diff = lib_blur - cust_blur;
    imshow("dif", diff);
    imshow("Original", img);

    waitKey(0);
}

/// May use locateROY()
/// use Mat.ptr to get start of row
void lab_box_filter(const Mat orig, Mat res, Size filter_size)
{
    // Need to add kernel array
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
        int x = filter_size.width / 2;
        int y = filter_size.height / 2;
        image_ROI.at<Vec3b>(y, x)[0] = (uint8_t)blue;
        image_ROI.at<Vec3b>(y, x)[1] = (uint8_t)green;
        image_ROI.at<Vec3b>(y, x)[2] = (uint8_t)red;
    }
}
