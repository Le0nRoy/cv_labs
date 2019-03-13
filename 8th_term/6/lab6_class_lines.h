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

    bool findCoins ( );

    bool findLines ( );

private:
    cv::Mat coins;
    cv::VideoCapture lines;

    bool coinsLoaded;
    bool linesLoaded;

private:
    /**
     * block for coins finding
     */

    void classifyCoins ( std::vector < cv::Vec3f > circles );
    void coinsHist ( );

private:
    /**
     * block for lines finding
     */

    /**
     * @brief - скелетезация нынешнего кадра
     *   * надо б переделать получение кадров, чтоб можно было одновременно и скелетизированное смотреть и обычное *
     * @param iter
     */
    void skeletezation ( );

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
    void *neighbours ( const cv::_InputArray &img, cv::Point pix, uchar *neighbs );

    /**
     * @brief - подсчет переходов от чёрного к белому "0->1"
     *          *более подробное описание алгоритма*
     * @param neighb - соседи заданного пиксела
     * @return - кол-во переходов
     */
    int trans ( uchar *neighb );

    void mergeLines ( );

};

#endif //TH_TERM_LAB_6