#include "lab6_class_lines.h"
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
{
    // TODO мб стоит тут удалять все созданные окна
    //  и вообще надо бы сделать вектор с именами окон, а не вот это все
}
