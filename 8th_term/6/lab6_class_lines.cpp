#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static string windowLinesThreshed = "lines_threshed";
static string windowLinesSkeleted = "lines_skeleted";
static const int threshMaxVal = 255;

int trackbarValueWaitTime = 10;
const int trackbarLimitWaitTime = 1000;

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

    Mat thresh_img;
    lines >> thresh_img;
    if ( thresh_img.empty ( ) )
    {
        cout << "skeletezation () : Frame is not loaded !" << endl;
    }
    thresh_img.convertTo ( thresh_img, CV_8U );
    cvtColor ( thresh_img, thresh_img, CV_BGR2GRAY );
    // can't leave only line on the floor, so need later taking into account
    // only nearest lines ( that are in the bottom )
    int thresh = 70;
    threshold ( thresh_img, thresh_img, thresh, threshMaxVal, THRESH_BINARY );

    namedWindow ( windowLinesThreshed, 1 );
    namedWindow ( windowLinesSkeleted, 1 );
    createTrackbar ( "Time", windowLinesSkeleted, &trackbarValueWaitTime, trackbarLimitWaitTime );
    imshow ( windowLinesThreshed, thresh_img );
    waitKey ( 0 );

    Mat zero_img = Mat::zeros ( thresh_img.rows, thresh_img.cols, thresh_img.type ( ) );

    bool imgChanged = true;
    int i = 0;
    while ( imgChanged )
//    while ( true )
    {
        skeletezation_iter ( thresh_img, thresh_img, 0 );
        imgChanged = skeletezation_iter ( thresh_img, thresh_img, 1 );
        if ( imgChanged )
        {
            cout << i++ << endl;
        }
    }

    waitKey ( 5000 );
    destroyWindow ( windowLinesThreshed );
    destroyWindow ( windowLinesSkeleted );
}

bool lab6_class::skeletezation_iter ( const _InputArray &img, const _OutputArray &skeleted_img, int iter )
{
    Mat thresh_img = img.getMat_ ( );
    if ( thresh_img.type ( ) != CV_8UC1 )
    {
        cout << " skeletezation_iter ( ) : wrong type of matrix ! " << endl;
        return false;
    }
    Mat flagMap = Mat::zeros ( thresh_img.size ( ), CV_8U );

    skeleted_img.create ( thresh_img.size ( ), thresh_img.type ( ) );
    Mat dst = skeleted_img.getMat ( );

    /*******************************************

                    P9  P2  P3

                    P8  P1  P4

                    P7  P6 P5

    *******************************************/

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

    imshow ( windowLinesSkeleted, dst );
    waitKey ( trackbarValueWaitTime );

    return imgChanged;
}

void *lab6_class::neighbours ( const _InputArray &img, Point pix, uchar *neighbs )
{
    Mat image = img.getMat ( );

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

void lab6_class::mergeLines ( )
{
    if ( !linesLoaded )
    {
        cout << "mergeLines () : Lines are not loaded !" << endl;
        return;
    }
    // соединять, офкорс, только сопряженные линии и на выходе наверн иметь массив массивов
}
