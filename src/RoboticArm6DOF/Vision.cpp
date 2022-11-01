#include "Vision.h"
#include <opencv2/opencv.hpp>

void lra::ComputerVision::Initialize() {
	input_camera.open(1);

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