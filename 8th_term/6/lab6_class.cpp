#include "lab6_class.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

lab6_class::lab6_class ( const string wayToImgCoins, const string wayToImgLines )
{
    coins = imread ( wayToImgCoins, CV_LOAD_IMAGE_COLOR );
    if ( coins.empty ( ) )
    {
        cout << "Coins failed to load !" << endl;
        coinsLoaded = false;
    }
    else
    {
        coinsLoaded = true;
    }
    lines = imread ( wayToImgLines, CV_LOAD_IMAGE_COLOR );
    if ( lines.empty ( ) )
    {
        cout << "Lines failed to load !" << endl;
        linesLoaded = false;
    }
    else
    {
        linesLoaded = true;
    }
}

lab6_class::~lab6_class ( )
{
    // take function for c++
    cvDestroyAllWindows ( );
}

void lab6_class::findLines ( )
{
    if ( !linesLoaded )
    {
        cout << "Lines are not loaded !" << endl;
        return;
    }
    skeletezation ( );

    mergeLines ( );
}

void lab6_class::skeletezation ( )
{
    if ( !linesLoaded )
    {
        cout << "Lines are not loaded !" << endl;
        return;
    }

}

void lab6_class::mergeLines ( )
{
    if ( !linesLoaded )
    {
        cout << "Lines are not loaded !" << endl;
        return;
    }
}

void lab6_class::findCoins ( )
{
    if ( !coinsLoaded )
    {
        cout << "Coins are not loaded !" << endl;
        return;
    }
    Mat grayCoins;
    coins.convertTo ( grayCoins, CV_BGR2GRAY );
    // smooth to have less false detections
    GaussianBlur ( grayCoins, grayCoins, Size ( 9, 9 ), 2, 2 );
    vector < Vec3f > circles;
    HoughCircles ( grayCoins, circles, HOUGH_GRADIENT,
                   2, grayCoins.rows / 4, 200, 100 );
    for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
    {
        Point center ( cvRound ( circles[ i ][ 0 ] ), cvRound ( circles[ i ][ 1 ] ) );
        int radius = cvRound ( circles[ i ][ 2 ] );
        // draw the circle center
        circle ( coins, center, 3, Scalar ( 0, 255, 0 ), -1, 8, 0 );
        // draw the circle outline
        circle ( coins, center, radius, Scalar ( 0, 0, 255 ), 3, 8, 0 );
    }
    namedWindow ( "circles", 1 );
    imshow ( "circles", coins );
    materialOfCoins ( );
}

void lab6_class::materialOfCoins ( )
{
    if ( !coinsLoaded )
    {
        cout << "Coins are not loaded !" << endl;
        return;
    }
}
