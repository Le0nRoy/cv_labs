#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

static string windowLinesOriginal = "lines_original";
static string windowLinesThreshed = "lines_threshed";
static string windowLinesSkeleted = "lines_skeleted";

static int trackbarValueWaitTimeSkelet = 10;
static const int trackbarLimitWaitTimeSkelet = 1000;

static int trackbarValueHoughThresh = 50;
static const int trackbarLimitHoughThresh = 100;

static const int threshMaxVal = 255;

/**
 * @note найти HoughLines, который выдает две точки на отрезок
 *      убрать все ,что вышел линии горизонта ( середина изображения? )
 *      при помощи открытия убрать все ,кроме близжайшей линии
 *      должна остаться только эта линия и выбрать произвольную точку на ней
 *      на исходном ( отфильтроаном ) изображении на выбранную точку применить заливку
 */

bool lab6_class::task_lines ( )
{
    if ( !linesLoaded )
    {
        cout << "task_lines () : Video is not loaded !" << endl;
        return false;
    }

    Mat frame;
    linesVideo.read ( frame );
    if ( frame.empty ( ) )
    {
        cout << "task_lines () : Frame is not loaded !" << endl;
        return false;
    }

    make_windows_lines ( );

    int exitCondition = 1;
    createTrackbar ( "Time_skelet", windowLinesSkeleted, &trackbarValueWaitTimeSkelet, trackbarLimitWaitTimeSkelet );
    createTrackbar ( "Hough_thresh", windowLinesOriginal, &trackbarValueHoughThresh, trackbarLimitHoughThresh );
    createTrackbar ( "turn_off", windowLinesOriginal, &exitCondition, 1 );

    imshow ( windowLinesOriginal, frame );
    waitKey ( 0 );

    while ( !frame.empty ( ) && ( exitCondition != 0 ) )
    {
        Mat skelet;
        skeletezation ( frame, skelet );
        imshow ( windowLinesSkeleted, skelet );

        vector < Vec4i > lines;
        find_lines ( skelet, lines );
        if ( lines.empty ( ) )
        {
            cout << "No lines found" << endl;
            destroy_windows_lines ( );
            return false;

        }

//        together ( frame, lines );
        merge_lines ( skelet, frame, lines );

        imshow ( windowLinesOriginal, frame );
        waitKey ( 1 );
        linesVideo.read ( frame );
    }

    waitKey ( 0 );

    destroy_windows_lines ( );
    return true;
}

void lab6_class::skeletezation ( InputArray src, OutputArray skeleted_img )
{
    Mat srcImg;
    srcImg = src.getMat_ ( );
    if ( srcImg.empty ( ) )
    {
        cout << "skeletezation () : Image is empty !" << endl;
        return;
    }

    Mat thresh_img;
    skeleted_img.create ( srcImg.size ( ), CV_8U );
    thresh_img = skeleted_img.getMat_ ( );

    blur ( srcImg, thresh_img, Size ( 5, 5 ) );
    cvtColor ( srcImg, thresh_img, CV_BGR2GRAY );
    inRange ( srcImg, Scalar ( 84, 72, 61 ), Scalar ( 156, 147, 150 ), thresh_img );
//    inRange(thresh_img, 70, 180, thresh_img);
    clear_above_horizont ( thresh_img );
    imshow ( "thresh", thresh_img );

//    namedWindow ( "hsvImg", 1 );
//    int h = 84;
//    int s = 72;
//    int v = 61;
//    int hMax = 156;
//    int sMax = 147;
//    int vMax = 150;
//    createTrackbar ( "h", "hsvImg", &h, 180 );
//    createTrackbar ( "s", "hsvImg", &s, 255 );
//    createTrackbar ( "v", "hsvImg", &v, 255 );
//    createTrackbar ( "hMax", "hsvImg", &hMax, 180 );
//    createTrackbar ( "sMax", "hsvImg", &sMax, 255 );
//    createTrackbar ( "vMax", "hsvImg", &vMax, 255 );
//    while ( waitKey ( 100 ) != 27 )
//    {
//        inRange ( srcImg, Scalar ( h, s, v ), Scalar ( hMax, sMax, vMax ), thresh_img );
//        imshow ( "hsvImg", thresh_img );
//    }
//    destroyWindow ( "hsvImg" );

//    imshow ( windowLinesThreshed, thresh_img );

    bool imgChanged = true;
    int i = 0;
    const int iterationsStop = 100;
    while ( imgChanged &&
            ( i < iterationsStop ) )
    {
        skeletezation_iter ( thresh_img, thresh_img, 0 );
        imgChanged = skeletezation_iter ( thresh_img, thresh_img, 1 );

//        imshow ( windowLinesSkeleted, thresh_img );
//        waitKey ( trackbarValueWaitTimeSkelet );
        i++;
    }
    cout << "Skeletezation finished after " << i << " iterations." << endl;
}

