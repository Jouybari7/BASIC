

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
std::mutex frameMutex; // Mutex for frame synchronization


std::string gstreamer_pipeline (int sensor_id,int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc sensor-id="+std::to_string(sensor_id)+" ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}


void captureFrames(cv::VideoCapture& cap, cv::Mat& frame)
{
   
     if (!cap.read(frame))
                {
                    std::cout<<"Capture read error"<<std::endl;
                    // break;  // what to do in case of not capturing
                }
    // std::lock_guard<std::mutex> lock(frameMutex);

}



void concatenate(cv::Mat& frame_L , cv::Mat& frame_R,cv::Mat& camera_images)
{
   
    cv::hconcat(frame_L, frame_R, camera_images);
    // std::lock_guard<std::mutex> lock(frameMutex);

}


// cv::Mat Read (cv::VideoCapture cap)
// {
//     cv::Mat frame;
//     // cap.read(frame);
//         if (!cap.read(frame))
//                 {
//                     std::cout<<"Capture read error"<<std::endl;
//                     // break;  // what to do in case of not capturing
//                 }
//     return frame;
// }

int main()
{
    int capture_width = 640 ;
    int capture_height = 480 ;
    int display_width = 640 ;
    int display_height = 480 ;
    int framerate = 30 ;
    int flip_method = 2 ;
    
     int sensor_id=0;
     std::string pipeline_L = gstreamer_pipeline(sensor_id,capture_width,capture_height,display_width,display_height,framerate,flip_method);
     sensor_id=1;
     std::string pipeline_R = gstreamer_pipeline(sensor_id,capture_width,capture_height,display_width,display_height,framerate,flip_method);

    std::cout << "Using pipeline: \n\t" << pipeline_L << "\n";
    std::cout << "Using pipeline: \n\t" << pipeline_R << "\n";


    cv::VideoCapture cap_L(pipeline_L, cv::CAP_GSTREAMER);
    cv::VideoCapture cap_R(pipeline_R, cv::CAP_GSTREAMER);

    if(!cap_L.isOpened()) {
	std::cout<<"Failed to open lEFT camera."<<std::endl;
	return (-1);
    }

    if(!cap_R.isOpened()) {
	std::cout<<"Failed to open RIGHT camera."<<std::endl;
	return (-1);
    }

            // std::string left_filename = "left_image.jpg";
            // std::string right_filename = "right_image.jpg";

    // cv::namedWindow("image_R", cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("image_L", cv::WINDOW_AUTOSIZE);

    cv::Mat frame_L,frame_R,camera_images;
    cv::Mat res_L,res_R;

    std::cout << "Hit ESC to exit" << "\n" ;

    while(true)
    {
    	 if (!cap_L.read(frame_L) || !cap_R.read(frame_R)) {
		std::cout<<"Capture read error"<<std::endl;
		break;}

    //    cap_L.read(frame_L);
    //    cap_R.read(frame_R);

    //    frame_L=Read(cap_L);
    //    frame_R=Read(cap_R);
    // captureFrames(std::ref(cap_L), std::ref(frame1));
    // captureFrames( std::ref(cap_R), std::ref(frame2));

    // Create two threads for camera capture
    std::thread thread1(captureFrames, std::ref(cap_L), std::ref(frame_L));
    std::thread thread2(captureFrames, std::ref(cap_R), std::ref(frame_R));
    std::thread thread3(concatenate, std::ref(frame_L), std::ref(frame_R),std::ref(camera_images));
        
        




    thread1.join();
    thread2.join();
    thread3.join();


        int key = cv::waitKey(1);
        if (key == 'C' || key == 'c') {
            // Save images from both cameras
        std::string filename = "output.png";
        cv::imwrite(filename, camera_images);
            std::cout << "Image saved." << std::endl;
        }

    
    // cv::hconcat(frame_L, frame_R, camera_images);
    
    // Display the combined image
    cv::imshow("Camera Images", camera_images);

    
    // cv::imshow("image_L",frame1);
	// cv::imshow("image_R",frame2);
    // // Wait for both threads to finish






	int keycode = cv::waitKey(10) & 0xff ; 
        if (keycode == 27) break ;
    }

    cap_L.release();
    cap_R.release();
    cv::destroyAllWindows() ;
    return 0;
}


//g++ -std=c++11 -Wall -I/usr/lib/opencv -I/usr/include/opencv4 simple_camera_2.cpp -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -pthread -o simple_camera_2


//  g++ -std=c++11 -Wall -I/usr/lib/opencv -I/usr/include/opencv4 simple_camera_2.cpp -L/usr/lib -lopencv_core -lopencv_highgui -pthread -lopencv_imgcodecs  -lopencv_videoio -o simple_camera_2
