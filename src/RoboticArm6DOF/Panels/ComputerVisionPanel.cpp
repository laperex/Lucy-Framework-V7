#include "Panel.h"
#include <opencv2/opencv.hpp>
#include <Lucy/imgui_lucy_impl.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <LucyGL/LucyGL.h>
#include <RoboticArm6DOF/Vision.h>
#include <Lucy/ECS.h>

static auto& registry = Registry::Instance();

void BindCVMat2GLTexture(cv::Mat& image, unsigned int texture) {
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

	glTexImage2D(GL_TEXTURE_2D,         // Type of texture
						0,                   // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,              // Internal colour format to convert to
						image.cols,          // Image width  i.e. 640 for Kinect in standard mode
						image.rows,          // Image height i.e. 480 for Kinect in standard mode
						0,                   // Border width in pixels (can either be 1 or 0)
			GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,    // Image data type
			image.ptr());        // The actual image data itself
}

lgl::Texture* texture = nullptr;

void lra::panel::ComputerVisionPanel() {
	auto& vision = registry.store<ComputerVision>();

	if (texture == nullptr) {
		texture = new lgl::Texture();
	}

	if (ImGui::Begin("Vision", nullptr, ImGuiWindowFlags_NoTitleBar)) {
		static float slider = 0.17;

		ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnOffset(1, offset);

		{
			ImGui::BeginChild("#0");

			if (ImGui::Button((vision.enable_live_feed) ? "Disable Live Feed": " Enable Live Feed")) {
				vision.enable_live_feed = !vision.enable_live_feed;
			}

			ImGui::EndChild();
		}

		ImGui::NextColumn();

		{
			ImGui::BeginChild("#1");
		
			if (!vision.frame.empty()) {
				cv::cvtColor(vision.frame, vision.frame, cv::COLOR_BGR2RGBA);

				texture->Bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vision.frame.cols, vision.frame.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, vision.frame.data);
				texture->UnBind();

				ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture->id)), ImVec2(vision.frame.cols, vision.frame.rows));
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}
}