void lab6_class::clear_above_horizont ( cv::InputOutputArray image )
{
    Mat img = image.getMat ( );
    if ( img.empty ( ) )
    {
        cout << " clear_above_horizont ( ) : empty matrix ! " << endl;
        return;
    }

    for ( int y = 0; y < img.rows / 2; y++)
    {
        for ( int x = 0; x < img.cols; x++ )
        {
            img.at < uchar > ( y, x ) = 0;
        }
    }
}

bool lab6_class::skeletezation_iter ( const _InputArray &img, const _OutputArray &skeleted_img, int iter )
{
    Mat thresh_img = img.getMat_ ( );
    if ( thresh_img.type ( ) != CV_8UC1 )
    {
        cout << " skeletezation_iter ( ) : wrong type of matrix ! " << endl;
        return false;
    }

    // Создаём матрицу для OutputImage
    skeleted_img.create ( thresh_img.size ( ), thresh_img.type ( ) );
    Mat dst = skeleted_img.getMat ( );

    /*******************************************

                    P9  P2  P3

                    P8  P1  P4

                    P7  P6 P5

    *******************************************/

    Mat flagMap = Mat::zeros ( thresh_img.size ( ), CV_8U );

    bool imgChanged = false;
    for ( int y = 1 ; y < dst.rows - 1 ; y++ )
    {
        for ( int x = 1 ; x < dst.cols - 1 ; x++ )
        {
            if ( dst.at < uchar > ( y, x ) != 0 )
            {
                uchar neighbs [ 8 ] = { 0 };
                neighbours ( dst, Point ( x, y ), neighbs );

                int A = trans ( neighbs );

                int B = 0;
                for ( int i = 0 ; i < 8 ; i++ )
                {
                    B += ( neighbs[ i ] != 0 ) ? 1 : 0;
                }

                // step 1 -> P2 * P4 * P6
                // step 2 -> P2 * P4 * P8
                int m1 = ( iter == 0 ) ? ( neighbs[ 0 ] * neighbs[ 2 ] * neighbs[ 4 ] ) :
                         ( neighbs[ 0 ] * neighbs[ 2 ] * neighbs[ 6 ] );
                // step 1 -> P4 * P6 * P8
                // step 2 -> P2 * P6 * P8
                int m2 = ( iter == 0 ) ? ( neighbs[ 2 ] * neighbs[ 4 ] * neighbs[ 6 ] ) :
                         ( neighbs[ 0 ] * neighbs[ 4 ] * neighbs[ 6 ] );

                if ( ( A == 1 ) &&
                     ( B >= 2 && B <= 6 ) &&
                     ( m1 == 0 && m2 == 0 ) )
                {
                    flagMap.at < uchar > ( y, x ) = threshMaxVal;
                }
            }
        }
    }

    // Тут происходит удаление пиксела
    for ( int y = 1 ; y < dst.rows - 1 ; y++ )
    {
        for ( int x = 1 ; x < dst.cols - 1 ; x++ )
        {
            if ( flagMap.at < uchar > ( y, x ) == threshMaxVal )
            {
                dst.at < uchar > ( y, x ) = 0;
                imgChanged = true;
            }
        }
    }

    return imgChanged;
}

