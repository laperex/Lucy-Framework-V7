#include "Panel.h"
#include <Lucy/imgui_lucy_impl.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <LucyGL/LucyGL.h>
#include <RoboticArm6DOF/Vision.h>
#include <RoboticArm6DOF/Controller.h>
#include <Lucy/ECS.h>
#include <glm/glm.hpp>
#include <map>
#include <unordered_map>
#include <LucyUtil/FileIO.h>

#define ENABLE_LOADSAVE false

static std::pair<bool, glm::vec2> DetectPosition(const cv::Mat& frame) {
	cv::Mat hsv, mask;
	static int hmin = 0, smin = 132, vmin = 156;
	static int hmax = 11, smax = 255, vmax = 199;
	static std::vector<std::vector<cv::Point>> contours;
	static std::vector<cv::Point> approx;

	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), mask);
	cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// static cv::Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0,0,0));

	if (contours.size()) {
		int max_contour_idx = -1;
		for (size_t idx = 0; idx < contours.size(); idx++) {
			// cv::drawContours(contourImage, contours, idx, cv::Scalar(0, 0, 255));
			auto area = cv::contourArea(contours[idx]);
			if (area > 100) {
				if (max_contour_idx < 0) {
					max_contour_idx = idx;
				} else if (area >= cv::contourArea(contours[max_contour_idx])) {
					max_contour_idx = idx;
				}
			}
		}

		if (max_contour_idx >= 0) {
			// std::cout << cv::contourArea(contours[max_contour_idx]) << '\n';

			cv::approxPolyDP(contours[max_contour_idx], approx, 0.01 * cv::arcLength(contours[max_contour_idx], true), true);
			auto rect = cv::boundingRect(approx);
			return { true, { rect.x + rect.width, rect.y + rect.height }};
			// cv::rectangle(frame, rect, (0, 255, 0), 4);
			// auto M = cv::moments(contours[max_contour_idx]);
			// if (M.m00 != 0) {
			// 	auto cx = int(M.m10 / M.m00);
			// 	auto cy = int(M.m01 / M.m00);
			// 	// cv::circle(frame, { cx, cy }, 3, (255, 0, 0), -1);
			// }
		}
	}

	return { false, { 0, 0 }};
}

