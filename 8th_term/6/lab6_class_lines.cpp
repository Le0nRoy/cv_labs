#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

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

    createTrackbar ( "Time_skelet", windowLinesSkeleted, &trackbarValueWaitTimeSkelet, trackbarLimitWaitTimeSkelet );
    createTrackbar ( "Hough_thresh", windowLinesOriginal, &trackbarValueHoughThresh, trackbarLimitHoughThresh );

    imshow ( windowLinesOriginal, frame );
    waitKey ( 0 );

    // FIXME при повторном запуске скелетизации ( без релода ) даёт немного другой результат
    //      случается это из-за того, что видео неустанно бежит вперед и кадры меняются

    // FIXME скелетезация отвратительная
    Mat skelet;
    skeletezation ( frame, skelet );
//    imshow ( windowLinesSkeleted, skelet );
//    waitKey ( 1 );

    // TODO как правильно получать вектор в Input ( Output ) Array ???
    vector < Vec2f > lines;
    find_lines ( skelet, lines );
    if ( lines.empty ( ) )
    {
        cout << "No lines found" << endl;
        destroy_windows_lines ( );
        return false;

    }

    merge_lines ( lines );

    draw_lines ( frame, lines );
    imshow ( windowLinesOriginal, frame );

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

    cvtColor ( srcImg, thresh_img, CV_BGR2GRAY );
    int thresh = 70;
    threshold ( thresh_img, thresh_img, thresh, threshMaxVal, THRESH_BINARY );

    imshow ( windowLinesThreshed, thresh_img );
    waitKey ( 1 );

    bool imgChanged = true;
    int i = 0;
    // FIXME не работает условие выхода ( функция почему-то продолжает считаь, что все меняется )
//    while ( imgChanged )
    while ( i < 60 && imgChanged )
//      while ( true )
    {
        skeletezation_iter ( thresh_img, thresh_img, 0 );
        imgChanged = skeletezation_iter ( thresh_img, thresh_img, 1 );

        imshow ( windowLinesSkeleted, thresh_img );
        waitKey ( trackbarValueWaitTimeSkelet );

        if ( imgChanged )
        {
            cout << "Итерация № " << i++ << endl;
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

    skeleted_img.create ( thresh_img.size ( ), thresh_img.type ( ) );
    Mat dst = skeleted_img.getMat ( );

    /*******************************************

                    P9  P2  P3

                    P8  P1  P4

                    P7  P6 P5

    *******************************************/

    Mat flagMap = Mat::zeros ( thresh_img.size ( ), CV_8U );

    // FIXME почему-то линия в середине превращается в набор точечек, а не тонкую линию :с
    bool imgChanged = false;
    for ( int y = 1 ; y < dst.rows - 1 ; y++ )
    {
        for ( int x = 1 ; x < dst.cols - 1 ; x++ )
        {
            uchar neighbs[8] = { 0 };
            neighbours ( dst, Point ( x, y ), neighbs );

            int A = trans ( neighbs );

            int B = 0;
            for ( int i = 0 ; i < 8 ; i++ )
            {
                B += neighbs[ i ];
            }

            // step 1 -> P2 * P4 * P6
            // step 2 -> P2 * P4 * P8
            int m1 = ( iter == 0 ) ? ( neighbs[ 0 ] * neighbs[ 2 ] * neighbs[ 4 ] ) :
                     ( neighbs[ 0 ] * neighbs[ 2 ] * neighbs[ 6 ] );
            // step 1 -> P4 * P6 * P8
            // step 2 -> P2 * P6 * P8
            int m2 = ( iter == 0 ) ? ( neighbs[ 2 ] * neighbs[ 4 ] * neighbs[ 6 ] ) :
                     ( neighbs[ 0 ] * neighbs[ 4 ] * neighbs[ 6 ] );

            if ( ( A == 1 * threshMaxVal ) &&
                 ( B >= 2 * threshMaxVal && B <= 6 * threshMaxVal ) &&
                 ( m1 == 0 && m2 == 0 ) )
            {
                flagMap.at < uchar > ( y, x ) = 255;
                imgChanged = true;
            }
        }
    }

    dst = dst - flagMap;

    return imgChanged;
}

void lab6_class::neighbours ( const _InputArray &img, Point pix, uchar *neighbs )
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
    if ( neighb == NULL )
    {
        cout << "trans ( ) : array of neighbours does not exist" << endl;
        return -1;
    }
    int res = 0;
    for ( int i = 0 ; i < 7 ; i++ )
    {
        if ( neighb[ i ] == 0 && neighb[ i + 1 ] == 1 )
        {
            res++;
        }
    }
}

void lab6_class::find_lines ( InputArray skel_img, vector < Vec2f > &lines )
{
    // Let Hough find all lines
    Mat img;
    img = skel_img.getMat_ ( );
    if ( img.empty ( ) || img.channels ( ) != 1 )
    {
        cout << "find_lines ( ) : Image is not prepared for function!" << endl;
        return;
    }

    HoughLines ( img, lines, 1, CV_PI / 180, trackbarValueHoughThresh );
    cout << lines.size ( ) << endl;
}

void lab6_class::merge_lines ( std::vector < cv::Vec2f > lines )
{
    if ( !linesLoaded )
    {
        cout << "merge_lines () : Lines are not loaded !" << endl;
        return;
    }
    // соединять, офкорс, только сопряженные линии и на выходе наверн иметь массив массивов
}

void lab6_class::draw_lines ( InputOutputArray img, std::vector < cv::Vec2f > lines )
{
    Mat image;
    image = img.getMat_ ( );
    // Draw lines on image
    float rho = 0;
    float theta = 0;
    for ( int i = 0 ; i < lines.size ( ) ; i++ )
    {
        rho = lines.at ( i )[ 0 ];
        theta = lines.at ( i )[ 1 ];
        Point pt1, pt2;
        double a = cos ( theta );
        double b = sin ( theta );
        double x0 = a * rho;
        double y0 = b * rho;
        pt1.x = cvRound ( x0 + 1000 * ( -b ) );
        pt1.y = cvRound ( y0 + 1000 * ( a ) );
        pt2.x = cvRound ( x0 - 1000 * ( -b ) );
        pt2.y = cvRound ( y0 - 1000 * ( a ) );
        line ( image, pt1, pt2, Scalar ( 0, 0, 255 ), 2, CV_AA );
    }
}

// TODO надо б список имен окон в виде вектора возвращать, а то че
void lab6_class::make_windows_lines ( )
{
    namedWindow ( windowLinesOriginal, 1 );
    namedWindow ( windowLinesThreshed, 1 );
    namedWindow ( windowLinesSkeleted, 1 );
}

void lab6_class::destroy_windows_lines ( )
{
    destroyWindow ( windowLinesOriginal );
    destroyWindow ( windowLinesThreshed );
    destroyWindow ( windowLinesSkeleted );
}