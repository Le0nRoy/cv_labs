#include "lab1.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;



void lab1()
{
    double pi = 3.14;

    Mat img = imread("../4/ig_0.jpg", CV_LOAD_IMAGE_COLOR);
    // Need another image (little one)
    Mat cat = imread("../4/ig_0.jpg", CV_LOAD_IMAGE_COLOR);

    for (int x = 0; x < (img.cols-cat.cols); x++)
    {
        Mat res = img.clone();
        int y = 50 + 50 * (sin(x*pi / 180) + 1);
        Rect rect(x, y, cat.cols, cat.rows);
        Mat ROI(res(rect));
        imshow("test_roi", ROI);
        cat.copyTo(ROI, cat > 5);
        imshow("Cat", cat);
        circle(img, { x, (y + cat.cols/2) }, 4, { 255 }, -1);
        imshow("cat", res);
        waitKey(15);
        if (x == (img.rows / 2))
            imwrite("Cat.jpg", res);
   }

    waitKey(0);
}
