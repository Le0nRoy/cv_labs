#ifndef CV_LABS_LAB_6
#define CV_LABS_LAB_6

#include "opencv2/core.hpp"
//#include <string>

class lab6_class
{
private:
    cv::Mat coins;
    cv::Mat lines;

    bool coinsLoaded;
    bool linesLoaded;

    void skeletezation ( );
    void mergeLines ( );
    void materialOfCoins ( );

public:
    lab6_class ( const std::string wayToImgCoins, const std::string wayToImgLines );
    ~lab6_class ( );

    void findLines ( );
    void findCoins ( );

};

#endif //TH_TERM_LAB_6