static void MatToTexture(lgl::Texture* texture, cv::Mat& frame) {
	assert(texture != nullptr);

	cv::cvtColor(frame, frame, cv::COLOR_BGR2RGBA);

	static std::unordered_map<lgl::Texture*, glm::vec2> last_size;

	if (last_size.find(texture) == last_size.end()) {
		last_size[texture] = { 0, 0 };
	}

	texture->Bind();
	texture->SetFilteringMode(lgl::FilterMode_LINEAR, lgl::FilterMode_LINEAR);
	texture->SetWrapMode(lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	if (last_size[texture].x != frame.cols || last_size[texture].y != frame.rows) {
		texture->Load2D(0, lgl::RGBA, frame.cols, frame.rows, 0, lgl::RGBA, lgl::UNSIGNED_BYTE, frame.data);
		last_size[texture].x = frame.cols;
		last_size[texture].y = frame.rows;
	} else {
		texture->LoadSub2D(0, 0, 0, last_size[texture].x, last_size[texture].y, lgl::RGBA, lgl::UNSIGNED_BYTE, frame.data);
	}
	texture->UnBind();
}


enum ViewMode {
	RawView,
	WrappedView,

	ViewMode_COUNT
};

static int hmin = 0, smin = 132, vmin = 156;
static int hmax = 11, smax = 255, vmax = 199;

struct DetectColor {
	glm::ivec3 min, max;
	std::string name;

	DetectColor() {}
	DetectColor(glm::ivec3 min, glm::ivec3 max, std::string name): min(min), max(max), name(name) {}
};

struct ColorDetectionData {
	std::vector<DetectColor> available_colors = {
		{
			{ 0, 132, 156 },
			{ 11, 255, 199 },
			"Red"
		}
	};

	std::array<glm::vec2, 4> wrap_points_normals = {
		glm::vec2(0.122, 0.390),
		glm::vec2(0.170, 0.765),
		glm::vec2(0.731, 0.314),
		glm::vec2(0.745, 0.769),
	};

	std::array<glm::vec2, 4> wrap_points_ik_positions = {
		glm::vec2(0, 0),
		glm::vec2(0, 320),
		glm::vec2(640, 0),
		glm::vec2(640, 320),
	};

	ViewMode view_mode = RawView;

	bool show_mask = false;
	bool initialize = true;

	cv::VideoCapture capture;

	cv::Mat* selected_mat = nullptr;

	bool is_view_window_hovered = false;
	bool is_view_window_clicked = false;
	bool is_view_window_clicked_left = false;
	bool is_view_window_clicked_right = false;

	glm::vec2 view_window_mouse_pos_normalized = { 0, 0 };
	glm::ivec2 view_window_size = { 0, 0 };
	glm::ivec2 view_window_pos = { 0, 0 };
};

void lra::panel::ColorDetectionPanel() {
	static ColorDetectionData color_detection_data;

	if (ImGui::Begin("ColorDetectionPanel")) {
		if (ENABLE_LOADSAVE) {
			if (color_detection_data.initialize) {
				auto [data, size] = util::read_bytes_from_file("ColorDetectionData.bin");
				if (size == sizeof(color_detection_data)) {
					color_detection_data = *decltype(&color_detection_data)(data);
				}
			}
			if (lucy::Events::IsQuittable()) {
				util::write_bytes_to_file("ColorDetectionData.bin", (uint8_t*)&color_detection_data, sizeof(color_detection_data));
			}
		}

		if (color_detection_data.initialize) {
			color_detection_data.capture.open(0);
		}

		bool load_frame = false;
		static float slider = 0.17;
		ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;

		{
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnOffset(1, offset);

			if (ImGui::BeginChild("##Options") || true) {
				if (ImGui::Button("Next View", { ImGui::GetColumnWidth(), 0 })) {
					color_detection_data.view_mode = (ViewMode)(1 + (uint32_t)color_detection_data.view_mode);

					if (color_detection_data.view_mode >= ViewMode_COUNT) {
						color_detection_data.view_mode = (ViewMode)0;
					}
				}

				if (ImGui::Button("ShowMask", { ImGui::GetColumnWidth(), 0 })) {
					color_detection_data.show_mask = !color_detection_data.show_mask;
				}

				static bool live_feed = false;
				if (ImGui::Button((live_feed) ? "Disable LiveFeed": " Enable LiveFeed", { ImGui::GetColumnWidth(), 0 })) {
					live_feed = !live_feed;
				}

				if (!live_feed) {
					if (ImGui::Button("Take Snapshot", { ImGui::GetColumnWidth(), 0 })) {
						load_frame = true;
					}
				}

				if (live_feed) {
					load_frame = true;
				}

				static bool color_menu = false;
				if (ImGui::Button("Show Color Menu")) {
					color_menu = !color_menu;
				}

				static bool wrap_points_menu = false;
				if (ImGui::Button("Show Wrap Points")) {
					wrap_points_menu = !wrap_points_menu;
				}

				if (ImGui::Begin("ColorMenu", &color_menu)) {
					if (ImGui::BeginTable("##ColorMenuTable", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Name");

						ImGui::TableSetColumnIndex(1);
						ImGui::Text("Min");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("Max");

						int idx = 0;
						for (int i = 0; i < color_detection_data.available_colors.size(); i++) {
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);

							static bool next_text_mode = false;
							if (next_text_mode) {
								ImGui::SanitisedInputText("##SelectedInputText", color_detection_data.available_colors[i].name);
							} else {
								ImGui::Selectable((color_detection_data.available_colors[i].name + "##" + std::to_string(idx)).c_str());
							}

							bool is_hovered = ImGui::IsItemHovered();

							if (is_hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
								next_text_mode = true;
							}
							if (!is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
								next_text_mode = false;
							}

							glm::vec3 temp;

							temp = ((glm::vec3)color_detection_data.available_colors[i].min) / 256.0f;
							ImGui::TableSetColumnIndex(1);
							ImGui::ColorEdit3(("##min" + std::to_string(idx)).c_str(), &temp[0], { ImGui::GetColumnWidth(), 0 });
							color_detection_data.available_colors[i].min = (glm::ivec3)(temp * 256.0f);

							temp = ((glm::vec3)color_detection_data.available_colors[i].max) / 256.0f;
							ImGui::TableSetColumnIndex(2);
							ImGui::ColorEdit3(("##max" + std::to_string(idx)).c_str(), &temp[0], { ImGui::GetColumnWidth(), 0 });
							color_detection_data.available_colors[i].max = (glm::ivec3)(temp * 256.0f);

							idx++;
						}

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
							color_detection_data.available_colors.push_back({});
						}

						ImGui::EndTable();
					}
				}
				ImGui::End();
			}
			ImGui::EndChild();

			// Calculations
			cv::Mat frame, wrap_frame;
			color_detection_data.selected_mat = nullptr;
			if (load_frame) {
				if (color_detection_data.capture.read(frame)) {
					auto temp = color_detection_data.wrap_points_normals;

					for (int i = 0; i < 4; i++) {
						temp[i].x = (float)frame.cols * temp[i].x;
						temp[i].y = (float)frame.rows * temp[i].y;
					}

					auto M = cv::getPerspectiveTransform((cv::Point2f*)temp.data(), (cv::Point2f*)color_detection_data.wrap_points_ik_positions.data());
					cv::warpPerspective(frame, wrap_frame, M, cv::Size(frame.cols, frame.rows));
					// auto [is_frame, point] = DetectPosition(wrap_frame);
					// if (is_frame) {
					// 	cv::circle(wrap_frame, *(cv::Point2f*)&point, 5, cv::Scalar(0, 255, 255), 3);

					// 	// norm_wrap_sel_pos.x = (point.x / (float)wrap_frame.cols);
					// 	// norm_wrap_sel_pos.y = (point.y / (float)wrap_frame.rows);
					// 	// target.y = 100;
					// }

					// static std::vector<std::vector<cv::Point>> contours;
					// cv::Mat hsv, mask;
					// for (auto& color_detect: color_detection_data.available_colors) {
					// 	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

					// cv::circle(wrap_frame, *(cv::Point2f*)&temp[0], 3, cv::Scalar(255, 0, 0), -1);

					for (int i = 0; i < 4; i++) {
						cv::circle(frame, *(cv::Point2f*)&temp[i], 3, cv::Scalar(255.0f * color_detection_data.wrap_points_ik_positions[i].x, 255.0f * color_detection_data.wrap_points_ik_positions[i].y, 255), -1);
						// cv::line(frame, *(cv::Point2f*)&temp[i], *(cv::Point2f*)&temp[(i - 1 < 0) ? 0 : i - 1], cv::Scalar(255.0f * color_detection_data.wrap_points_ik_positions[i].x, 255.0f * color_detection_data.wrap_points_ik_positions[i].y, 255));
					}

					// 	cv::inRange(hsv, cv::Scalar(color_detect.min.x, color_detect.min.y, color_detect.min.z), cv::Scalar(color_detect.max.x, color_detect.max.y, color_detect.max.z), mask);

					// 	// cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
						
					// 	// cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
					// 	// cv::dilate( mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

					// 	// //morphological closing (fill small holes in the foreground)
					// 	// cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5))); 
					// 	// cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

					// 	break;
					// }

					// cv::cvtColor(mask, wrap_frame, cv::COLOR_GRAY2BGR);

					switch (color_detection_data.view_mode) {
						case RawView:
							color_detection_data.selected_mat = &frame;
							break;
						
						case WrappedView:
							color_detection_data.selected_mat = &wrap_frame;
							break;
						
						default:
							color_detection_data.selected_mat = nullptr;
					};
				}
			}

			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			if (ImGui::BeginChild("##View") || true) {
				ImGui::PopStyleVar();

				static lgl::Texture* texture = nullptr;

				auto pos = ImGui::GetWindowPos();
				auto size = ImGui::GetWindowSize();
				auto cursor_pos =  ImGui::GetMouseCursor();

				ImGui::InvisibleButton("##view_window_invisible_button", size);

				color_detection_data.view_window_pos = *(glm::ivec2*)&pos;
				color_detection_data.view_window_size = *(glm::ivec2*)&size;

				color_detection_data.is_view_window_hovered = ImGui::IsItemHovered();
				color_detection_data.is_view_window_clicked = ImGui::IsItemClicked();
				color_detection_data.is_view_window_clicked_left = ImGui::IsMouseClicked(ImGuiMouseButton_Left) && color_detection_data.is_view_window_hovered;
				color_detection_data.is_view_window_clicked_right = ImGui::IsMouseClicked(ImGuiMouseButton_Right) && color_detection_data.is_view_window_hovered;

				if (color_detection_data.is_view_window_clicked) {
					color_detection_data.view_window_mouse_pos_normalized.x = (ImGui::GetMousePos().x - pos.x) / size.x;
					color_detection_data.view_window_mouse_pos_normalized.y = (ImGui::GetMousePos().y - pos.y) / size.y;
				}

				if (texture == nullptr) {
					texture = new lgl::Texture;
					texture->mode = lgl::TEXTURE_2D;
				}

				if (color_detection_data.selected_mat != nullptr) {
					MatToTexture(texture, *color_detection_data.selected_mat);
				}

				ImGui::GetWindowDrawList()->AddImage((void*)texture->id, ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1));
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();

	color_detection_data.initialize = false;
}
