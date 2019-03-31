#ifndef LAB5_CLASS_H
#define LAB5_CLASS_H

#include <string>
#include <opencv2/videoio.hpp>

class lab5_class
{
public:
    lab5_class();

    void loadVideo ( std::string pathToVideo );

    void run_lab ( );

private:
    bool videoIsLoaded;
    cv::VideoCapture video;

    void threshold_lighting ( cv::InputArray src, cv::OutputArray threshed_img );

    void draw_map ( cv::OutputArray map, int pix_per_sm );

    void draw_on_map ( cv::InputArray map, cv::OutputArray map_with_objects ,
                       std::vector < int > x_points,
                       std::vector < int > distances );

};

#endif // LAB5_CLASS_H
