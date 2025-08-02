#include "misora2_qr/detect.hpp"

int main(int argc, char *argv[]) {
     std::string path; 
    if (argc > 1){
       path = argv[1];
    }else{
        std::cout << "Please input image path" << std::endl;
        return 0;
    }
    
    cv::Mat image = cv::imread(path);

    std::string message = DetectFunc::detect_with_zbar(image);

    std::cout << "QRコード検出結果 : " << message << std::endl;
  

    return 0;
}
