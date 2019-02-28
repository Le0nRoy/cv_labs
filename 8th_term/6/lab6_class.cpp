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
    lines = imread ( wayToImgLines, CV_LOAD_IMAGE_COLOR );
    if ( lines.empty ( ) )
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
    skeletezation ( );

    mergeLines ( );
    return true;
}

void lab6_class::skeletezation ( )
{
    if ( !linesLoaded )
    {
        cout << "skeletezation () : Lines are not loaded !" << endl;
        return;
    }

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
    meanShift ( coins, Rect ( meanRadius, meanRadius ),  )
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
