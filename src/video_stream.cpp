#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Membuka video capture dengan device index 0 (biasanya kamera internal laptop/PC)
    VideoCapture cap(2);

    // Cek jika video capture tidak berhasil dibuka
    if (!cap.isOpened()) {
        cout << "Error: Could not open the webcam." << endl;
        return -1;
    }

    namedWindow("Webcam Stream", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;

        // Membaca frame dari webcam
        cap >> frame;

        // Jika frame kosong, berarti stream selesai
        if (frame.empty()) break;

        // Menampilkan frame
        imshow("Webcam Stream", frame);

        // Keluar dari loop jika user menekan tombol 'q'
        if (waitKey(1) == 'q') break;
    }

    // Menutup semua jendela yang terbuka
    destroyAllWindows();
    return 0;
}
