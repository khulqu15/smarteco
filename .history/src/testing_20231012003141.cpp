#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

cv::Scalar getDominantColor(const cv::Mat &image) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    
    vector<cv::Mat> hsv_planes;
    cv::split(hsv, hsv_planes);
    cv::Mat hue = hsv_planes[0];

    int histSize = 180;
    float hue_range[] = {0, 180};
    const float* ranges = {hue_range};

    cv::Mat histogram;
    cv::calcHist(&hue, 1, 0, cv::Mat(), histogram, 1, &histSize, &ranges, true, false);

    double maxVal = 0;
    int dominantValue = 0;
    for(int i=0; i<histSize; i++) {
        float binVal = histogram.at<float>(i);
        if(binVal > maxVal) {
            maxVal = binVal;
            dominantValue = i;
        }
    }

    cv::Mat dominantColorHSV(1, 1, CV_8UC3, cv::Scalar(dominantValue, 255, 255));
    cv::Mat dominantColorBGR;
    cv::cvtColor(dominantColorHSV, dominantColorBGR, cv::COLOR_HSV2BGR);
    cv::Scalar dominantColor = dominantColorBGR.at<cv::Vec3b>(0, 0);
    
    return dominantColor;
}

int main(int argc, char** argv)
{
    cv::Mat img = cv::imread("../samples/3.jpg", cv::IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image ! " << std::endl;
        return 1;
    }

    cv::Scalar dominantColor = getDominantColor(img);
    cout << "Dominant Colot (BGR): " << dominantColor << endl;

    return 0;
}