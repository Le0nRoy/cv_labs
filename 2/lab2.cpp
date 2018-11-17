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
    //cv::Mat img = cv::imread("/home/bledgharm/CV_labs/labs/2/images1.jpg", CV_LOAD_IMAGE_COLOR);
    Mat lib_blur = img.clone();
    Mat cust_blur = img.clone();

    Mat compare = img.clone();

    Size filter_size(3, 3);
    // Start processing from first pixel (0,0)
//    Point startROI(-1, -1);
//    Rect ROI = Rect(startROI, filter_size);

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
    //return 0;
}

/// May use locateROY()
/// use Mat.ptr to get start of row
void custom_blur(const Mat orig, Mat res, Size filter_size)
{
    int width = res.cols - 2;
    int imgSize = (res.cols - 2) * (res.rows - 2);

    // Need to expand original image and make non-filtering frame
    // After processing delete this frame
    Point startROI(0, 0);
    Rect ROI = Rect(startROI, filter_size);
    int sizeROI = filter_size.height * filter_size.width;

    MatIterator_<Vec3b> beg, it, end, res_it, p;

    for (int i = 1; i < imgSize; i++)
    {
        ROI.x = i % width;
        ROI.y = i / width;
        Mat image_ROI = res(ROI);
        // Change pixels inside img_ROI
        int x = 0;
        int y = 0;

        int blue  = 0;
        int green = 0;
        int red   = 0;

        for (int j = 0; j < sizeROI; j++)
        {
            x = j % filter_size.width;
            y = j / filter_size.height;
            blue  += image_ROI.at<Vec3b>(x,y)[0];
            green += image_ROI.at<Vec3b>(x,y)[1];
            red   += image_ROI.at<Vec3b>(x,y)[2];
        }
        blue /= 9;
        green /= 9;
        red /= 9;
        x = ROI.x + 1;
        y = ROI.y + 1;
        image_ROI.at<Vec3b>(x,y)[0] = blue;
//        cout << "new blue is: " << static_cast<int>(image_ROI.at<Vec3b>(x,y)[0] ) << endl;
        image_ROI.at<Vec3b>(x,y)[1] = green;
        image_ROI.at<Vec3b>(x,y)[2] = red;
    }
}

float compare_blurs(Mat compare1, Mat compare2)
{
    int width = compare1.cols;
    int height = compare1.rows;
    int blue1 = 0, green1 = 0, red1 = 0;
    int blue2 = 0, green2 = 0, red2 = 0;
    int simil;

    for (int i=0; i<width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Gaining intensity of first image
            Vec3b elem = compare1.at<Vec3b>(i, j);
            blue1  += elem.val[0]/10;
            green1 += elem.val[1]/10;
            red1   += elem.val[2]/10;

            // Getting ratio of intensity with second image
            elem = compare2.at<Vec3b>(i, j);
            blue2  += elem.val[0]/10;
            green2 += elem.val[1]/10;
            red2   += elem.val[2]/10;
        }
    }
    simil = (blue1+green1+red1)/((blue2+green2+red2));
    return simil;
}

//void my3x3FilterBox(Mat orig, Mat res, int x0, int y0)
//{
//	Rect rp(y0-1, x0-1, 3, 3);
//	Mat roi = orig(rp);
//	int blue = 0, green = 0, red = 0;
//
//	for (int i=0; i<3; i++)
//		for (int j = 0; j < 3; j++)
//		{
//			Vec3b elem = roi.at<Vec3b>(i, j);
//			blue += elem.val[0];
//			green += elem.val[1];
//			red += elem.val[2];
//		}
//
//	blue /= 9;
//	green /= 9;
//	red /= 9;
//	Vec3b newElem = Vec3b(blue, green, red);
//	res.at<Vec3b>(x0, y0) = newElem;
//}