#ifndef DETECT_HPP
#define DETECT_HPP

#include <opencv2/opencv.hpp>
#include <zbar.h>
namespace DetectFunc
{
    std::string detect_with_zbar(cv::Mat& image);
    cv::Mat preprocess_for_opencv(const cv::Mat& src);
    std::string try_opencv(const cv::Mat& image);
};

#endif