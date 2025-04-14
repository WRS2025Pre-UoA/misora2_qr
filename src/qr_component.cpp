#include "misora2_qr/qr_component.hpp"

namespace component_qr
{
DetectQR::DetectQR(const rclcpp::NodeOptions &options)
    : Node("detect_qr", options)
{
    receive_image_ = this->create_subscription<MyAdaptedType>("qr_image",10,std::bind(&DetectQR::update_image_callback,this,std::placeholders::_1));
    
    decode_data_publisher_ = this->create_publisher<std_msgs::msg::String>("qr_result_data",10);
    result_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("qr_result_image",10);//不要だったらコメントアウト

    // cv::Mat型のreceive_imageを入力としたデコード関数
    // std::string result_data = func(receive_image)
}   

void DetectQR::update_image_callback(const std::unique_ptr<cv::Mat> msg){
    receive_image = std::move(*msg);

    RCLCPP_INFO_STREAM(this->get_logger(),"Receive image address: " << &(msg->data));

    std_msgs::msg::String msg_S;
    msg_S.data = "code";
    decode_data_publisher_->publish(msg_S);
    
}

} //namespace component_qr
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_qr::DetectQR)