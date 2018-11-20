#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;


Mat img, input_img, cimg, timg;
vector<vector<Point>> cnts, tnts;


int main()
{

	input_img = imread("gk.jpg", 0);
	img = imread("gk_tmplt.jpg", 0);


	resize(input_img, input_img, cv::Size(), 0.75, 0.75);

	//обработка gk
	threshold(input_img, cimg, 240, 255, THRESH_BINARY_INV);

	findContours(cimg, cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cvtColor(input_img, input_img, CV_GRAY2BGR);//перевод в RGB, чтобы контура отображались цветными


	//обработка gk_tmplt
	threshold(img, timg, 240, 255, THRESH_BINARY);

	findContours(timg, tnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cvtColor(img, img, CV_GRAY2BGR);//перевод в RGB, чтобы контура отображались цветными
	polylines(img, tnts[0], true, Vec3b(0, 255, 0), 2, 8);
	


	for (int i = 0; i < cnts.size(); i++)
	{
		float diff = matchShapes(cnts[i], tnts[0], CV_CONTOURS_MATCH_I2, 0);//разность моментов

		if (abs(diff) < 0.5)
		{
			polylines(input_img, cnts[i], true, Vec3b(0, 255, 0), 2, 8);
		}
		else polylines(input_img, cnts[i], true, Vec3b(0, 0, 255), 2, 8);
	}
	imshow("input_img", input_img);
	imshow("template", img);

	waitKey(0);
}