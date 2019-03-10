#ifndef CV_LABS_LAB_6
#define CV_LABS_LAB_6

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/core/matx.hpp"
//#include <string>

class lab6_class
{
private:
    cv::Mat coins;
    cv::VideoCapture lines;

    bool coinsLoaded;
    bool linesLoaded;

    void skeletezation ( int iter );
    void mergeLines ( );
    void classifyCoins ( std::vector < cv::Vec3f > circles );

public:
    lab6_class ( const std::string wayToImgCoins, const std::string wayToImgLines );
    ~lab6_class ( );

    bool findLines ( );
    bool findCoins ( );

};

#endif //TH_TERM_LAB_6
