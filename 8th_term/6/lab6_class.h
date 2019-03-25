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

    bool task_lines ( );

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

    /**
     * @brief - скелетезация нынешнего кадра
     *   * надо б переделать получение кадров, чтоб можно было одновременно и скелетизированное смотреть и обычное *
     */
    void skeletezation ( const cv::_InputArray &src, const cv::_OutputArray &skeleted_img );

    /**
     * @brief - полная итерация скелитизации по алгоритму Шанг-Суня
     * @param iter - номер шага
     */
    bool skeletezation_iter ( cv::InputArray img, cv::OutputArray skeleted_img, int iter );

    /**
     * @brief - поиск соседей заданного пиксела
     * @param img
     * @param pix
     * @return - массив соседей пиксела в правильном порядке ( P2, P3 .. P9 )
     */
    void neighbours ( cv::InputArray img, cv::Point pix, uchar *neighbs );

    /**
     * @brief - подсчет переходов от чёрного к белому "0->1"
     *          *более подробное описание алгоритма*
     * @param neighb - соседи заданного пиксела
     * @return - кол-во переходов
     */
    int trans ( uchar *neighb );

    void find_lines ( cv::InputArray skel_img, std::vector < cv::Vec2f > &lines );

    void merge_lines ( cv::InputArray skel_img, cv::InputOutputArray drawImage, std::vector < cv::Vec2f > &lines );

    void draw_lines ( cv::InputOutputArray img, std::vector < cv::Vec2f > lines, bool threeColors );

    void make_windows_lines ( );

    void destroy_windows_lines ( );

};

#endif //TH_TERM_LAB_6
