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

static int trackbarValueHoughThresh = 55;
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

    while ( !frame.empty ( ) )
    {
        Mat skelet;
        skeletezation ( frame, skelet );

        // TODO как правильно получать вектор в Input ( Output ) Array ???
        vector < Vec2f > lines;
        find_lines ( skelet, lines );
        if ( lines.empty ( ) )
        {
            cout << "No lines found" << endl;
            destroy_windows_lines ( );
            return false;

        }

        merge_lines ( skelet, frame, lines );

        imshow ( windowLinesOriginal, frame );
        waitKey ( 5 );
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

    cvtColor ( srcImg, thresh_img, CV_BGR2GRAY );
//    int thresh = 70;
//    threshold ( thresh_img, thresh_img, thresh, threshMaxVal, THRESH_BINARY );
    inRange ( srcImg, Scalar ( 84, 72, 61 ), Scalar ( 156, 147, 150 ), thresh_img );

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

    imshow ( windowLinesThreshed, thresh_img );
//    waitKey ( 1 );

    bool imgChanged = true;
    int i = 0;
    const int iterationsStop = 100;
    while ( imgChanged &&
            ( i < iterationsStop ) )
    {
        skeletezation_iter ( thresh_img, thresh_img, 0 );
        imgChanged = skeletezation_iter ( thresh_img, thresh_img, 1 );

        imshow ( windowLinesSkeleted, thresh_img );
//        waitKey ( trackbarValueWaitTimeSkelet );

//        cout << "Iteration #" << i++ << endl;
        i++;
    }
    waitKey ( 1 );
    cout << "Skeletezation finished after " << i << " iterations." << endl;
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

void lab6_class::find_lines ( InputArray skel_img, vector < Vec2f > &lines )
{
    Mat img;
    img = skel_img.getMat_ ( );
    if ( img.empty ( ) || img.channels ( ) != 1 )
    {
        cout << "find_lines ( ) : Image is not prepared for function!" << endl;
        return;
    }

    HoughLines ( img, lines, 1, CV_PI / 180, trackbarValueHoughThresh );
}

void lab6_class::merge_lines ( InputArray skel_img, InputOutputArray drawImage, std::vector < cv::Vec2f > &lines )
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

    float eps_th = 0.05f;
    int eps_rho = 10;
    for ( uint i = 0 ; i < lines.size ( ) ; i++ )
    {
        for ( uint j = 0 ; j < lines.size ( ) ; j++ )
        {
            if ( i != j &&
                 abs ( lines.at ( i )[ 0 ] - lines.at ( j )[ 0 ] ) <= eps_rho &&
                 abs ( lines.at ( i )[ 1 ] - lines.at ( j )[ 1 ] ) <= eps_th )
            {
                // FIXME во время видео скорее всего здесь OutOfRange
                lines.erase ( lines.begin ( ) + static_cast < int > ( j ) );
            }
        }
    }

    Mat linesMask ( imageWithLines.size ( ), CV_8UC1, Scalar ( 0, 0, 0 ) );
    linesMask.zeros ( skeletedImage.size ( ), CV_8UC1 );
    draw_lines ( linesMask, lines, false );
    for ( int y = linesMask.rows - 1 ; y != 0; y-- )
    {
        for ( int x = linesMask.cols - 1 ; x != 0; x--)
        {
            if ( skeletedImage.at < uchar > ( y, x ) != linesMask.at < uchar > ( y, x ) )
            {
                linesMask.at < uchar > ( y, x ) = 0;
            }
        }
    }

    // TODO можно найти крайние точки линий, чтобы по ним через line () строить
    // TODO можно попробовать посоединять линии
    // TODO можно попробовать удалить лишние артефакты ( хотя попробуй отличи их )
    for ( int y = imageWithLines.rows - 1 ; y != 0; y-- )
    {
        for ( int x = imageWithLines.cols - 1 ; x != 0; x--)
        {
            if ( linesMask.at < uchar > ( y, x ) )
            {
                imageWithLines.at < Vec3b > ( y, x ) = Vec3b ( 0, 0, 255 );
            }
        }
    }

//    imshow ( "lines_raw", linesMask );
//    waitKey ( 0 );
//    destroyWindow ( "lines_raw" );
}

void lab6_class::draw_lines ( InputOutputArray img, std::vector < cv::Vec2f > lines, bool threeColors )
{
    Mat image;
    image = img.getMat_ ( );

    // Рисуем линии
    float rho = 0;
    float theta = 0;
    Scalar color;
    if ( threeColors )
    {
        color [ 0 ] = 0;
        color [ 1 ] = 0;
        color [ 2 ] = 255;
        color [ 3 ] = 0;
    }
    else
    {
        color [ 0 ] = 255;
        color [ 1 ] = 0;
        color [ 2 ] = 0;
        color [ 3 ] = 0;
    }

    for ( uint i = 0 ; i < lines.size ( ) ; i++ )
    {
        rho = lines.at ( i )[ 0 ];
        theta = lines.at ( i )[ 1 ];
        Point pt1, pt2;
        float a = cos ( theta );
        float b = sin ( theta );
        float x0 = a * rho;
        float y0 = b * rho;

        pt1.x = cvRound ( x0 + 1000 * ( -b ) );
        pt1.y = cvRound ( y0 + 1000 * ( a ) );
        pt2.x = cvRound ( x0 - 1000 * ( -b ) );
        pt2.y = cvRound ( y0 - 1000 * ( a ) );

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
    namedWindow ( windowLinesThreshed, 1 );
    namedWindow ( windowLinesSkeleted, 1 );
}

void lab6_class::destroy_windows_lines ( )
{
    destroyWindow ( windowLinesOriginal );
    destroyWindow ( windowLinesThreshed );
    destroyWindow ( windowLinesSkeleted );
}
