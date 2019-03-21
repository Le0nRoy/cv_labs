#ifndef CV_LABS_LAB_6
#define CV_LABS_LAB_6

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/core/matx.hpp"
//#include <string>

class lab6_class
{
public:
    lab6_class ( const std::string wayToImgCoins, const std::string wayToImgLines );

    ~lab6_class ( );

    bool task_coins ( );

    bool task_lines ( );

    void setPathToTemps ( const std::string nickel, const std::string brass );

    /**
     * @brief - скелетезация всего видео ( ну ооооочень тормозунто )
     */
    bool task_lines_video ( );

private:
    cv::Mat coinsImg;
    cv::VideoCapture linesVideo;

    bool coinsLoaded;
    bool linesLoaded;

private:
    /**
     * block for coins finding
     */

    std::string tempNickel;
    std::string tempBrass;

    void find_coins ( std::vector < cv::Vec3f > coins );

    void coins_hist ( cv::OutputArray histogram );

    void classify_coins ( std::vector < cv::Vec3f > circles );

private:
    /**
     * block for lines finding
     */

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
    void neighbours ( const cv::_InputArray &img, cv::Point pix, uchar *neighbs );

    /**
     * @brief - подсчет переходов от чёрного к белому "0->1"
     *          *более подробное описание алгоритма*
     * @param neighb - соседи заданного пиксела
     * @return - кол-во переходов
     */
    int trans ( uchar *neighb );

    void find_lines ( cv::InputArray skel_img, std::vector < cv::Vec2f > &lines );

    void merge_lines ( std::vector < cv::Vec2f > &lines );

    void draw_lines ( cv::InputOutputArray img, std::vector < cv::Vec2f > lines );

    void make_windows_lines ( );

    void destroy_windows_lines ( );

};

#endif //TH_TERM_LAB_6
