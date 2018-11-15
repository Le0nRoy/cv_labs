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
    Mat img = imread("/home/bledgharm/CV_labs/labs/2/lena.jpeg", CV_LOAD_IMAGE_COLOR);
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

    imshow("Original", img);

    waitKey(0);
    //return 0;
}

/// May use locateROY()
/// use Mat.ptr to get start of row
void custom_blur(const Mat orig, Mat res, Size filter_size)
{
    int width = orig.cols;
    //int height = orig.rows;

//    Size filter_size(3, 3);
    // Need to expand original image and make non-filtering frame
    // After processing delete this frame
    Point startROI(0, 0);
    Rect ROI = Rect(startROI, filter_size);
    Mat image_ROI = res(ROI);

    MatIterator_<Vec3b> beg, it, end, res_it, p;
    int blue = 0, green = 0, red = 0;

    for( beg = orig.begin<Vec3b>(), it = beg, end = orig.end<Vec3b>(),
         res_it = res.begin<Vec3b>(); it != end; ++it, ++res_it)
    {
        p = it;
        blue = 0, green = 0, red = 0;
        for (int i = 0; i <= 8; i++)
        {
            p = it-(1-i/3)*width-(1-i%3);
            blue += (*p)[0];
            green += (*p)[1];
            red += (*p)[2];
        }

        blue /= 9;
        green /= 9;
        red /= 9;
        Vec3b newElem = Vec3b(blue, green, red);
        *res_it = newElem;
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