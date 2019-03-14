#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static const string windowCoinsMarked = "circles";

bool lab6_class::task_coins ( )
{
    if ( !coinsLoaded )
    {
        cout << "classify_coins () : Coins are not loaded !" << endl;
        return false;
    }
    vector < Vec3f > circles;
    find_coins ( circles );
    classify_coins ( circles );

    // show result of functions
    namedWindow ( windowCoinsMarked, 1 );
    imshow ( windowCoinsMarked, coinsImg );
    waitKey ( 0 );
    destroyWindow ( windowCoinsMarked );
    return true;
}

void lab6_class::find_coins ( vector < Vec3f > coins )
{
    Mat grayCoins;
    // HoughCircles work only with 8U GrayScale image
    coinsImg.convertTo ( grayCoins, CV_8U );
    cvtColor ( coinsImg, grayCoins, CV_BGR2GRAY );
    // smooth to have less false detections
    GaussianBlur ( grayCoins, grayCoins, Size ( 9, 9 ), 2, 2 );
    // find circles on picture
    HoughCircles ( grayCoins, coins, HOUGH_GRADIENT,
                   2, grayCoins.rows / 4, 200, 100 );
}

void lab6_class::coins_hist ( OutputArray histogram )
{
    Mat hsv;
    cvtColor ( coinsImg, hsv, COLOR_BGR2HSV );
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30;
    int sbins = 32;
    int histSize[] = { hbins, sbins };
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float *ranges[] = { hranges, sranges };

    MatND hist;
    histogram.create ( hist.size ( ), hist.type ( ) );
    hist = histogram.getMat_ ( );
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = { 0, 1 };
    calcHist ( &hsv, 1, channels, Mat ( ), // do not use mask
               hist, 2, histSize, ranges,
               true, // the histogram is uniform
               false );
}

void lab6_class::classify_coins ( vector < Vec3f > circles )
{
    if ( !coinsLoaded )
    {
        cout << "classify_coins () : Coins are not loaded !" << endl;
        return;
    }
    MatND hist;
    coins_hist ( hist );
    // use meanshift classificator here
    // for window size use median radius
    vector < int > radius ( circles.size ( ) );
    int meanRadius = 0;
    for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
    {
        radius.at ( i ) = cvRound ( circles[ i ][ 2 ] );
        meanRadius += radius.at ( i ) / circles.size ( );
    }
//    meanShift ( coinsImg, Rect ( meanRadius, meanRadius ),  )
    // draw
    for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
    {
        Point center ( cvRound ( circles[ i ][ 0 ] ), cvRound ( circles[ i ][ 1 ] ) );
        // draw the circle center
        circle ( coinsImg, center, 3, Scalar ( 0, 0, 255 ), -1, 8, 0 );
        // draw the circle outline
//        circle ( coinsImg, center, radius.at ( i ), Scalar ( 0, 0, 0 ), 1, 8, 0 );
    }
}
