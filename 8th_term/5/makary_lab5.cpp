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


int makary_lab5 ( )
{
	int graphicCoefficient = 1;	

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

	myVideo.open("video/2.avi");

	Mat inputImg = imread("video/calib_1_0.jpg");

	Mat outputImg, hsvInputImg;

	while (1)
	{
		scanImg = startScanImg.clone();

		Mat frame;
		myVideo >> frame; 
		inputImg = frame.clone();		

		cvtColor(inputImg, hsvInputImg, CV_BGR2HSV);

		inRange(hsvInputImg, Vec3b(60, 0, 200), Vec3b(120, 100, 255), outputImg);

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
}
