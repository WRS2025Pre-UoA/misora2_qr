#include "misora2_qr/detect.hpp"

std::string DetectFunc::detect_with_zbar(cv::Mat& image) {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    zbar::Image zbar_img(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
    scanner.scan(zbar_img);

    for (zbar::Image::SymbolIterator symbol = zbar_img.symbol_begin(); symbol != zbar_img.symbol_end(); ++symbol) {
        return symbol->get_data();
    }
    return "";
}
