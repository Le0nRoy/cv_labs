#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;


int th = 0;
Moments mnts;
Mat imgBGR, imgHSV, cimg, a, b, kernel;

int main()
{
	string fn = "5.jpg";
	imgBGR = imread(fn);
	imshow(fn, imgBGR);
	cvtColor(imgBGR, imgHSV, CV_BGR2HSV);
//	imshow(fn, imgHSV);
	inRange(imgHSV, Vec3b(0, 50, 50), Vec3b(17, 255, 255), a);
//	imshow("a", a);
	inRange(imgHSV, Vec3b(155, 50, 50), Vec3b(179, 255, 255), b);
//	imshow("b", b);
	cimg = a + b;
//	imshow("a + b", cimg);

	kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
//	erode(cimg, cimg, kernel, Point(-1, -1), 1);
	dilate(cimg, cimg, kernel, Point(-1, -1), 3);
//	imshow("cimg", cimg);

	vector<vector<Point>> cnts;
	findContours(cimg, cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	imshow("contours", cimg);

	for (int i = 0; i < cnts.size(); i++)
	{
		if (cnts[i].size() > 30)
		{
			mnts = moments(cnts[i]);
			polylines(imgBGR, cnts[i], true, Vec3b(255, 255, 255), 2, 8);
			circle(imgBGR, Point(mnts.m10 / mnts.m00, mnts.m01 / mnts.m00), 5, Vec3b(0, 0, 0), -1);
		}
	}
	imshow("result", imgBGR);

	waitKey(0);
}