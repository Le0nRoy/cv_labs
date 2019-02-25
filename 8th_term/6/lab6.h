#ifndef CV_LABS_LAB_6
#define CV_LABS_LAB_6

#include "opencv2/core.hpp"

class lab6
{
private:
    cv::Mat image;

public:
    void skeletezation ( );
    void findLines ( );
    // take a look if there are some ready functions for this
    void mergeLines ( );
    void findCoins ( );
    void materialOfCoins ( );

};

#endif //TH_TERM_LAB_6