#include "lab6_class.h"
#include <opencv2/core.hpp>
#include "opencv2/core/matx.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

lab6_class::lab6_class ( )
{
}

lab6_class::~lab6_class ( )
{
    // TODO мб стоит тут удалять все созданные окна
    //  и вообще надо бы сделать вектор с именами окон, а не вот это все
}

bool lab6_class::loadLines (const std::string wayToImgLines)
{
    linesVideo.open ( wayToImgLines, CAP_ANY );
    if ( !linesVideo.isOpened ( ) )
    {
        cout << "loadLines () : Lines failed to load !" << endl;
        linesLoaded = false;
    }
    else
    {
        linesLoaded = true;
    }

    return linesLoaded;
}

bool lab6_class::loadCoins (const std::string wayToImgCoins)
{
    coinsImg = imread ( wayToImgCoins, CV_LOAD_IMAGE_COLOR );
    if ( coinsImg.empty ( ) )
    {
        cout << "loadCoins () : Coins failed to load !" << endl;
        coinsLoaded = false;
    }
    else
    {
        coinsLoaded = true;
    }

    return coinsLoaded;
}

bool lab6_class::loadTemplates ( const string wayToNickel, const string wayToBrass )
{
    templateNickel = imread ( wayToNickel, CV_LOAD_IMAGE_COLOR );
    templateBrass = imread ( wayToBrass, CV_LOAD_IMAGE_COLOR );
    if ( templateNickel.empty ( ) || templateBrass.empty ( ) )
    {
        cout << "loadTemplates () : Templates failed to load !" << endl;
        templatesLoaded = false;
    }
    else
    {
        templatesLoaded = true;
    }

    return templatesLoaded;
}
