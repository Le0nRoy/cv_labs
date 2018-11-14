/*
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
*/
#include "lab2.h"

/// Remake somehow
void lab2() {
    cv::Mat img = cv::imread("/home/bledgharm/CV_labs/labs/2/lena.jpeg", CV_LOAD_IMAGE_COLOR);
    //cv::Mat img = cv::imread("/home/bledgharm/CV_labs/labs/2/images1.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat lib_blur = img.clone();
    cv::Mat cust_blur = img.clone();

    cv::Mat compare = img.clone();

    cv::Size filter_size(3, 3);

    cv::TickMeter msec_timer;

    msec_timer.reset();
    msec_timer.start();
    blur(lib_blur, lib_blur, filter_size);
    msec_timer.stop();
    imshow("OpenCV_Filter", lib_blur);
    std::cout << "OpenCV time = " << msec_timer.getTimeMilli() << " msec" << std::endl;

    msec_timer.reset();
    msec_timer.start();
    custom_blur(cust_blur, cust_blur);
    msec_timer.stop();
    imshow("Custom_Filter", cust_blur);
    std::cout << "Custom time = " << msec_timer.getTimeMilli() << " msec" << std::endl;

    float percentage = 0;
    percentage = compare_blurs(lib_blur, cust_blur);
    std::cout << "Similarity = " << percentage*100 << "%" << std::endl;

    imshow("Original", img);

    cv::waitKey(0);
    //return 0;
}

/// May use locateROY()
/// use Mat.ptr to get start of row
void custom_blur(cv::Mat orig, cv::Mat res){
    int width = orig.cols;
    //int height = orig.rows;

    cv::MatIterator_<cv::Vec3b> beg, it, end, res_it, p;
    int blue = 0, green = 0, red = 0;

    for( beg = orig.begin<cv::Vec3b>(), it = beg, end = orig.end<cv::Vec3b>(),
         res_it = res.begin<cv::Vec3b>(); it != end; ++it, ++res_it){
        p = it;
        blue = 0, green = 0, red = 0;
        for (int i = 0; i <= 8; i++){
            p = it-(1-i/3)*width-(1-i%3);
            blue += (*p)[0];
            green += (*p)[1];
            red += (*p)[2];
        }

        blue /= 9;
        green /= 9;
        red /= 9;
        cv::Vec3b newElem = cv::Vec3b(blue, green, red);
        *res_it = newElem;
    }
}

float compare_blurs(cv::Mat compare1, cv::Mat compare2){
    int width = compare1.cols;
    int height = compare1.rows;
    int blue1 = 0, green1 = 0, red1 = 0;
    int blue2 = 0, green2 = 0, red2 = 0;
    int simil;

    for (int i=0; i<width; i++) {
        for (int j = 0; j < height; j++){
            // Gaining intensity of first image
            cv::Vec3b elem = compare1.at<cv::Vec3b>(i, j);
            blue1  += elem.val[0]/10;
            green1 += elem.val[1]/10;
            red1   += elem.val[2]/10;

            // Getting ratio of intensity with second image
            elem = compare2.at<cv::Vec3b>(i, j);
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