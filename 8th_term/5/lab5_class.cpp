#include "lab5_class.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define PI 3.14159265359

using namespace cv;
using namespace std;

static const std::string windowVideo = "capture";

/**
 * @note расстояние между камерой и лазером +-20см
 *      как-то через калибровочную плоскость можно находить расстояние
 *      структурированая подсветка и камера дают прямоугольный треугольник
 */

lab5_class::lab5_class() :
    width_map ( 600 ),
    height_map ( 600 ),
    d ( -0.25 ),
    videoIsLoaded ( false )
{

}

lab5_class::~lab5_class ( )
{
    video.release ( );
    destroyAllWindows ( );
}

void lab5_class::loadVideo ( std::string pathToVideo )
{
    video.open ( pathToVideo );

    if ( !video.isOpened ( ) )
    {
        videoIsLoaded = false;
        std::cout << "loadVideo () : failed to load video !" << std::endl;
        return;
    }

    videoIsLoaded = true;
}

void lab5_class::run_lab (  )
{
    if ( !videoIsLoaded )
    {
        std::cout << "run_lab ( ) : no video downloaded !" << std::endl;
    }
//    cv::Mat frame;
//    video.read ( frame );

//    int exitCondition = 1;
//    cv::namedWindow ( windowVideo, cv::WINDOW_AUTOSIZE );
//    cv::createTrackbar ( "turn_off", windowVideo, &exitCondition, 1 );

//    while ( !frame.empty ( ) && exitCondition )
//    {
//        cv::imshow ( windowVideo, frame );
//        video.read ( frame );
//        cv::waitKey ( 5 );
//    }
    Mat img;
    Mat transit_matrix = Mat::zeros(4, 4, CV_64F);
    double angle = 74 * PI / 180;

    video >> img;
    cx = img.cols / 2;
    cy = img.rows / 2;
    fx = tan(angle) * cx;
    fy = fx;
    transit_matrix.at<double>(0, 0) = fx;
    transit_matrix.at<double>(1, 1) = fy;
    transit_matrix.at<double>(0, 3) = cx;
    transit_matrix.at<double>(1, 3) = cy;
    transit_matrix.at<double>(3, 3) = 1;

    Mat kernel = Mat::ones(3, 3, CV_8U);
    Point2d dot;
    Point2i dot1 = Point2i(0, 0);
    Mat frame;
    Mat frame_copy;
    Mat map;
    for (;;)
    {
        video >> frame;
        if (frame.empty())
            break;
        frame_copy = Mat::zeros(frame.rows, frame.cols, CV_8U);
        imshow("Original video", frame);
        cvtColor(frame, frame_copy, COLOR_BGR2GRAY);
        threshold(frame_copy, frame_copy, 135, 255, THRESH_BINARY);
        dilate(frame_copy, frame_copy, kernel, Point(-1, -1), 2);
        erode(frame_copy, frame_copy, kernel, Point(-1, -1), 2);
        //imshow("Binary video", frame_copy);
        map = Mat::zeros(height_map, width_map, CV_8UC3);
        map = draw_map(map);
        for (int i = 0; i < frame.rows; i++)
        {
            for (int j = 0; j < frame.cols; j++)
            {
                if (frame_copy.at<uchar>(i, j) != 0)
                {
                    dot = map_pixel(j, i);
                    dot1.x = (int)(width_map - (dot.y) * 100) % width_map;
                    dot1.y = (int)(height_map / 2 - (dot.x) * 100) % height_map;
                    map.at<Vec3b>(dot1.y, dot1.x) = {0, 255, 255};
                }
            }
        }
        rotate(map, map, 2);
        imshow("Map", map);
        //delay N millis, usually long enough to display and capture input
        char key = (char)waitKey(30);
        if(key >= 0) break;
    }
    video.release();
    destroyAllWindows();
}

Mat lab5_class::draw_map ( Mat &img )
{
    for (int x = 0; x < img.cols; x = x + 20)
        line(img, Point(0, x), Point(img.rows, x), Scalar(100, 100, 100), 1, 1);
    for (int y = 0; y < img.rows; y += 20)
        line(img, Point(y, 0), Point(y, img.cols), Scalar(100, 100, 100), 1, 1);
    return img;
}

Point2d lab5_class::map_pixel(int col, int row)
{
    Point2d dot = Point2d (0, 0);
    double px = (col - cx) / fx;
    double py = (row - cy) / fy;
    double pz = 1;
    double mod = pow((pow(px, 2.0) + pow(py, 2.0) + pow(pz, 2.0)), 0.5);
    double ex = px / mod;
    double ey = py / mod;
    double ez = pz / mod;
    double k = static_cast < double > ( -d ) / ey; //ั. ะบ. n = [0 1 0]
    dot.x = k * ex;
    dot.y = k * ez;
    return dot;
}
