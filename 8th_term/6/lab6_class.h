#ifndef CV_LABS_LAB_6
#define CV_LABS_LAB_6

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/core/matx.hpp"
//#include <string>

class lab6_class
{
public:
    lab6_class ( );

    ~lab6_class ( );

    bool task_coins ( );

    int lines_lab();

    bool loadLines ( const std::string wayToImgLines );

    bool loadCoins ( const std::string wayToImgCoins );

    bool loadTemplates ( const std::string wayToNickel, const std::string wayToBrass );

    /**
     * @brief - скелетезация всего видео ( ну ооооочень тормозунто )
     */
    bool task_lines_video ( );

private:
    /**
     * block for coins finding
     */
    cv::Mat coinsImg;
    cv::Mat templateNickel;
    cv::Mat templateBrass;

    bool coinsLoaded;
    bool templatesLoaded;

    void find_coins ( std::vector < cv::Vec3f > &coins );

    float average_saturation ( cv::InputArray src );

    void draw_rounds ( cv::InputOutputArray drawImage, std::vector < cv::Vec3f > rounds, cv::Scalar color , bool filled );

    void classify_coins ( std::vector < cv::Vec3f > circles );

private:
    /**
     * block for lines finding
     */
    cv::VideoCapture linesVideo;

    bool linesLoaded;

};

#endif //TH_TERM_LAB_6