void lab6_class::neighbours ( InputArray img, Point pix, uchar *neighbs )
{
    Mat image = img.getMat ( );
    if ( image.empty ( ) )
    {
        cout << "neighbours () : Image is empty !" << endl;
        return;
    }

    neighbs[ 0 ] = image.at < uchar > ( pix.y - 1, pix.x );
    neighbs[ 1 ] = image.at < uchar > ( pix.y - 1, pix.x + 1 );
    neighbs[ 2 ] = image.at < uchar > ( pix.y, pix.x + 1 );
    neighbs[ 3 ] = image.at < uchar > ( pix.y + 1, pix.x + 1 );
    neighbs[ 4 ] = image.at < uchar > ( pix.y + 1, pix.x );
    neighbs[ 5 ] = image.at < uchar > ( pix.y + 1, pix.x - 1 );
    neighbs[ 6 ] = image.at < uchar > ( pix.y, pix.x - 1 );
    neighbs[ 7 ] = image.at < uchar > ( pix.y - 1, pix.x - 1 );
}

int lab6_class::trans ( uchar *neighb )
{
    if ( neighb == nullptr )
    {
        cout << "trans ( ) : array of neighbours does not exist" << endl;
        return -1;
    }

    int res = 0;
    for ( int i = 0 ; i < 7 ; i++ )
    {
        if ( neighb[ i ] == 0 && neighb[ i + 1 ] != 0 )
        {
            res++;
        }
    }

    if ( neighb[ 7 ] == 0 && neighb[ 0 ] != 0 )
    {
        res++;
    }

    return res;
}

void lab6_class::find_lines ( InputArray skel_img, vector < Vec4i > &lines )
{
    Mat img;
    img = skel_img.getMat_ ( );
    if ( img.empty ( ) || img.channels ( ) != 1 )
    {
        cout << "find_lines ( ) : Image is not prepared for function!" << endl;
        return;
    }

//    HoughLinesP ( img, lines, 1, CV_PI / 180, trackbarValueHoughThresh );
    HoughLinesP ( img, lines, 1, CV_PI / 180, trackbarValueHoughThresh, 50, 80 );
}

void lab6_class::together ( cv::InputOutputArray image, vector < Vec4i > &lines )
{
    Mat frame = image.getMat ( );
    if ( frame.empty ( ) )
    {
        cout << " together ( ) : empty matrix ! " << endl;
        return;
    }

    Point d;
    int r_min = 25;

    for (uint k = 0; k < lines.size(); k++)
    {
        Vec4i l = lines[k];
        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i j = lines[i];
            if (i != k)
            {
                if (abs(l[0] - j[2]) < r_min && abs(l[1] - j[3]) < r_min)
                {
                    line(frame, Point(l[0], l[1]), Point(j[2], j[3]), Scalar(0, 0, 255), 3, CV_AA);
                }
                if (abs(l[2] - j[0]) < r_min && abs(l[3] - j[1]) < r_min)
                {
                    line(frame, Point(l[2], l[3]), Point(j[0], j[1]), Scalar(0, 0, 255), 3, CV_AA);
                }
                if (abs(l[0] - j[0]) < r_min && abs(l[1] - j[1]) < r_min)
                {
                    line(frame, Point(l[0], l[1]), Point(j[0], j[1]), Scalar(0, 0, 255), 3, CV_AA);
                }
                if (abs(l[2] - j[2]) < r_min && abs(l[3] - j[3]) < r_min)
                {
                    line(frame, Point(l[2], l[3]), Point(j[2], j[3]), Scalar(0, 0, 255), 3, CV_AA);
                }
            }
        }
    }
}

