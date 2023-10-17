#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

// Fungsi untuk mendapatkan warna dominan dari gambar menggunakan k-means clustering
cv::Scalar getDominantColor(const cv::Mat &image) {
    // Mereshape gambar menjadi single column
    cv::Mat pixels = image.reshape(1, image.rows * image.cols);
    pixels.convertTo(pixels, CV_32F);

    const int clusterCount = 1;
    cv::Mat labels, centers;
    // Melakukan k-means clustering untuk mendapatkan warna tengah
    cv::kmeans(pixels, clusterCount, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 0.2), 3, cv::KMEANS_PP_CENTERS, centers);
    centers.convertTo(centers, CV_8U);
    return cv::Scalar(centers.at<uchar>(0, 0), centers.at<uchar>(0, 1), centers.at<uchar>(0, 2));
}

// Fungsi untuk mendeteksi tepi menggunakan algoritma Canny
cv::Mat detectEdges(const cv::Mat &image) {
    cv::Mat edges;
    cv::Canny(image, edges, 50, 150);
    return edges;
}

// Fungsi untuk mendeteksi tekstur dengan filter Gabor
cv::Mat detectTexture(const cv::Mat &image) {
    cv::Mat kernel = cv::getGaborKernel(cv::Size(21, 21), 5, 1, 10, 0.5, 0, CV_32F);
    kernel /= cv::sum(kernel)[0];
    cv::Mat texture;
    cv::filter2D(image, texture, CV_32F, kernel);
    cv::normalize(texture, texture, 0, 255, cv::NORM_MINMAX, CV_8U);
    return texture;
}

// Fungsi untuk melakukan segmentasi gambar dengan thresholding adaptif
cv::Mat segmentImage(const cv::Mat &image) {
    cv::Mat segmented;
    cv::adaptiveThreshold(image, segmented, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    return segmented;
}

void processImage(const string& imagePath) {
    // Membaca gambar dari direktori
    cv::Mat img = cv::imread("../samples/1_1.png", cv::IMREAD_COLOR);

    // Cek jika gambar tidak berhasil dibaca
    if(img.empty())
    {
        std::cout << "Could not read the image ! " << std::endl;
        return;
    }

    // Menghitung rasio aspek untuk resize gambar
    double aspectRatio = (double)img.rows / img.cols;
    int newHeight = static_cast<int>(480 * aspectRatio);

    // Melakukan resize gambar
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(480, newHeight));

    // Mengubah gambar yang sudah diresize menjadi grayscale
    cv::Mat grayResizedImg;
    cv::cvtColor(resizedImg, grayResizedImg, cv::COLOR_BGR2GRAY);

    // Mendapatkan warna dominan
    cv::Scalar dominantColor = getDominantColor(resizedImg);
    cout << "Dominant Color (BGR): " << dominantColor << endl;

    // Menampilkan warna dominan pada gambar
    cv::rectangle(resizedImg, cv::Point(0, 0), cv::Point(120, 40), dominantColor, -1);
    string text = to_string((int)dominantColor[0]) + ", " + to_string((int)dominantColor[1]) + ", " + to_string((int)dominantColor[2]);
    cv::putText(resizedImg, text, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // Melakukan deteksi tepi, deteksi tekstur, dan segmentasi
    cv::Mat edges = detectEdges(grayResizedImg);
    cv::Mat texture = detectTexture(grayResizedImg);
    cv::Mat segmented = segmentImage(grayResizedImg);

    // Menampilkan hasil dari tiap fungsi
    cv::imshow("Image Dominant Color", resizedImg);
    cv::imshow("Edge Detection", edges);
    cv::imshow("Texture Detection", texture);
    cv::imshow("Segmentation", segmented);
    cv::waitKey(0);
}

void processVideo() {
    // Membuka webcam
    cv::VideoCapture cap(2, cv::CAP_V4L); // 0 untuk kamera default
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 100);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 100);
    cap.set(cv::CAP_PROP_FPS, 12);  // misalnya, atur ke 15 FPS

    if (!cap.isOpened()) {
        cout << "Error: Could not open camera." << endl;
        return;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // Membaca frame dari webcam

        if (frame.empty()) {
            cout << "Error: Could not read frame." << endl;
            break;
        }

        // Menghitung rasio aspek untuk resize gambar
        double aspectRatio = (double)frame.rows / frame.cols;
        int newHeight = static_cast<int>(480 * aspectRatio);

        // Melakukan resize gambar
        cv::resize(frame, frame, cv::Size(480, newHeight));

        // Mengubah gambar yang sudah diresize menjadi grayscale
        cv::Mat gray;
        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Mendapatkan warna dominan
        cv::Scalar dominantColor = getDominantColor(frame);
        cout << "Dominant Color (BGR): " << dominantColor << endl;

        // Menampilkan warna dominan pada gambar
        rectangle(frame, cv::Point(0, 0), cv::Point(120, 40), dominantColor, -1);
        string text = to_string((int)dominantColor[0]) + ", " + to_string((int)dominantColor[1]) + ", " + to_string((int)dominantColor[2]);
        cv::putText(frame, text, cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

        // Melakukan deteksi tepi, deteksi tekstur, dan segmentasi
        cv::Mat edges = detectEdges(gray);
        cv::Mat texture = detectTexture(gray);
        cv::Mat segmented = segmentImage(gray);

        // Menampilkan hasil dari tiap fungsi
        imshow("Webcam Stream", frame);
        imshow("Edge Detection", edges);
        imshow("Texture Detection", texture);
        imshow("Segmentation", segmented);

        // Tekan tombol 'q' untuk keluar
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    cap.release(); // Menutup kamera
    cv::destroyAllWindows(); // Menutup semua jendela
}

int main(int argc, char** argv)
{
    cout << "Select option:" << endl;
    cout << "1: Process an image" << endl;
    cout << "2: Process video from webcam" << endl;

    int choice;
    cin >> choice;
    string imagePath;

    switch (choice) {
        case 1:
            cout << "Enter image path: ";
            cin >> imagePath;

            processImage(imagePath);
            break;

        case 2:
            processVideo();
            break;

        default:
            cout << "Invalid choice!" << endl;
            break;
    }

    return 0;
}
