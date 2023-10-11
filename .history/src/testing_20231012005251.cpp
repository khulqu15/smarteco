#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

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