void lab6_class::merge_lines ( InputArray skel_img, InputOutputArray drawImage, std::vector < cv::Vec4i > &lines )
{
    if ( !linesLoaded )
    {
        cout << "merge_lines () : Lines are not loaded !" << endl;
        return;
    }
    Mat skeletedImage;
    skeletedImage = skel_img.getMat_ ( );

    Mat imageWithLines;
    imageWithLines = drawImage.getMat_ ( );

    assert ( !skeletedImage.empty ( ) );
    assert ( !imageWithLines.empty ( ) );

    try
    {
        // Подсчет всех тангенсов
//        std::vector < double > linesTan;
//        for ( uint i = 0; i < lines.size ( ); i++ )
//        {
//            //      ( y2 - y1 )
//            // tan = ---------
//            //      ( x2 - x1 )
//            linesTan.push_back( static_cast < double > ( lines.at ( i )[ 3 ] - lines.at ( i )[ 1 ] )
//                    / static_cast < double > ( lines.at ( i )[ 2 ] - lines.at ( i )[ 0 ] ) );
//        }

//        // Поиск первой точки
//        // Первая точка в любом случае находится в самом низу изображения
//        std::vector <cv::Vec4i > sortedLines ( lines.size ( ) );
//        sortedLines.insert ( sortedLines.begin ( ),
//                             cv::Vec4i ( lines.at ( 0 )[ 0 ], sortedLines.at ( 0 )[ 1 ], 0, 0 ) );
////        cv::Point firstPoint ( lines.at ( 0 )[ 0 ], skeletedImage.rows - 1 );
//        uint pointNum = 0;
//        if ( lines.at ( 0 )[ 2 ] > lines.at ( 0 )[ 0 ] )
//        {
//            sortedLines.at ( 0 )[ 0 ] = lines.at ( 0 )[ 2 ];
//        }
//        if ( lines.at ( 0 )[ 3 ] > lines.at ( 0 )[ 1 ] )
//        {
//            sortedLines.at ( 0 )[ 1 ] = lines.at ( 0 )[ 3 ];
//        }
//        for ( uint i = 0; i < lines.size ( ); i++ )
//        {
//            if ( ( lines.at ( i )[ 0 ] > sortedLines.at ( 0 )[ 0 ] &&
//                   lines.at ( i )[ 1 ] > sortedLines.at ( 0 )[ 1 ] ) ||
//                 ( lines.at ( i )[ 2 ] > sortedLines.at ( 0 )[ 0 ] &&
//                   lines.at ( i )[ 3 ] > sortedLines.at ( 0 )[ 1 ] ) )
//            {

//                // Первая линия не может быть горизонтальной
//                // ( скелетезация дает снизу горизонтальный кусок, который не нужно воспринимать )
//                if ( lines.at ( i )[ 0 ] != lines.at ( i )[ 2 ] )
//                {
//                // Первая точка находится ниже остальных
//                    if ( lines.at ( i )[ 1 ] > lines.at ( i )[ 3 ] )
//                    {
//                        sortedLines.at ( 0 )[ 0 ] = lines.at ( i )[ 0 ];
//                        sortedLines.at ( 0 )[ 2 ] = lines.at ( i )[ 2 ];
//                        sortedLines.at ( 0 )[ 3 ] = lines.at ( i )[ 3 ];
//                    }
//                    else
//                    {
//                        sortedLines.at ( 0 )[ 0 ] = lines.at ( i )[ 2 ];
//                        sortedLines.at ( 0 )[ 2 ] = lines.at ( i )[ 0 ];
//                        sortedLines.at ( 0 )[ 3 ] = lines.at ( i )[ 1 ];
//                    }
//                    pointNum = i;
//                    sortedLines.at ( 0 )[ 1 ] = skeletedImage.rows - 1;
//                }
//            }
//        }

//        // Поиск следуюих точек как точек пересечения с предыдущей линией и взятия минимально удаленной точки
//        unsigned long minDistance = skeletedImage.rows - 1;
//        cv::Point newPoint;
//        for ( uint i = 0; i < sortedLines.size ( ) - 1; i++ )
//        {
//            for ( uint j = 0; j < lines.size ( ); j++ )
//            {
//                if ( i != j && ( i != pointNum || j != pointNum ) )
//                {
//                    cv::Point crossPoint = countCrossPoint( lines.at ( i ), lines.at ( j ) );
//                    unsigned long dist = cvRound ( sqrt ( pow ( lines.at ( i )[ 0 ] - crossPoint.x, 2 ) +
//                                                pow ( lines.at ( i )[ 1 ] - crossPoint.y, 2 ) ) );
//                    if ( dist < minDistance )
//                    {
//                        minDistance = dist;
//                        pointNum = j;
//                        newPoint = crossPoint;
//                    }
//                }
//            }
//            sortedLines.at ( i )[ 2 ] = newPoint.x;
//            sortedLines.at ( i )[ 3 ] = newPoint.y;
//            std::vector < cv::Vec4i >::iterator iterator = sortedLines.begin ( ) /*+ i + 1*/;
//            sortedLines.insert ( iterator,
//                                 cv::Vec4i ( newPoint.x, newPoint.y, 0, 0 ) );
//        }

        draw_lines ( imageWithLines, lines, true );
    }
    catch ( exception &e )
    {
        cout << endl << "merge_lines : MY EXCEPTION"
             << endl << e.what ( ) << endl;
    }
}

