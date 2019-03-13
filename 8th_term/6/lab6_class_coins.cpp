#include "lab6_class_lines.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static const string windowCoinsMarked = "circles";

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
    namedWindow ( windowCoinsMarked, 1 );
    imshow ( windowCoinsMarked, coins );
    waitKey ( 0 );
    destroyWindow ( windowCoinsMarked );
    return true;
}

void lab6_class::classifyCoins ( vector < Vec3f > circles )
{
    if ( !coinsLoaded )
    {
        cout << "classifyCoins () : Coins are not loaded !" << endl;
        return;
    }
    coinsHist ( );
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

void lab6_class::coinsHist ( )
{
    Mat hsv;
    cvtColor ( coins, hsv, COLOR_BGR2HSV );
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = { hbins, sbins };
    // hue varies from 0 to 179, see cvtColor
    float hranges [ ] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges [ ] = { 0, 256 };
    const float *ranges [ ] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = { 0, 1 };
    calcHist ( &hsv, 1, channels, Mat ( ), // do not use mask
               hist, 2, histSize, ranges,
               true, // the histogram is uniform
               false );
    double maxVal = 0;
    minMaxLoc ( hist, 0, &maxVal, 0, 0 );
    int scale = 10;
    Mat histImg = Mat::zeros ( sbins * scale, hbins * 10, CV_8UC3 );
    for ( int h = 0 ; h < hbins ; h++ )
    {
        for ( int s = 0 ; s < sbins ; s++ )
        {
            float binVal = hist.at < float > ( h, s );
            int intensity = cvRound ( binVal * 255 / maxVal );
            rectangle ( histImg, Point ( h * scale, s * scale ),
                        Point ( ( h + 1 ) * scale - 1, ( s + 1 ) * scale - 1 ),
                        Scalar::all ( intensity ),
                        CV_FILLED );
        }
    }
    namedWindow ( "Source", 1 );
    imshow ( "Source", coins );
    namedWindow ( "H-S Histogram", 1 );
    imshow ( "H-S Histogram", histImg );
    waitKey ( );
}
