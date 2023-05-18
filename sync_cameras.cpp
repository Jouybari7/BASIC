#include <opencv2/opencv.hpp>
#include <chrono>

int main()
{
    int capture_width = 1280;
    int capture_height = 720;
    int display_width = 640;
    int display_height = 480;
    int framerate =120;

    std::string pipeline1 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)" +
                            std::to_string(capture_width) + ", height=(int)" +
                            std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" +
                            std::to_string(framerate) + "/1 ! nvvidconv flip-method=2 ! video/x-raw, width=(int)" +
                            std::to_string(display_width) + ", height=(int)" +
                            std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    std::string pipeline2 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=(int)" +
                            std::to_string(capture_width) + ", height=(int)" +
                            std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" +
                            std::to_string(framerate) + "/1 ! nvvidconv flip-method=2 ! video/x-raw, width=(int)" +
                            std::to_string(display_width) + ", height=(int)" +
                            std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

    cv::VideoCapture cap1(pipeline1, cv::CAP_GSTREAMER);
    cv::VideoCapture cap2(pipeline2, cv::CAP_GSTREAMER);

    if (!cap1.isOpened() || !cap2.isOpened())
    {
        std::cout << "Failed to open cameras." << std::endl;
        return -1;
    }

    cv::namedWindow("Camera 1", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera 1", display_width, display_height);
    cv::namedWindow("Camera 2", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera 2", display_width, display_height);

    cv::Mat frame1, frame2;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;

    while (true)
    {
        start_time = std::chrono::high_resolution_clock::now();

        if (!cap1.read(frame1) || !cap2.read(frame2))
        {
            std::cout << "Capture read error." << std::endl;
            break;
        }

        cv::imshow("Camera 1", frame1);
        cv::imshow("Camera 2", frame2);

        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> latency = end_time - start_time;
        std::cout << "Latency: " << latency.count() << " ms" << std::endl;

        int keycode = cv::waitKey(10) & 0xff;
        if (keycode == 27)
            break;
    }

    cap1.release();
    cap2.release();
    cv::destroyAllWindows();

    return 0;
}
