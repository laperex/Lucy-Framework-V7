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
#include <glm/gtx/string_cast.hpp>
#include "Sorting.h"

#define ENABLE_LOADSAVE true

cv::Mat GetMask(const cv::Mat& frame, glm::ivec3 min, glm::ivec3 max) {
	cv::Mat hsv, mask;
	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(min[0], min[1], min[2]), cv::Scalar(max[0], max[1], max[2]), mask);
	cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

	return mask;
}

static std::tuple<bool, glm::vec2, std::vector<glm::vec2>> DetectPosition(const cv::Mat& mask, float max_area = 100) {
	static std::vector<std::vector<cv::Point>> contours;
	static std::vector<cv::Point> approx;
	std::vector<glm::vec2> points_all;

	cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// static cv::Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0,0,0));

	if (contours.size()) {
		int max_contour_idx = -1;
		for (size_t idx = 0; idx < contours.size(); idx++) {
			// cv::drawContours(contourImage, contours, idx, cv::Scalar(0, 0, 255));
			auto area = cv::contourArea(contours[idx]);
			if (area > max_area) {
				auto M = cv::moments(contours[idx]);
				if (M.m00 != 0) {
					auto cx = int(M.m10 / M.m00);
					auto cy = int(M.m01 / M.m00);
					points_all.push_back({ cx, cy });
				}

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
			// auto rect = cv::boundingRect(approx);
			// cv::rectangle(frame, rect, (0, 255, 0), 4);
			auto M = cv::moments(contours[max_contour_idx]);
			if (M.m00 != 0) {
				auto cx = int(M.m10 / M.m00);
				auto cy = int(M.m01 / M.m00);
			// 	// cv::circle(frame, { cx, cy }, 3, (255, 0, 0), -1);
				return { true, { cx, cy }, points_all };
			}
		}
	}

	return { false, { 0, 0 }, {} };
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

struct DetectColor {
	glm::vec3 min, max;
	std::string name;

	DetectColor() {}
	DetectColor(glm::vec3 min, glm::vec3 max, std::string name): min(min), max(max), name(name) {}
};

struct ColorDetectionData {
	std::vector<DetectColor> available_colors = {
		{
			{ 0, 132, 156 },
			{ 11, 255, 199 },
			"Red"
		}
	};

	std::vector<glm::vec2> wrap_points_normals = {
		glm::vec2(0.122, 0.390),
		glm::vec2(0.170, 0.765),
		glm::vec2(0.731, 0.314),
		glm::vec2(0.745, 0.769),
	};

	std::vector<glm::vec2> wrap_points_ik_positions = {
		glm::vec2(0, 0),
		glm::vec2(0, 400),
		glm::vec2(800, 0),
		glm::vec2(800, 400),
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

	float max_area = 100;
};

void lra::panel::ColorDetectionPanel() {
	static ColorDetectionData color_detection_data;

	auto& controller = Registry::Instance().store<Controller>();
	auto& sorting = Registry::Instance().store<Sorting>();
	auto& animator = Registry::Instance().store<Animator>();

	if (ImGui::Begin("ColorDetectionPanel")) {
		if (ENABLE_LOADSAVE) {
			if (color_detection_data.initialize) {
				uint8_t* data = nullptr;
				std::size_t size;

				data = util::read_bytes_from_file("color_detection_data.wrap_points_normals.bin", size);
				if (size) color_detection_data.wrap_points_normals = decltype(color_detection_data.wrap_points_normals)((glm::vec2*)data, (glm::vec2*)data + (size / sizeof(((glm::vec2*)data)[0])));

				// data = util::read_bytes_from_file("color_detection_data.wrap_points_ik_positions.bin", size);
				// if (size) color_detection_data.wrap_points_ik_positions = decltype(color_detection_data.wrap_points_ik_positions)((glm::vec2*)data, (glm::vec2*)data + (size / sizeof(((glm::vec2*)data)[0])));

				data = util::read_bytes_from_file("color_detection_data.available_colors.bin", size);
				if (size) color_detection_data.available_colors = decltype(color_detection_data.available_colors)((DetectColor*)data, (DetectColor*)data + (size / sizeof(((DetectColor*)data)[0])));
			}
			if (lucy::Events::IsQuittable()) {
				util::write_bytes_to_file("color_detection_data.available_colors.bin", (uint8_t*)color_detection_data.available_colors.data(), color_detection_data.available_colors.size() * sizeof(color_detection_data.available_colors[0]));
				util::write_bytes_to_file("color_detection_data.wrap_points_normals.bin", (uint8_t*)color_detection_data.wrap_points_normals.data(), color_detection_data.wrap_points_normals.size() * sizeof(color_detection_data.wrap_points_normals[0]));
				// util::write_bytes_to_file("color_detection_data.wrap_points_ik_positions.bin", (uint8_t*)color_detection_data.wrap_points_ik_positions.data(), color_detection_data.wrap_points_ik_positions.size() * sizeof(color_detection_data.wrap_points_ik_positions[0]));
			}
		}

		if (color_detection_data.initialize) {
			color_detection_data.capture.open(1);

			sorting.Initialize();
		}

		bool load_frame = false;
		bool ik_move = false;
		static float slider = 0.17;
		ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;
		static int show_mask_idx = 0;

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

				if (ImGui::Button("Move")) {
					ik_move = true;
				}

				ImGui::SliderDragFloat("Max Area", &color_detection_data.max_area, 0.1, 0, 500);

				if (ImGui::Begin("ColorMenu", &color_menu)) {
					if (ImGui::BeginTable("##ColorMenuTable", 4, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Name");

						ImGui::TableSetColumnIndex(1);
						ImGui::Text("Min");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("Max");

						ImGui::TableSetColumnIndex(3);
						ImGui::Text("Mask");

						int idx = 0;
						for (int i = 0; i < color_detection_data.available_colors.size(); i++) {
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);

							ImGui::SanitisedInputText(("##SelectedInputText" + std::to_string(idx)).c_str(), color_detection_data.available_colors[i].name);

							ImGui::TableSetColumnIndex(1);
							ImGui::ColorEdit3(("##min" + std::to_string(idx)).c_str(), &color_detection_data.available_colors[i].min[0], { ImGui::GetColumnWidth(), 0 });

							ImGui::TableSetColumnIndex(2);
							ImGui::ColorEdit3(("##max" + std::to_string(idx)).c_str(), &color_detection_data.available_colors[i].max[0], { ImGui::GetColumnWidth(), 0 });

							ImGui::TableSetColumnIndex(3);
							if (ImGui::Button((show_mask_idx == idx) ? "Viewing" : "Show")) {
								show_mask_idx = idx;
							}

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

				if (ImGui::Begin("WarpSettings", &color_menu)) {
					if (ImGui::BeginTable("##123423140203", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Index");

						ImGui::TableSetColumnIndex(1);
						ImGui::Text("Select");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("S.Pos");

						int idx = 0;
						for (int i = 0; i < color_detection_data.wrap_points_normals.size(); i++) {
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::Text(std::to_string(idx).c_str());

							ImGui::TableSetColumnIndex(1);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
							if (ImGui::Button(("Reset##" + std::to_string(idx)).c_str())) {
								// selected_pos = idx + 1;
							}
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(1));
							ImGui::DragFloat2(("##1212##" + std::to_string(idx)).c_str(), &color_detection_data.wrap_points_normals[i][0], 0.001, 0);

							ImGui::TableSetColumnIndex(2);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(2));
							ImGui::DragFloat2(("##4364##" + std::to_string(idx)).c_str(), &color_detection_data.wrap_points_ik_positions[i].x, 0, 0);

							idx++;
						}

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(1);
						// if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
						// 	p_array.push_back({ 0.5, 0.5 });
						// 	if (p_array.size() > dest_array.size()) {
						// 		dest_array.push_back({ 500, 500 });
						// 	}
						// }

						ImGui::EndTable();
					}
				}
				ImGui::End();
			}
			ImGui::EndChild();

			// Calculations
			static cv::Mat frame;
			static bool is_frame_available;
			color_detection_data.selected_mat = nullptr;
			if (load_frame) {
				is_frame_available = color_detection_data.capture.read(frame);
			}

			cv::Mat selected;
			static std::vector<std::pair<std::string, glm::vec2>> selected_points;

			if (ik_move) {
				if (selected_points.size()) {
					glm::ivec2 pos;
					pos.x = (1 - (selected_points[0].second.y / (float)400)) * 300;
					pos.y = (selected_points[0].second.x / (float)800) * 400 - 200;

					if (selected_points[0].first == "Red")
						sorting.SetParameters(SelectedBall::RED, pos);
						
					if (selected_points[0].first == "Blue")
						sorting.SetParameters(SelectedBall::BLUE, pos);

					animator.selected_animation = sorting.animations_id;
					animator.animationstate = PLAY;
					// controller.ik_target.z = pos.y;
					// controller.ik_target.x = pos.x;

					std::cout << glm::to_string(pos) << '\n';
				}
			} else if (is_frame_available && animator.animationstate != PLAY) {
				cv::Mat wrap_frame;
				auto temp = color_detection_data.wrap_points_normals;

				for (int i = 0; i < 4; i++) {
					temp[i].x = (float)800 * temp[i].x;
					temp[i].y = (float)400 * temp[i].y;

					if (color_detection_data.wrap_points_ik_positions[i].x > 800) {
						color_detection_data.wrap_points_ik_positions[i].x = 800;
					}
					if (color_detection_data.wrap_points_ik_positions[i].y > 400) {
						color_detection_data.wrap_points_ik_positions[i].y = 400;
					}
				}

				auto M = cv::getPerspectiveTransform((cv::Point2f*)temp.data(), (cv::Point2f*)color_detection_data.wrap_points_ik_positions.data());
				cv::warpPerspective(frame, wrap_frame, M, cv::Size(800, 400));

				selected_points.clear();

				cv::Mat mask;
				int idx = 0;
				for (auto& color: color_detection_data.available_colors) {
					glm::ivec3 min = glm::ivec3(color.min * 255.0f);
					glm::ivec3 max = glm::ivec3(color.max * 255.0f);

					mask = GetMask(wrap_frame, min, max);
					auto [is_frame, point, points_all] = DetectPosition(mask, color_detection_data.max_area);
					if (is_frame) {
						bool is_valid;

						glm::ivec3 pos;
						pos.x = (1 - (point.y / (float)400)) * 300;
						pos.y = 30;
						pos.z = (point.x / (float)800) * 400 - 200;

						Kinematics::GetInverseKinematics(is_valid, pos);

						if (is_valid) {
							selected_points.push_back(std::make_pair(color.name, point));
							cv::circle(wrap_frame, *(cv::Point2f*)&point, 5, cv::Scalar(0, 255, 255), 3);
						}
					}

					if (color_detection_data.show_mask && show_mask_idx == idx) {
						cv::cvtColor(mask, selected, cv::COLOR_GRAY2BGR);
					}

					idx++;
				}

				for (int i = 0; i < 4; i++) {
					cv::circle(frame, *(cv::Point2f*)&temp[i], 3, cv::Scalar(255.0f * color_detection_data.wrap_points_ik_positions[i].x, 255.0f * color_detection_data.wrap_points_ik_positions[i].y, 255), -1);
				}

				color_detection_data.selected_mat = &selected;

				switch (color_detection_data.view_mode) {
					case RawView:
						selected = frame;
						break;
					
					case WrappedView:
						if (!color_detection_data.show_mask)
							selected = wrap_frame;
						break;
					
					default:
						color_detection_data.selected_mat = nullptr;
				};
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
