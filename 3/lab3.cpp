#include "lab3.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void lab3()
{
    /// Load filenames of images
    vector<string> images = getFilesLab3();
    char control = getchar();
    while (control != '0')
    {
        /// Filename of image for function checking
        string imgForFunc;
        switch (control)
        {
            case '1':
            {
                /// First part (houses)
//                imgForFunc = images.at(0);
//                imgForFunc = images.at(1);
                imgForFunc = images.at(2);
                findHouse(imgForFunc);
                destroyAllWindows();
                break;
            }
            case '2':
            {
                /// Second part (engines)
                /// may be can do smth with 9 and 11
//                imgForFunc = images.at(7);
//                imgForFunc = images.at(8);
//                imgForFunc = images.at(9);
//                imgForFunc = images.at(10);
                imgForFunc = images.at(11);
                findEngine(imgForFunc);
                destroyAllWindows();
                break;
            }
            case '3':
            {
                /// Third part (robots, lamp)
//                imgForFunc = images.at(5);
                imgForFunc = images.at(6);
                findRobots(imgForFunc);
                destroyAllWindows();
                break;
            }
            case '4':
            {
                /// Fourth part (wrench)
                imgForFunc = images.at(3);
                string wrenchTemplate = images.at(4);
                findWrench(imgForFunc, wrenchTemplate);
                destroyAllWindows();
                break;
            }
            default:
                break;
        }
        control = getchar();
    }

    destroyAllWindows();
}

/// Good
vector<string> getFilesLab3()
{
    vector<string> images(15); // 12 pictures for lab3, but allocate more memory as stock

    string wayToFolder = "../3/Tasks/img_zadan"; // Way from home to folder with folders that contain images

    /// allababah
    images.at(0)  = wayToFolder+"/allababah/ig_0.jpg";
    images.at(1)  = wayToFolder+"/allababah/ig_1.jpg";
    images.at(2)  = wayToFolder+"/allababah/ig_2.jpg";

    /// gk
    images.at(3)  = wayToFolder+"/gk/gk.jpg";
    images.at(4)  = wayToFolder+"/gk/gk_tmplt.jpg";

    /// roboti
    images.at(5)  = wayToFolder+"/roboti/roi_robotov.jpg";
    images.at(6)  = wayToFolder+"/roboti/roi_robotov_1.jpg";

    /// teplovizor
    images.at(7)  = wayToFolder+"/teplovizor/21331.res.jpg";
    images.at(8)  = wayToFolder+"/teplovizor/ntcs_quest_measurement.png";
    images.at(9)  = wayToFolder+"/teplovizor/445923main_STS128_16FrAvg_color1.jpg";
    images.at(10) = wayToFolder+"/teplovizor/size0-army.mil-2008-08-28-082221.jpg";
    images.at(11) = wayToFolder+"/teplovizor/MW-AW129-measured.jpg";

    return images;
}

/// Good
void findHouse(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty())
    {
        cout << "findHouse() : Failed to load image" << endl;
        return;
    }
    /// All works with copy of image
    Mat imgGRAY;
    img.copyTo(imgGRAY);
    /// Set to binary mode
    threshold(imgGRAY, imgGRAY, 220, 255, THRESH_BINARY);
    /// Use morphology to throw away noises
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(imgGRAY, imgGRAY, kernel, Point(-1, -1), 1);
    dilate(imgGRAY, imgGRAY, kernel, Point(-1,-1), 4);
    /// Find contours of objects
    vector<vector<Point>> contours;
    findContours(imgGRAY, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Higlight objects by it's contour
    img.copyTo(imgGRAY);
    cvtColor(imgGRAY, imgGRAY, CV_GRAY2BGR);
    polylines(imgGRAY, contours, true, Vec3b(0, 0, 255), 2, 8);
    /// Find mass center of each house
    Moments house_moments;
    for (int i = 0; i < contours.size(); i++)
    {
        /// Check if contour length is not too low
        if (contours.at(i).size() > 10)
        {
            house_moments = moments(contours.at(i) );
            int x = house_moments.m10 / house_moments.m00;
            int y = house_moments.m01 / house_moments.m00;
            circle(imgGRAY, Point(x, y), 5, Vec3b(0, 0, 255), -1);
        }
    }

    imshow("lab3.1_transformed", imgGRAY);
    waitKey(0);
}

