
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;



int main()
{
    double pi = 3.14;

    Mat img = imread("/home/bledgharm/CV_labs/lab1/Image0.png", CV_LOAD_IMAGE_COLOR);//????????? ???
    Mat cat = imread("/home/bledgharm/CV_labs/lab1/Image1.png", CV_LOAD_IMAGE_COLOR);//????????? ?????????? ????????

    int x=3;
    for (int x = 0; x < (img.cols-cat.cols); x++)
    {
        Mat res = img.clone();
        int y = 50 + 50 * (sin(x*pi / 180) + 1);
        Rect rect(x, y, cat.cols, cat.rows);
        Mat ROI(res(rect)); // This is what not works
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
