#include<opencv2\opencv.hpp>
using namespace cv;
using namespace std;


double PixToX(double i)
{
	//double x = 339 * i*i*i / 20593300 - 5253 * i*i / 823732 - 6508741 * i / 5148325 + 43411827 / 79205;
	//double x = 0.002269 * i*i  + 0.101797 * i / 3393 - 13.054 ;
	//double x = 0.00239 * i*i + 0.401797 * i / 3393 - 150;
	//double x = 0.00639 * i*i + 0.401797 * i / 9393 - 150;
	//double x = 132379 * i*i*i / 783626400 - 14281107 * i*i / 391811320 + 193770409* i / 87069600 + 25;
	//double x = 225457 * i*i*i / 2350879200 - 463717 * i*i / 23508792 + 328769947 * i / 261208800 + 162/5;
	double x = 205457 * i*i*i / 2350879200 - 123717 * i*i / 23508792 + 128769947 * i / 261208800 + 162 / 5;
	x /= 20;
	return x;
}

double PixToY(double j, double x)
{
	double alpha;
	if (j < 80)
	{
		j = 80 - j;
		//alpha = (j / 360.0)*(30.0*2.7 / 180.0);
		alpha = (j / 360.0)*(37.0*CV_PI / 180.0);
		//alpha = (j / 360.0)*(60*CV_PI / 180.0);
	}
	else
	{
		j = j - 80;
		alpha = -(j / 360.0)*(37.0*CV_PI / 180.0);
		//alpha = -(j / 360.0)*(60*CV_PI / 180.0);
	}

	double y = x*tan(alpha);
	//double y = x *sin(alpha);
	y -= 30;
	y *= 1;
	
	return y;
}


int main()
{
	int graphicCoefficient = 1;	
	//Mat scanImg(500* graphicCoefficient, 600 * graphicCoefficient, CV_8UC3, Scalar(0, 0, 0));
	Mat scanImg(500* graphicCoefficient, 500 * graphicCoefficient, CV_8UC3, Scalar(0, 0, 0));
	//imshow("scanImg", scanImg);
	int dist = 10 * graphicCoefficient;
	int width = scanImg.size().width;
	int height = scanImg.size().height;
	for (int i = 0; i < height; i += dist)
	{
		line(scanImg, Point(0, i), Point(width, i), Scalar(255, 255, 255));
	/*	if (i== 540)
			line(scanImg, Point(0, i), Point(width, i), Scalar(0, 255, 0));
		if (i == 140)
			line(scanImg, Point(0, i), Point(width, i), Scalar(0, 255, 0));*/

	}
	for (int i = 0; i < width; i += dist)
	{
		line(scanImg, Point(i, 0), Point(i, height), Scalar(255, 255, 255));
	}
	Mat startScanImg = scanImg.clone();

	VideoCapture myVideo;
	myVideo.open("2.avi");
	Mat inputImg = imread("start.jpg");;
	Mat outputImg, hsvInputImg;

	while (1)
	{
		scanImg = startScanImg.clone();

		Mat frame;
		myVideo >> frame; 
		inputImg = frame.clone();		

		cvtColor(inputImg, hsvInputImg, CV_BGR2HSV);
		inRange(hsvInputImg, Vec3b(60, 0, 200), Vec3b(120, 100, 255), outputImg);
		imwrite("InRange.jpg", outputImg);
		//imshow("inRange", outputImg);
		//cout << outputImg.rows << "\n";
		//cout << outputImg.cols << "\n";
		for (int i = 0; i < outputImg.rows; i++)
		{
			for (int j = 0; j < outputImg.cols; j++)
			{
				if (outputImg.at<uchar>(i, j) == 255)
				{
					double xPoint = PixToX(i);
					double yPoint = PixToY(j, xPoint);
					//scanImg.rows / 2.0 -
					circle(scanImg, Point(scanImg.rows / 2.0 - yPoint * graphicCoefficient, (xPoint)*graphicCoefficient), 2, Scalar(255, 0, 0), -1);
					//cout << XXX(i) << "\n";
					//cout << YYY(j, XXX(i)) << "\n";
					//cout << i << "\n";
				}
			}
		}
		

		imshow("mat", scanImg);
		imshow("out", outputImg);
		imshow("img", inputImg);
		//waitKey();
		if (waitKey(1) >= 0) break;
	}
	waitKey();
}