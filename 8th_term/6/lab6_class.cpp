#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

lab6_class::lab6_class ( const string wayToImgCoins, const string wayToImgLines )
{
    coins = imread ( wayToImgCoins, CV_LOAD_IMAGE_COLOR );
    if ( coins.empty ( ) )
    {
        cout << "lab6_class () : Coins failed to load !" << endl;
        coinsLoaded = false;
    }
    else
    {
        coinsLoaded = true;
    }
    lines.open ( wayToImgLines, CAP_ANY );
    if ( !lines.isOpened ( ) )
    {
        cout << "lab6_class () : Lines failed to load !" << endl;
        linesLoaded = false;
    }
    else
    {
        linesLoaded = true;
    }
}

lab6_class::~lab6_class ( )
{ }

bool lab6_class::findLines ( )
{
    if ( !linesLoaded )
    {
        cout << "findLines () : Lines are not loaded !" << endl;
        return false;
    }
    skeletezation ( 0 );

    mergeLines ( );
    return true;
}

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * \param  im    Binary image with range = 0-1
 * \param  iter  0=even, 1=odd
 */
void lab6_class::skeletezation ( int iter )
{
    if ( !linesLoaded )
    {
        cout << "skeletezation () : Lines are not loaded !" << endl;
        return;
    }

    Mat thresh_img;
    lines >> thresh_img;
    if ( thresh_img.empty ( ) )
    {
        cout << "skeletezation () : Frame is not loaded !" << endl;
    }
    thresh_img.convertTo ( thresh_img, CV_8U );
    cvtColor ( thresh_img, thresh_img, CV_BGR2GRAY );
    int thresh = 100;
    threshold ( thresh_img, thresh_img, thresh, 255, THRESH_BINARY );

    Mat marker = Mat::zeros ( thresh_img.size ( ), CV_8U );
//    threshold ( thresh_img, thresh_img, thresh, 255, THRESH_OTSU );
//    adaptiveThreshold ( thresh_img, thresh_img, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 1 );
    string windowName = "lines_threshed";
    namedWindow ( windowName, 1 );
    imshow ( windowName, thresh_img );
    waitKey ( 0 );
    destroyWindow ( windowName );

//    for ( int i = 1 ; i < lines.rows - 1 ; i++ )
//    {
//        for ( int j = 1 ; j < lines.cols - 1 ; j++ )
//        {
//            uchar p2 = lines.at < uchar > ( i - 1, j );
//            uchar p3 = lines.at < uchar > ( i - 1, j + 1 );
//            uchar p4 = lines.at < uchar > ( i, j + 1 );
//            uchar p5 = lines.at < uchar > ( i + 1, j + 1 );
//            uchar p6 = lines.at < uchar > ( i + 1, j );
//            uchar p7 = lines.at < uchar > ( i + 1, j - 1 );
//            uchar p8 = lines.at < uchar > ( i, j - 1 );
//            uchar p9 = lines.at < uchar > ( i - 1, j - 1 );
//
//            int A = ( p2 == 0 && p3 == 1 ) + ( p3 == 0 && p4 == 1 ) +
//                    ( p4 == 0 && p5 == 1 ) + ( p5 == 0 && p6 == 1 ) +
//                    ( p6 == 0 && p7 == 1 ) + ( p7 == 0 && p8 == 1 ) +
//                    ( p8 == 0 && p9 == 1 ) + ( p9 == 0 && p2 == 1 );
//            int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
//            int m1 = iter == 0 ? ( p2 * p4 * p6 ) : ( p2 * p4 * p8 );
//            int m2 = iter == 0 ? ( p4 * p6 * p8 ) : ( p2 * p6 * p8 );
//
//            if ( A == 1 && ( B >= 2 && B <= 6 ) && m1 == 0 && m2 == 0 )
//            {
//                marker.at < uchar > ( i, j ) = 1;
//            }
//        }
//    }
//
//    lines &= ~marker;

}

void lab6_class::mergeLines ( )
{
    if ( !linesLoaded )
    {
        cout << "mergeLines () : Lines are not loaded !" << endl;
        return;
    }
}

bool lab6_class::findCoins ( )
{
    if ( !coinsLoaded )
    {
        cout << "classifyCoins () : Coins are not loaded !" << endl;
        return false;
    }
    Mat grayCoins;
    // HoughCircles work only with 8U GrayScale image
    coins.convertTo ( grayCoins, CV_8U );
    cvtColor ( coins, grayCoins, CV_BGR2GRAY );
    // smooth to have less false detections
    GaussianBlur ( grayCoins, grayCoins, Size ( 9, 9 ), 2, 2 );
    // find circles on picture
    vector < Vec3f > circles;
    HoughCircles ( grayCoins, circles, HOUGH_GRADIENT,
                   2, grayCoins.rows / 4, 200, 100 );
    classifyCoins ( circles );
    // show result of functions
    string windowName = "circles";
    namedWindow ( windowName, 1 );
    imshow ( windowName, coins );
    waitKey ( 0 );
    destroyWindow ( windowName );
    return true;
}

void lab6_class::classifyCoins ( vector < Vec3f > circles )
{
    if ( !coinsLoaded )
    {
        cout << "classifyCoins () : Coins are not loaded !" << endl;
        return;
    }
    // use meanshift classificator here
    // for window size use median radius
    vector < int > radius ( circles.size ( ) );
    int meanRadius = 0;
    for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
    {
        radius.at ( i ) = cvRound ( circles[ i ][ 2 ] );
        meanRadius += radius.at ( i ) / circles.size ( );
    }
//    meanShift ( coins, Rect ( meanRadius, meanRadius ),  )
    // draw
    for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
    {
        Point center ( cvRound ( circles[ i ][ 0 ] ), cvRound ( circles[ i ][ 1 ] ) );
        // draw the circle center
        circle ( coins, center, 3, Scalar ( 0, 0, 255 ), -1, 8, 0 );
        // draw the circle outline
//        circle ( coins, center, radius.at ( i ), Scalar ( 0, 0, 0 ), 1, 8, 0 );
    }
}
