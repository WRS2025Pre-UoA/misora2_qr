#ifndef QR_COMPONENT_HPP
#define QR_COMPONENT_HPP

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <functional>
#include <algorithm>

#include <rclcpp/clock.hpp>
#include <rclcpp/time.hpp>

#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/string.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <rclcpp/type_adapter.hpp>

#include "misora2_qr/cv_mat_type_adapter.hpp"

using namespace std::chrono_literals;

namespace component_qr
{
class DetectQR : public rclcpp::Node
{
public:
    using MyAdaptedType = rclcpp::TypeAdapter<cv::Mat, sensor_msgs::msg::Image>;

    std::string decode_contents;
    cv::Mat result_image, receive_image;

    explicit DetectQR(const rclcpp::NodeOptions &options);
    DetectQR() : DetectQR(rclcpp::NodeOptions{}) {}

private:
    void update_image_callback(const std::unique_ptr<cv::Mat> msg);

    rclcpp::Subscription<MyAdaptedType>::SharedPtr receive_image_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr decode_data_publisher_;
    rclcpp::Publisher<MyAdaptedType>::SharedPtr result_image_publisher_;
};

} // namespace component_qr

#endif // QR_COMPONENT_HPP