/// Good
void findEngine(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findEngine() : Failed to load image" << endl;
        return;
    }
    /// All works with copy of image
    // Change to imgHSV
    Mat img_copy;
    img.copyTo(img_copy);
    /// Convert to HSV
    cvtColor(img, img_copy, CV_BGR2HSV);
    cvtColor(img, img, CV_BGR2HSV);
    /// Find objects
    inRange(img_copy, Vec3b(0, 0, 0), Vec3b(35, 255, 255), img_copy);
    /// Use morphology to throw away noises
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(img_copy, img_copy, kernel, Point(-1, -1), 1);
    dilate(img_copy, img_copy, kernel, Point(-1, -1), 3);
    /// Find contours of objects
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Higlight objects by it's contour
    img.copyTo(img_copy);
    cvtColor(img_copy, img_copy, CV_HSV2BGR);
    polylines(img_copy, contours, true, Vec3b(0, 0, 255), 2, 8);
    /// Find mass center of each house
    Moments house_moments;
    for (int i = 0; i < contours.size(); i++)
    {
        /// Check if contour length is not too low
        if (contours.at(i).size() > 10)
        {
            house_moments = moments(contours.at(i) );
            int x = house_moments.m10 / house_moments.m00;
            int y = house_moments.m01 / house_moments.m00;
            circle(img_copy, Point(x, y), 5, Vec3b(0, 0, 0), -1);
        }
    }

    imshow("lab3.2_transformed", img_copy);
    waitKey(0);
}

