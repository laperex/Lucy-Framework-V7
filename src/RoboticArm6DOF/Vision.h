#pragma once

#include <opencv2/opencv.hpp>

namespace lra {
	struct ComputerVision {
		cv::VideoCapture input_camera;
		cv::Mat frame;

		int width;
		int height;

		bool is_frame_avaliable = false;
		bool enable_live_feed = false;

		void Initialize();
		void Update();

		bool IsCameraOpen();
		bool IsFrameAvailable();

		cv::Mat* GetFrame();

		void Calculate();
	};
}