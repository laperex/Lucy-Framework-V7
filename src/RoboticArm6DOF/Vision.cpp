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

int hmin = 0, smin = 182, vmin = 186;
int hmax = 11, smax = 227, vmax = 255;

void Calculate(const cv::Mat& frame) {
	static cv::Mat hsv, mask;
	std::vector<std::vector<cv::Point>> contours;

	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), mask);
	cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


}

cv::Mat* lra::ComputerVision::GetFrame() {
	if (!input_camera.read(frame))
		return nullptr;
	return &frame;
}

bool lra::ComputerVision::IsFrameAvailable() {
	return is_frame_avaliable;
}