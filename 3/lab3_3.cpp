#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

double d, minum;
Moments mnts, imnts, lmnts;
Mat img, input_img, cimg, buff, kernel, result, lamp, red, green, blue, a, b;
vector<vector<Point>> cnts;

int x=0, y=0, k=0, p=0;

void together()
{
	for (y = 0; y < 540; y++)
	{
		for (x = 450; x <= 480; x++)
		{
			if (cimg.at<uchar>(y, x) == 255)
			{
			    k = x; break;
			}
		}

		for (x = 480; x >= 450; x--)
		{
			if (cimg.at<uchar>(y, x) == 255)
			{
			    p = x; break;
			}
		}

		if (k!=0)
        {
            for (x = k; x < p; x++)
            {
                cimg.at<uchar>(y, x) = 255;
            }
        }
		k = 0;
	}
	//imshow("3", cimg);
}

void result_img(int mini, int maxi, Vec3b color, int n)
{
	findContours(cimg, cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	d = 0;
	minum = 1000;
	for (int i = 0; i < cnts.size(); i++)
	{
		if (cnts[i].size() > mini && cnts[i].size() < maxi)
		{
			mnts = moments(cnts[i]);
			if (n == 0)
			{
				d = sqrt((lmnts.m10 / lmnts.m00 - mnts.m10 / mnts.m00) * (lmnts.m10 / lmnts.m00 - mnts.m10 / mnts.m00) +
						(lmnts.m01 / lmnts.m00 - mnts.m01 / mnts.m00) * (lmnts.m01 / lmnts.m00 - mnts.m01 / mnts.m00));
				if (minum > d)
				{
					minum = d;
					imnts = mnts;
				}
			}
			else if (n == 1)
			{
			    lmnts = mnts; imnts = mnts;
			}

			polylines(result, cnts[i], true, color, 2, 8);
		}
	}

	circle(result, Point(imnts.m10 / imnts.m00, imnts.m01 / imnts.m00), 5, Vec3b(0, 0, 0), -1);
	line(result, Point(imnts.m10 / imnts.m00, imnts.m01 / imnts.m00), Point(lmnts.m10 / lmnts.m00, lmnts.m01 / lmnts.m00), Vec3b(0, 0, 0), 2, 8);
}

int lab3_3()
{
	input_img = imread("../3/Tasks/img_zadan/roboti/roi_robotov.jpg");
	resize(input_img, img, cv::Size(), 0.5, 0.5);
	resize(input_img, input_img, cv::Size(), 0.75, 0.75);

	cvtColor(input_img, cimg, CV_BGR2HSV);
	/// resized and HSV
	buff = cimg.clone();
	result = input_img.clone();
	imshow("img", img);
	/// lamp
	inRange(cimg, Vec3b(0, 0, 150), Vec3b(35, 12, 255), cimg);
    kernel = getStructuringElement(CV_SHAPE_RECT, Size(5, 5));
    erode(cimg, cimg, kernel, Point(-1, -1), 1);
    dilate(cimg, cimg, kernel, Point(-1, -1), 3);
	result_img(65, 110, Vec3b(0, 255, 255), 1);
	/// red
	cimg = buff.clone();
	erode(cimg, cimg, kernel, Point(-1, -1), 5);
	inRange(buff, Vec3b(0, 10, 0), Vec3b(10, 255, 255), a);
	inRange(buff, Vec3b(160, 10, 0), Vec3b(179, 255, 255), b);
	cimg = a + b;
	together();
	result_img(40, 120, Vec3b(0, 0, 255), 0);
	/// green
	cimg = buff.clone();
	inRange(cimg, Vec3b(65, 50, 140), Vec3b(80, 255, 255), cimg);
	together();
	result_img(17, 120, Vec3b(0, 255, 0), 0);
	/// blue
	cimg = buff.clone();
	inRange(cimg, Vec3b(92, 50, 128), Vec3b(102, 255, 255), cimg);
	together();
	result_img(0, 100, Vec3b(255, 0, 0), 0);

	imshow("result", result);
	waitKey(0);
}