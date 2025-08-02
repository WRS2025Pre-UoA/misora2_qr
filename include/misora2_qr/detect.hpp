#ifndef DETECT_HPP
#define DETECT_HPP

#include <opencv2/opencv.hpp>
#include <zbar.h>
namespace DetectFunc
{
    std::string detect_with_zbar(cv::Mat& image);
};

#endif