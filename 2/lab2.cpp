#include "lab2.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/// Remake somehow
void lab2( )
{
    Mat img = imread("../2/Lenna.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//    resize(img, img, Size(0, 0), 0.5, 0.5);
    Mat lib_blur = img.clone( );
    Mat cust_blur = img.clone( );
    Mat compare = img.clone( );

    Size filter_size(3, 3);

    TickMeter msec_timer;

    msec_timer.reset( );
    msec_timer.start( );
    // библиотечное размытие
    blur(lib_blur, lib_blur, filter_size);
    msec_timer.stop( );
    imshow("OpenCV_Filter", lib_blur);
    cout << "OpenCV time = " << msec_timer.getTimeMilli( ) << " msec" << endl;

    msec_timer.reset( );
    msec_timer.start( );
    // моё размытие
    lab_box_filter(cust_blur, filter_size);
    msec_timer.stop( );
    imshow("Custom_Filter", cust_blur);
    cout << "Custom time = " << msec_timer.getTimeMilli( ) << " msec" << endl;

    // сравнение размытых изображений
    float similarity = compare_blurs(cust_blur, lib_blur);
    cout << "Similarity of images is : " << similarity * 100 << "%" << endl;
    imshow("Original", img);

    waitKey(0);
}

void lab_box_filter( Mat res, Size filter_size )
{
    // ширина изображения без крайних пикселей
    int width = res.cols - 2;
    // площадь (размер) изображения без крайних пикселей
    int imgSize = (res.cols - 2) * (res.rows - 2);

    Point startROI(0, 0);
    // область интереса
    Rect ROI = Rect(startROI, filter_size);
    // размер области интереса берём как входное значение функции (по-умолчанию 3х3)
    int sizeROI = filter_size.height * filter_size.width;

    // проходимся областью интереса по всему изображению
    for ( int i = 0; i < imgSize; i++ )
    {
        // вычисления нынешней координаты ROI-я (тут матрица рассматривается как одномерный массив)
        // х - остаток от деления нынешнего положения в массиве на ширину изображения
        // у - целое от деления нынешнего положения на ширину
        ROI.x = i % width;
        ROI.y = i / width;
        // встраиваем ROI на эти координаты (координаты отвечают за левый верхний угол)
        Mat image_ROI = res(ROI);

        int pixel = 0;

        int xInROI = 0;
        int yInROI = 0;
        // проходимся по всем пикселам, попавшим в ROI и находим их среднее
        for ( int j = 0; j < sizeROI; j++ )
        {
            // вычисление нынешней локальной координаты внутри ROI-я (тот же принцип, что выше)
            xInROI = j % filter_size.width;
            yInROI = j / filter_size.height;
            // находим сумму пикселей внутри ROI
            // у box-фильтра ядро состоит из единиц, поэтому домножение на них не отображается
            pixel += image_ROI.at < uchar >(yInROI, xInROI);
        }
        // делим на сумму ядра (сумма всех единиц ядра)
        pixel /= sizeROI;
        // вычисляем координаты центрального пиксела ROI
        // т.к ширина и длина ядер нечётные достаточно целочисленного деления (округлит в меньшую сторону, что и будет центром)
        int x = filter_size.width / 2;
        int y = filter_size.height / 2;
        // присваиваем найденное значение соответсвующему пикселу оригинального значения
        image_ROI.at < uchar >(y, x) = ( uchar ) pixel;
    }
}

float compare_blurs( Mat compare1, Mat compare2 )
{
    int width = compare1.cols;
    int height = compare1.rows;
    // сумма одинаковых (или почти) пикселов
    float simil;
    // попиксельно сравниваем оба изображения
    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            // если разница между соответсвующими пикселами изображений небольшая, то добавляем к сумме единичку
            if ( abs(compare1.at < uchar >(i, j) - compare2.at < uchar >(i, j)) < 10 )
            {
                simil++;
            }
        }
    }
    // получаем отношение кол-ва одинаковых пикселов к кол-ву всех пикселов (потом из этого можно сделать проценты)
    simil = simil / (width * height);
    return simil;
}
