#include "misora2_qr/qr_component.hpp"
#include "misora2_qr/detect.hpp"
namespace component_qr
{
DetectQR::DetectQR(const rclcpp::NodeOptions &options)
    : Node("detect_qr", options)
{
    receive_image_ = this->create_subscription<MyAdaptedType>("qr_image",10,std::bind(&DetectQR::update_image_callback,this,std::placeholders::_1));
    
    publisher_ = this->create_publisher<misora2_custom_msg::msg::Custom>("qr_results",10);

    // ZBarスキャナの初期化
    scanner_.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
    // cv::Mat型のreceive_imageを入力としたデコード関数
    // std::string result_data = func(receive_image)
}   

void DetectQR::update_image_callback(const std::unique_ptr<cv::Mat> msg){
    cv::Mat receive_image = std::move(*msg);
    std::string decode_contents;
    if (not(receive_image.empty())){
        if (flag == false and receive_image.channels() != 1){// カラー画像である
            // 実装分部-----------------------------------------------------------------
            std::string decode_contents = DetectFunc::detect_with_zbar(receive_image);
            if(decode_contents != ""){
                RCLCPP_INFO_STREAM(this->get_logger(),"Publish: "<< receive_image.size() << " with decode: " << decode_contents );
                flag = true;

                misora2_custom_msg::msg::Custom data;
                data.result = decode_contents;
                data.image = *(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", receive_image).toImageMsg());
                publisher_->publish(data);
                // ---------------------------------------------------
            }   
            // 見つからなかった場合　opencvバージョンを試す
            else{
                cv::Mat processed = DetectFunc::preprocess_for_opencv(receive_image);
                std::string contents_read_opencv = DetectFunc::try_opencv(processed);
                if(contents_read_opencv != ""){
                    RCLCPP_INFO_STREAM(this->get_logger(),"Publish: "<< receive_image.size() << " with decode: " << decode_contents );
                    flag = true;

                    misora2_custom_msg::msg::Custom data;
                    data.result = contents_read_opencv;
                    data.image = *(cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", receive_image).toImageMsg());
                    publisher_->publish(data);
                }
            }
            // それでも見つからない場合　データ送信を行わない
        }
        else if(receive_image.channels() == 1){
            RCLCPP_INFO_STREAM(this->get_logger(),"Failed finding" );
            flag = false;// 1 chanelある画像　黒画像
        }
    }
}

} //namespace component_qr
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_qr::DetectQR)