void findRobots(string image)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "findRobots() : Failed to load image" << endl;
        return;
    }
    Mat target = img.clone();

    ///** Selecting lamp and finding center point
    Mat lamp_pixels;
    inRange(img, Vec3b(0, 0, 248), Vec3b(179, 10, 255), lamp_pixels);
    vector<vector<Point>> lamp_cnts;
    findContours(lamp_pixels.clone(), lamp_cnts, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    int Xlamp, Ylamp;
    for (int i = 0; i < lamp_cnts.size(); i++)
    {
        if (lamp_cnts[i].size() > 70)
        {
            Moments mnts = moments(lamp_cnts[i]);
            double m00 = mnts.m00;
            double m01 = mnts.m01;
            double m10 = mnts.m10;
            Xlamp = int(m10 / m00);
            Ylamp = int(m01 / m00);
            circle(img, {Xlamp, Ylamp}, 5, Scalar(0, 0, 0), -1);
        }
    }

    ///** Blue pixels
    Mat blue_pixels;
    inRange(img, Vec3b(90, 23, 21), Vec3b(139, 231, 255), blue_pixels);
    erode(blue_pixels, blue_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    dilate(blue_pixels, blue_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    imshow("blue", blue_pixels);

    vector<vector<Point>> blue_cnts;

    findContours(blue_pixels.clone(), blue_cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int min_blue_dist, Xblue, Yblue;
    for (int i = 0; i < blue_cnts.size(); i++)
    {
        if (blue_cnts[i].size() < 70)
        {
            blue_cnts.erase(blue_cnts.begin() + i);
            i--;
        }
        else
        {
            Moments mnts = moments(blue_cnts[i]);
            double m00 = mnts.m00;
            double m01 = mnts.m01;
            double m10 = mnts.m10;
            int Xc = int(m10 / m00);
            int Yc = int(m01 / m00);
            int dist = findNearestRobot(Xc, Yc, Xlamp, Ylamp);
            if ((i == 0) || (dist < min_blue_dist))
            {
                min_blue_dist = dist;
                Xblue = Xc;
                Yblue = Yc;
            }
        }
    }
    circle(target, { Xblue, Yblue }, 5, Scalar(120, 255, 255), -1);
    drawContours(target, blue_cnts, -1, Scalar(120, 255, 255), 2);
    ///** Selecting red robots
    Mat red_pixels;
    //inRange(img, Vec3b(Hue_down, Saturation_down, Value_down), Vec3b(Hue_up, Saturation_up, Value_up), red_pixels); for debug
    inRange(img, Vec3b(10, 0, 0), Vec3b(160, 255, 255), red_pixels);
    bitwise_not(red_pixels, red_pixels);
    erode(red_pixels, red_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    dilate(red_pixels, red_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    imshow("red", red_pixels);

    vector<vector<Point>> red_cnts;
    findContours(red_pixels.clone(), red_cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int min_red_dist = 10000, Xred, Yred;

    for (int i = 0; i < red_cnts.size(); i++)
    {
        if ((red_cnts[i].size() < 150) && (red_cnts[i].size() > 40))
        {
            Moments mnts = moments(red_cnts[i]);
            double m00 = mnts.m00;
            double m01 = mnts.m01;
            double m10 = mnts.m10;
            int Xc = int(m10 / m00);
            int Yc = int(m01 / m00);
            int dist = findNearestRobot(Xc, Yc, Xlamp, Ylamp);
            if ((i == 0) || (dist < min_red_dist))
            {
                min_red_dist = dist;
                Xred = Xc;
                Yred = Yc;
            }
        }
        else
        {
            red_cnts.erase(red_cnts.begin() + i);
            i--;
        }
    }
    circle(target, { Xred, Yred }, 5, Scalar(0, 255, 255), -1);
    drawContours(target, red_cnts, -1, Scalar(0, 255, 255), 2);
    ///** Selecting green robots
    Mat green_pixels;
    inRange(img, Vec3b(40, 50, 133), Vec3b(90, 255, 255), green_pixels);
    erode(green_pixels, green_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    dilate(green_pixels, green_pixels, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)), Point(-1, -1), 1);
    imshow("green", green_pixels);

    vector<vector<Point>> green_cnts;
    findContours(green_pixels.clone(), green_cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int min_green_dist, Xgreen, Ygreen;
    for (int i = 0; i < green_cnts.size(); i++)
    {
        if (green_cnts[i].size() > 49)
        {
            Moments mnts = moments(green_cnts[i]);
            double m00 = mnts.m00;
            double m01 = mnts.m01;
            double m10 = mnts.m10;
            int Xc = int(m10 / m00);
            int Yc = int(m01 / m00);
            int dist = findNearestRobot(Xc, Yc, Xlamp, Ylamp);
            if ((i == 0) || (dist < min_green_dist))
            {
                min_green_dist = dist;
                Xgreen = Xc;
                Ygreen = Yc;
            }
        }
        else
        {
            green_cnts.erase(green_cnts.begin() + i);
            i--;
        }
    }
    circle(target, { Xgreen, Ygreen }, 5, Scalar(60, 255, 255), -1);
    drawContours(target, green_cnts, -1, Scalar(60, 255, 255), 2);


    cvtColor(target, target, CV_HSV2BGR);
    imshow("Target point ", target);
    waitKey(0);
}


//void findRobots(string image)
//{
//    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
//    if (img.empty())
//    {
//        cout << "findEngine() : Failed to load image" << endl;
//        return;
//    }
//    /// All works with copy of image
//    Mat img_copy;
//    img.copyTo(img_copy);
//    // Blue
//    inRange(img_copy, Vec3b(0, 50, 50), Vec3b(255, 150, 150), img_copy);
//    imshow("blue", img_copy);
//    waitKey(0);
//
//}

int findNearestRobot(int X1, int Y1, int X2, int Y2)
{
    int dist = sqrt(pow((X1 - X2), 2) + pow((Y1 - Y2), 2));
    return dist;
}

/// Good
void findWrench(string image, string objectTemplate)
{
    Mat img = imread(image, CV_LOAD_IMAGE_COLOR);
    resize(img, img, Size(), 0.75, 0.75);
    Mat temp = imread(objectTemplate, CV_LOAD_IMAGE_COLOR);
    if (img.empty() )
    {
        cout << "findWrench() : Failed to load image" << endl;
        return;
    }
    else if (temp.empty() )
    {
        cout << "findWrench() : Failed to load template" << endl;
        return;
    }
    ///*** Find contours of main image
    // Work with copy of image (resized not to deal with such a giant)
    Mat img_copy;
    cvtColor(img, img_copy, COLOR_BGR2GRAY);
    // Get binary image for simplier compare
    threshold(img_copy, img_copy, 240, 255, THRESH_BINARY_INV);
    // Find contours of objects
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    ///*** Find contours of template
    // All works with copy of image
    Mat temp_copy;
    cvtColor(temp, temp_copy, COLOR_BGR2GRAY);
    threshold(temp_copy, temp_copy, 240, 255, THRESH_BINARY_INV);
    vector<vector<Point>> temp_contours;
    findContours(temp_copy, temp_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    /// Compare mask with each object and higlight in different color if similar or not
    for (int i = 0; i < contours.size(); i++)
    {
        float diff = (float)matchShapes(contours.at(i), temp_contours.at(0), CV_CONTOURS_MATCH_I2, 0);
        /// If not big difference, then object is similar to template
        /// (may be difference is counted as percentage?)
        if (abs(diff) > 0.9)
        {
            polylines(img, contours.at(i), true, Vec3b(0, 255, 0), 2, 8);
        }
        else
        {
            polylines(img, contours.at(i), true, Vec3b(0, 0, 255), 2, 8);
        }
    }

    imshow("img", img);
    waitKey(0);
}
