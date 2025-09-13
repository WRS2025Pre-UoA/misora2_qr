#include "misora2_qr/detect.hpp"

// ===== ZBarのみ（前処理なし） =====
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

// ===== OpenCV用 前処理 =====
cv::Mat DetectFunc::preprocess_for_opencv(const cv::Mat& src) {
    cv::Mat gray, eq, resized;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, eq);
    cv::resize(eq, resized, cv::Size(), 2.0, 2.0);  // リサイズで歪みに強く
    return resized;
}

// ===== OpenCVのみ（前処理あり） =====
std::string DetectFunc::try_opencv(const cv::Mat& image) {
    cv::QRCodeDetector detector;
    std::string result = detector.detectAndDecode(image);
    return result;
}
