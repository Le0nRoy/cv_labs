#include "lab6_class.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
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

void lab6_class::find_coins ( vector < Vec3f > &coins )
{
    if ( !coinsLoaded )
    {
        cout << "classify_coins () : Coins are not loaded !" << endl;
        return;
    }
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

float lab6_class::average_saturation ( InputArray src )
{
    Mat img = src.getMat ( );
    if ( img.empty ( ) )
    {
        cout << "average_hue ( ) : empty mat given !" << endl;
        return 0;
    }
    Mat imgHSV;
    Mat imgMask;
    float res = 0;
    cvtColor ( img, imgHSV, CV_BGR2HSV );
    cvtColor ( img, imgMask, CV_BGR2GRAY );
    threshold ( imgMask, imgMask, 20, 255, THRESH_BINARY );

    for ( int y = 10; y < imgHSV.rows - 10; y++ )
    {
        for ( int x = 10; x < imgHSV.cols - 10; x++ )
        {
            if ( imgMask.at < uchar > ( y, x ) != 0 )
            {
                res += imgHSV.at < Vec3b > ( y, x ) [ 1 ];
            }
        }
    }
    res /= imgHSV.rows;
    res /= imgHSV.cols;
    return res;
}

void lab6_class::draw_rounds ( InputOutputArray drawImage, vector < Vec3f > rounds, Scalar color , bool filled )
{
    Mat drawImageMat = drawImage.getMat ( );
    if ( drawImage.empty ( ) )
    {
        cout << "draw_rounds () : empty matrix given !" << endl;
        return;
    }
    if ( rounds.size ( ) == 0 )
    {
        cout << "draw_rounds () : empty vector given !" << endl;
        return;
    }

    int thickness = filled ? -1 : 3;
    int lineType = filled ? FILLED : LINE_8;
    for ( size_t i = 0 ; i < rounds.size ( ) ; i++ )
    {
        Point center ( cvRound ( rounds.at ( i )[ 0 ] ), cvRound ( rounds. at ( i )[ 1 ] ) );
        // draw the circle center
        circle ( drawImageMat, center, 3, color, -1, FILLED, 0 );
        // draw the circle outline
        int radius = cvRound ( rounds.at ( i )[ 2 ] );
        circle ( drawImageMat, center, radius, color, thickness, lineType, 0 );
    }
}

void lab6_class::classify_coins ( vector < Vec3f > circles )
{
    if ( !coinsLoaded )
    {
        cout << "classify_coins () : Coins are not loaded !" << endl;
        return;
    }
    else if (  !templatesLoaded )
    {
        cout << "classify_coins () : Templates are not loaded !" << endl;
        return;
    }

    float nickelSaturation = average_saturation ( templateNickel ); // 17.8
    float brassSaturation = average_saturation ( templateBrass ); // 72.4
    // по-хорошему я должен бы тут еще две переменные ввести ( мин и макс порог )
    // и через сравнение верхних переменных присвоить, но я ленивая жоппка

    try
    {
        for ( size_t i = 0 ; i < circles.size ( ) ; i++ )
        {
            Point center ( cvRound ( circles[ i ][ 0 ] ), cvRound ( circles[ i ][ 1 ] ) );
            int radius = cvRound ( circles.at ( i )[ 2 ] );

            Point roiLeftUp ( center.x - radius, center.y - radius );
            Point roiRightDown ( center.x + radius, center.y + radius );
            Rect roiRect ( roiLeftUp, roiRightDown );

            Mat ROI;
            ROI = coinsImg ( roiRect );

            Scalar color ( 0, 0, 0 );
            Scalar colorBrass ( 0, 0, 255 );
            Scalar colorNickel ( 0, 255, 0 );

            float curSaturation = average_saturation ( ROI );
            if ( curSaturation < brassSaturation )
            {
                color = colorBrass;
            }
            else if ( curSaturation > nickelSaturation )
            {
                color = colorNickel;
            }
            else if ( abs ( brassSaturation - curSaturation ) < abs ( curSaturation - nickelSaturation ) )
            {
                color = colorBrass;
            }
            else
            {
                color = colorNickel;
            }

            vector < Vec3f > drawnCircle ( 1 );
            drawnCircle.at ( 0 ) = circles.at ( i );
            // draw clusters
            draw_rounds ( coinsImg, drawnCircle, color, false );

        }
    }
    catch ( exception &e )
    {
        cout << endl << "MY EXCEPTION"
             << endl << e.what ( ) << endl;
    }
}