cv::Point lab6_class::countCrossPoint ( cv::Vec4i line1, cv::Vec4i line2 )
{
    if ( line1 [ 0 ] == line1 [ 2 ] ||
         line1 [ 1 ] == line1 [ 3 ] ||
         line2 [ 0 ] == line2 [ 2 ] ||
         line2 [ 1 ] == line2 [ 3 ] )
    {
        return cv::Point ( 0, 0 );
    }
    double k1 = ( line1 [ 3 ] - line1 [ 1 ] ) / ( line1 [ 2 ] - line1 [ 0 ] );
    double k2 = ( line2 [ 3 ] - line2 [ 1 ] ) / ( line2 [ 2 ] - line2 [ 0 ] );

    int xcross = static_cast < int > (
                     ( k1 * line1 [ 0 ] - k2 * line2 [ 0 ] + line2 [ 1 ] - line1 [ 1 ] ) /
                     ( k1 - k2 )
                );
    int ycross = static_cast < int > ( ( xcross - line1 [ 0 ] ) * k1 + line1 [ 1 ] );
    return cv::Point ( xcross, ycross );
}

void lab6_class::draw_lines ( InputOutputArray img, std::vector < cv::Vec4i > lines, bool threeColors )
{
    Mat image;
    image = img.getMat_ ( );

    // Рисуем линии
    Scalar color;
    if ( threeColors )
    {
        color = Scalar ( 0, 0, 255 );
    }
    else
    {
        color = Scalar ( 255, 0, 0 );
    }

    for ( uint i = 0 ; i < lines.size ( ) ; i++ )
    {
        Point pt1 ( lines.at ( i ) [ 0 ], lines.at ( i ) [ 1 ] );
        Point pt2 ( lines.at ( i ) [ 2 ], lines.at ( i ) [ 3 ] );

        if ( threeColors )
        {
            color [ 0 ] = i;
        }

        line ( image, pt1, pt2, color, 2, CV_AA );
    }
}

// TODO надо б список имен окон в виде вектора возвращать, а то че
void lab6_class::make_windows_lines ( )
{
    namedWindow ( windowLinesOriginal, 1 );
    namedWindow ( windowLinesSkeleted, 1 );
}

void lab6_class::destroy_windows_lines ( )
{
    destroyWindow ( windowLinesOriginal );
    destroyWindow ( windowLinesSkeleted );
}
