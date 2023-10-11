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

    cv::rectangle(img, cv::Point(0, 0), cv::Point(50, 50), dominantColor, -1);

    string text = to_string((int)dominantColor[0]) + ", " + to_string((int)dominantColor[1]) + ", " + to_string((int)dominantColor[2]);
    cv::putText(img, text, cv::Point(60, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);

    double aspectRatio = (double)img.rows / img.cols;
    int newHeight = static_cast<int>(480 * aspectRatio);
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(480, newHeight));

    cv::imshow("Image Dominant Color", resizedImg);
    cv::waitKey(0);

    return 0;
}