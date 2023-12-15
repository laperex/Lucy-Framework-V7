#include "Vision.h"
#include <opencv2/opencv.hpp>

#include <opencv2/opencv.hpp>
#include <iostream>

int mainddd(int, char**) {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    // array to hold image
    cv::Mat frame;
    
    // display the frame until you press a key
    while (1) {
        // capture the next frame from the webcam
        camera >> frame;
        // show the image on the window
        cv::imshow("Webcam", frame);
        // wait (10ms) for esc key to be pressed to stop
        if (cv::waitKey(10) == 27)
            break;
    }
    return 0;
}

void lra::ComputerVision::Initialize() {
	input_camera.open(0);

	// width = input_camera.get(cv::CAP_PROP_FRAME_WIDTH);
	// height = input_camera.get(cv::CAP_PROP_FRAME_HEIGHT);

	enable_live_feed = false;
}

void lra::ComputerVision::Update() {
	// if (enable_live_feed) {
	// 	is_frame_avaliable = input_camera.read(frame);
	// }
}

bool lra::ComputerVision::IsCameraOpen() {
	return input_camera.isOpened();
}

cv::Mat* lra::ComputerVision::GetFrame() {
	if (!input_camera.read(frame))
		return nullptr;
	return &frame;
}

bool lra::ComputerVision::IsFrameAvailable() {
	return is_frame_avaliable;
}