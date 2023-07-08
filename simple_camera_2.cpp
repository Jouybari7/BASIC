

#include <opencv2/opencv.hpp>
#include <thread>

std::string gstreamer_pipeline (int sensor_id,int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc sensor-id="+std::to_string(sensor_id)+" ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

cv::Mat Read (cv::VideoCapture cap)
{
    cv::Mat frame;
    // cap.read(frame);
        if (!cap.read(frame))
                {
                    std::cout<<"Capture read error"<<std::endl;
                    // break;  // what to do in case of not capturing
                }
    return frame;
}

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


    cv::namedWindow("image_R", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("image_L", cv::WINDOW_AUTOSIZE);

    cv::Mat frame_L,frame_R;
    cv::Mat res_L,res_R;

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
    	//  if (!cap_L.read(frame_L) || !cap_R.read(frame_R)) {
		// std::cout<<"Capture read error"<<std::endl;
		// break;}

    //    cap_L.read(frame_L);
    //    cap_R.read(frame_R);

       frame_L=Read(cap_L);
       frame_R=Read(cap_R);



    cv::imshow("image_L",frame_L);
	cv::imshow("image_R",frame_R);

	int keycode = cv::waitKey(10) & 0xff ; 
        if (keycode == 27) break ;
    }

    cap_L.release();
    cap_R.release();
    cv::destroyAllWindows() ;
    return 0;
}


//g++ -std=c++11 -Wall -I/usr/lib/opencv -I/usr/include/opencv4 simple_camera_2.cpp -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -pthread -o simple_camera_2
