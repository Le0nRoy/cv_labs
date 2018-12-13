#include "lab3.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "lab3_3.h"

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
                imgForFunc = images.at(0);
                findHouse(imgForFunc);
                imgForFunc = images.at(1);
                findHouse(imgForFunc);
                imgForFunc = images.at(2);
                findHouse(imgForFunc);
                destroyAllWindows();
                break;
            }
            case '2':
            {
                /// Second part (engines)
                /// may be can do smth with 9 and 11
                imgForFunc = images.at(7);
                findEngine(imgForFunc);
                imgForFunc = images.at(8);
                findEngine(imgForFunc);
                imgForFunc = images.at(9);
                findEngine(imgForFunc);
                imgForFunc = images.at(10);
                findEngine(imgForFunc);
                imgForFunc = images.at(11);
                findEngine(imgForFunc);
                destroyAllWindows();
                break;
            }
            case '3':
            {
                /// Third part (robots, lamp)
                imgForFunc = images.at(5);
                lab3_3(imgForFunc);
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
    // делаем копию изображения
    Mat imgGRAY;
    img.copyTo(imgGRAY);
    // бинаризуем (всё кроме домика станет чёрным фоном, а домик будет белым)
    threshold(imgGRAY, imgGRAY, 220, 255, THRESH_BINARY);
    // убираем шумы морфологией
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
    // сужением убрали все мелкие объекты (шумы)
	erode(imgGRAY, imgGRAY, kernel, Point(-1, -1), 1);
	// расширением вернули размер домику и даже увеличили, чтоб найденный контур не наезжал на домик, а рисовался вокруг
    dilate(imgGRAY, imgGRAY, kernel, Point(-1,-1), 4);
    // находим контуры полученных домиков
    vector<vector<Point>> contours;
    findContours(imgGRAY, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //** рисуем контуры
    // снова скопировали изображение, чтоб рисовать на нём, а не на бинарке
    img.copyTo(imgGRAY);
    // конвертируем из ч/б в цветной, чтоб нарисовать контур красивой красной линией
    cvtColor(imgGRAY, imgGRAY, CV_GRAY2BGR);
    // нарисовали контур
    polylines(imgGRAY, contours, true, Vec3b(0, 0, 255), 2, 8);
    // находим центры масс через моменты
    Moments house_moments;
    // проходимся по всем контурам
    for (int i = 0; i < contours.size(); i++)
    {
        // нас интересуют только большие контуры
        if (contours.at(i).size() > 10)
        {
            // нашли моменты контура
            house_moments = moments(contours.at(i) );
            // нашли центр масс контура
            int x = house_moments.m10 / house_moments.m00;
            int y = house_moments.m01 / house_moments.m00;
            // нарисовали его
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
    Mat img_copy;
    img.copyTo(img_copy);
    // конвертируем в HSV - в нём удобнее искать движки
    cvtColor(img, img_copy, CV_BGR2HSV);
    cvtColor(img, img, CV_BGR2HSV);
    // эмпирически нашли диапазон, в котором на бинаризованное изображение попадают только движки (и чутка шумов)
    inRange(img_copy, Vec3b(0, 0, 0), Vec3b(35, 255, 255), img_copy);
    // убрали шумы
    Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));
	erode(img_copy, img_copy, kernel, Point(-1, -1), 1);
    dilate(img_copy, img_copy, kernel, Point(-1, -1), 3);
    // нашли контуры
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    // нарисовали контуры
    img.copyTo(img_copy);
    cvtColor(img_copy, img_copy, CV_HSV2BGR);
    polylines(img_copy, contours, true, Vec3b(0, 0, 255), 2, 8);
    // нашли центры масс
    Moments house_moments;
    for (int i = 0; i < contours.size(); i++)
    {
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
    // ищем контуры проверяемых объектов
    Mat img_copy;
    cvtColor(img, img_copy, COLOR_BGR2GRAY);
    // ищем на бинарном
    threshold(img_copy, img_copy, 240, 255, THRESH_BINARY_INV);
    // нашли контуры
    vector<vector<Point>> contours;
    findContours(img_copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    // ищем контур образца
    // All works with copy of image
    Mat temp_copy;
    cvtColor(temp, temp_copy, COLOR_BGR2GRAY);
    threshold(temp_copy, temp_copy, 240, 255, THRESH_BINARY_INV);
    vector<vector<Point>> temp_contours;
    findContours(temp_copy, temp_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    // сравниваем контур образца с контурами объектов
    for (int i = 0; i < contours.size(); i++)
    {
        float diff = (float)matchShapes(contours.at(i), temp_contours.at(0), CV_CONTOURS_MATCH_I2, 0);
        // если разница маленькая ,то всё ок и отмечаем зелёным
        if (abs(diff) > 0.9)
        {
            polylines(img, contours.at(i), true, Vec3b(0, 255, 0), 2, 8);
        }
        // иначе отмечаем красным
        else
        {
            polylines(img, contours.at(i), true, Vec3b(0, 0, 255), 2, 8);
        }
    }

    imshow("img", img);
    waitKey(0);
}
