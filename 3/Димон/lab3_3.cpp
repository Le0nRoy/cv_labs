#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;


double d, minum;
Moments mnts, imnts, lmnts;
Mat img, input_img, cimg, buff, kernel, result, lamp, red, green, blue;
vector<vector<Point>> cnts;

void result_img(int mini, int maxi, Vec3b color, int n)
{
	//??????? ?????????? ??????????, ?????????? ????? ????? ?????? ? ????????? ???????

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
				//?? ??????? ?????????? ????? ????? ??????? ? ?????????? ????????????
				d = sqrt((lmnts.m10 / lmnts.m00 - mnts.m10 / mnts.m00) * (lmnts.m10 / lmnts.m00 - mnts.m10 / mnts.m00) + (lmnts.m01 / lmnts.m00 - mnts.m01 / mnts.m00) * (lmnts.m01 / lmnts.m00 - mnts.m01 / mnts.m00));
				if (minum > d)
				{
					minum = d;
					imnts = mnts;
				}
			}
			else if (n == 1) { lmnts = mnts; imnts = mnts; }


			polylines(result, cnts[i], true, color, 2, 8);
		}
	}

	circle(result, Point(imnts.m10 / imnts.m00, imnts.m01 / imnts.m00), 5, Vec3b(0, 0, 0), -1);
	line(result, Point(imnts.m10 / imnts.m00, imnts.m01 / imnts.m00), Point(lmnts.m10 / lmnts.m00, lmnts.m01 / lmnts.m00), Vec3b(0, 0, 0), 2, 8, 0);
}

int main()
{

	input_img = imread("1.jpg");

	resize(input_img, img, cv::Size(), 0.5, 0.5);

	resize(input_img, input_img, cv::Size(), 0.75, 0.75);

	cvtColor(input_img, cimg, CV_BGR2HSV);//H-??????? S-???????????? V-???????
	buff = cimg.clone();
	result = input_img.clone();

	imshow("img", img);


	//lamp
	inRange(cimg, Vec3b(0, 0, 150), Vec3b(35, 12, 255), cimg);//??????? ?????
	result_img(65, 110, Vec3b(0, 255, 255), 1);
	
	//red
	cimg = buff.clone();
	kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));//???????????? ???? ??? ??????
	erode(cimg, cimg, kernel, Point(-1, -1), 5);//??????
	inRange(cimg, Vec3b(0, 10, 0), Vec3b(3, 255, 255), cimg);
	result_img(80, 150, Vec3b(0, 0, 255), 0);
	
	//green
	cimg = buff.clone();
	inRange(cimg, Vec3b(65, 50, 140), Vec3b(80, 255, 255), cimg);
	result_img(17, 120, Vec3b(0, 255, 0), 0);
	green = result;

	//blue
	cimg = buff.clone();
	inRange(cimg, Vec3b(92, 50, 128), Vec3b(102, 255, 255), cimg);
	result_img(0, 100, Vec3b(255, 0, 0), 0);
	blue = result;


	imshow("result", result);
	waitKey(0);
}