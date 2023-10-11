#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

cv::Scalar getDominantColor(const cv::Mat &image) {
    cv::Mat pixels = image.reshape(1, image.rows * image.cols);
    pixels.convertTo(pixels, CV_32F);

    const int clusterCount = 1;
    cv::Mat labels, centers;
    cv::kmeans(pixels, clusterCount, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 0.2), 3, cv::KMEANS_PP_CENTERS, centers);
    centers.convertTo(centers, CV_8U);
    return cv::Scalar(centers.at<uchar>(0, 0), centers.at<uchar>(0, 1), centers.at<uchar>(0, 2));
}

cv::Mat detectEdges(const cv::Mat &image) {
    cv::Mat edges;
    cv::Canny(image, edges, 50, 150);
    return edges;
}

cv::Mat detectTexture(const cv::Mat &image) {
    cv::Mat kernel = cv::getGaborKernel(cv::Size(21, 21), 5, 1, 10, 0.5, 0, CV_32F);
    kernel /= cv::sum(kernel)[0];
    cv::Mat texture;
    cv::filter2D(image, texture, CV_32F, kernel);
    cv::normalize(texture, texture, 0, 255, cv::NORM_MINMAX, CV_8U);
    return texture;
}

cv::Mat segmentImage(const cv::Mat &image) {
    cv::Mat segmented;
    cv::adaptiveThreshold(image, segmented, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    return segmented;
}

int main(int argc, char** argv)
{
    cv::Mat img = cv::imread("../samples/2.jpg", cv::IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image ! " << std::endl;
        return 1;
    }

    double aspectRatio = (double)img.rows / img.cols;
    int newHeight = static_cast<int>(480 * aspectRatio);
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(480, newHeight));
    cv::Mat grayResizedImg;
    cv::cvtColor(resizedImg, grayResizedImg, cv::COLOR_BGR2GRAY);


    cv::Scalar dominantColor = getDominantColor(img);
    cout << "Dominant Colot (BGR): " << dominantColor << endl;
    cv::rectangle(img, cv::Point(0, 0), cv::Point(1200, 400), dominantColor, -1);
    string text = to_string((int)dominantColor[0]) + ", " + to_string((int)dominantColor[1]) + ", " + to_string((int)dominantColor[2]);
    cv::putText(img, text, cv::Point(50, 220), cv::FONT_HERSHEY_COMPLEX, 4, cv::Scalar(255, 255, 255), 2);

    cv::Mat edges = detectEdges(grayResizedImg);
    cv::Mat texture = detectTexture(grayResizedImg);
    cv::Mat segmented = segmentImage(grayResizedImg);

    cv::imshow("Image Dominant Color", resizedImg);
    cv::imshow("Edge Detection", edges);
    cv::imshow("Texture Detection", texture);
    cv::imshow("Segmentation", segmented);
    cv::waitKey(0);

    return 0;
}