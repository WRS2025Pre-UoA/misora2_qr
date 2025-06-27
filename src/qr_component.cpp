#include "misora2_qr/qr_component.hpp"

namespace component_qr
{
DetectQR::DetectQR(const rclcpp::NodeOptions &options)
    : Node("detect_qr", options)
{
    receive_image_ = this->create_subscription<MyAdaptedType>("qr_image",10,std::bind(&DetectQR::update_image_callback,this,std::placeholders::_1));
    
    decode_data_publisher_ = this->create_publisher<std_msgs::msg::String>("qr_result_data",10);
    result_image_publisher_ = this->create_publisher<MyAdaptedType>("qr_result_image",10);//不要だったらコメントアウト

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
            // グレースケールへ変換
            cv::Mat gray;
            cv::cvtColor(receive_image, gray, cv::COLOR_BGR2GRAY);

            // ZBar形式に変換してスキャン
            zbar::Image zbarImage(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
            int n = scanner_.scan(zbarImage); // 発見数

            if (n > 0) { // 少なくとも一つ見つかったとき
                for (auto symbol = zbarImage.symbol_begin(); symbol != zbarImage.symbol_end(); ++symbol) {
                    decode_contents = symbol->get_data();
                    RCLCPP_INFO_STREAM(this->get_logger(), "Success QR decode! Message: " << decode_contents);
                    break;
                }
            
                // テスト用出力-------------------------------------------
                std_msgs::msg::String msg_S;
                msg_S.data = decode_contents;
                decode_data_publisher_->publish(msg_S);
                result_image_publisher_->publish(receive_image);
                RCLCPP_INFO_STREAM(this->get_logger(),"Publish: "<< receive_image.size() );
                flag = true;
                // ---------------------------------------------------
            }
            // 見つからなかった場合　データ送信を行わない
        }
        else if(receive_image.channels() == 1) flag = false;// 1 chanelある画像　黒画像
    }
}

} //namespace component_qr
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_qr::DetectQR)