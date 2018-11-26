#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;


int th = 0;
Moments mnts;
Mat img, thimg, cimg, kernel;

int main(int argc, char** argv)
{
	string fn;
	/// Load image
	if (argc>1)
    {
        fn = argv[1];
    }
	else
    {
        fn = "ig_0.jpg";
    }
	img = imread(fn, 0);
	/// Show image
	imshow(fn, img);
    /// Set to binary mode
	threshold(img, thimg, 230, 255, THRESH_BINARY);
    /// kernel for morphology
	kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));

//	erode(thimg, cimg, kernel, Point(-1, -1), 2);
//	imshow("erode", cimg);
    /// Make object bigger
	dilate(thimg, cimg, kernel, Point(-1, -1), 4);
//	imshow("dilate", cimg);
    /// Find contours of object
	vector<vector<Point>> cnts;
	findContours(cimg, cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

//	imshow("contours", cimg);

	cvtColor(img, img, CV_GRAY2BGR);

	polylines(img, cnts, true, Vec3b(0, 255, 0), 2, 8);
//	imshow("polyline", img);

	for (int i = 0; i < cnts.size(); i++)
	{
		if (cnts[i].size() > 10)
		{
			mnts = moments(cnts[i]);
			circle(img, Point(mnts.m10 / mnts.m00, mnts.m01 / mnts.m00), 5, Vec3b(0, 0, 255), -1);
		}
	}
	imshow("result", img);

	waitKey(0);
}