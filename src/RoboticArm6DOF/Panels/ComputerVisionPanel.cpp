#include "Panel.h"
// #include <opencv2/opencv.hpp>
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

static auto& registry = Registry::Instance();

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

void lra::panel::ComputerVisionPanel() {
	static ComputerVision* vision = nullptr;

	if (vision == nullptr) {
		vision = new ComputerVision();
		vision->Initialize();
	}

	auto& controller = registry.store<Controller>();

	static cv::Mat frame;
	static bool live_feed = false;

	static std::vector<cv::Point2f> p_array = {
		{ 0.122, 0.390 },
		// cv::Point2f(0, 0.5),
		{ 0.170, 0.765 },
		// cv::Point2f(0.5, 1),
		{ 0.731, 0.314 },
		// cv::Point2f(1, 0.5),
		{ 0.745, 0.769 },
	};
	static std::vector<cv::Point2f> dest_array = {
		{ 0, 0 },
		// { 0, 400 / 2 },
		{ 0, 400 },
		// { 800 / 2, 400 },
		{ 800, 0 },
		// { 800, 400 / 2 },
		{ 800, 400 },
	};

	static bool initialize = true;
	if (initialize) {
		{
			auto [data, size] = util::read_bytes_from_file("p_array.bin");
			if (size) {
				p_array = decltype(p_array)((cv::Point2f*)data, (cv::Point2f*)data + (size / sizeof(((cv::Point2f*)data)[0])));
			}
		}
		{
			auto [data, size] = util::read_bytes_from_file("dest_array.bin");
			if (size) {
				dest_array = decltype(dest_array)((cv::Point2f*)data, (cv::Point2f*)data + (size / sizeof(((cv::Point2f*)data)[0])));
			}
		}

		initialize = false;
	}

	if (lucy::Events::IsQuittable()) {
		util::write_bytes_to_file("p_array.bin", (uint8_t*)p_array.data(), p_array.size() * sizeof(p_array[0]));
		util::write_bytes_to_file("dest_array.bin", (uint8_t*)dest_array.data(), dest_array.size() * sizeof(dest_array[0]));
	}

	static glm::ivec2 wrap_size = { 640, 480 };
	static glm::vec2 norm_wrap_sel_pos = { 0, 0 };
	static bool is_wrap_select_pos = false;
	static glm::ivec3 target = { 0, 100, 0};

	static int selected_pos = 0;

	live_feed = false;

	if (ImGui::Begin("CV_Calc"/* , nullptr, ImGuiWindowFlags_NoTitleBar */)) {
		// ImGui::Checkbox("Live Feed", &live_feed);

		ImGui::NewLine();

		target.z = norm_wrap_sel_pos.x * 400 - 200;
		target.x = (1 - norm_wrap_sel_pos.y) * 200;

		if (ImGui::Button("Select Pos")) {
			is_wrap_select_pos = true;
		}

		ImGui::DragInt3("Target", &target[0]);

		static bool toggle_move = false;
		if (ImGui::Button("Move")) {
			toggle_move = !toggle_move;
		}

		if (toggle_move) {
			controller.ik_target = target;
		}

		ImGui::NewLine();

		if (ImGui::Button("Detect")) {
			live_feed = true;
		}

		ImGui::NewLine();

		if (ImGui::BeginTable("##123423140203", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Index");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Select");

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("S.Pos");

			int idx = 0;
			for (int i = 0; i < p_array.size(); i++) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(std::to_string(idx).c_str());

				ImGui::TableSetColumnIndex(1);
				if (ImGui::Button(("Reset##" + std::to_string(idx)).c_str())) {
					selected_pos = idx + 1;
				}
				ImGui::DragFloat2(("##1212##" + std::to_string(idx)).c_str(), &p_array[i].x, 0.001, 0);

				ImGui::TableSetColumnIndex(2);
				ImGui::DragFloat2(("##4364##" + std::to_string(idx)).c_str(), &dest_array[i].x, 0, 0);
				if (dest_array[i].x > wrap_size.x) {
					dest_array[i].x = wrap_size.x;
				}
				if (dest_array[i].y > wrap_size.y) {
					dest_array[i].y = wrap_size.y;
				}

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

	static lgl::Texture* frame_texture = nullptr;
	if (frame_texture == nullptr) frame_texture = new lgl::Texture;

	static lgl::Texture* wrap_texture = nullptr;
	if (wrap_texture == nullptr) wrap_texture = new lgl::Texture;

	static cv::Mat wrap_frame;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

	ImGui::Begin("CV_Disp"/* , nullptr, ImGuiWindowFlags_NoTitleBar */);

	{
		ImGui::PopStyleVar();

		auto [x, y] = ImGui::GetWindowPos();
		auto [w, h] = ImGui::GetWindowSize();

		auto* drawlist = ImGui::GetWindowDrawList();

		ImGui::InvisibleButton("##invis", { w, h });
		if (!ImGui::IsItemHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			selected_pos = 0;
		}
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked() && selected_pos != 0) {
			p_array[selected_pos - 1].x = (ImGui::GetMousePos().x - x) / w;
			p_array[selected_pos - 1].y = (ImGui::GetMousePos().y - y) / h;
		}

		if (live_feed) {
			bool is_frame = vision->input_camera.read(frame);

			if (is_frame) {
				if (dest_array.size() >= 4) {
					auto temp = p_array;

					for (int i = 0; i < temp.size(); i++) {
						temp[i].x = (float)wrap_size.x * temp[i].x;
						temp[i].y = (float)wrap_size.y * temp[i].y;
					}

					auto M = cv::getPerspectiveTransform(temp.data(), dest_array.data());
					cv::warpPerspective(frame, wrap_frame, M, cv::Size(wrap_size.x, wrap_size.y));
					auto [is_frame, point] = DetectPosition(wrap_frame);
					if (is_frame) {
						cv::circle(wrap_frame, *(cv::Point2f*)&point, 5, cv::Scalar(0, 255, 255), 3);

						norm_wrap_sel_pos.x = (point.x / (float)wrap_frame.cols);
						norm_wrap_sel_pos.y = (point.y / (float)wrap_frame.rows);
						target.y = 100;
					}

					cv::cvtColor(wrap_frame, wrap_frame, cv::COLOR_BGR2RGBA);
					MatToTexture(wrap_texture, wrap_frame);
				}

				// for (auto& p: p_array) {
				// 	cv::Point point;
				// 	point.x = (float)frame.cols * p.x;
				// 	point.y = (float)frame.rows * p.y;
				// 	cv::circle(frame, point, 3, cv::Scalar(0, 0, 255));
				// }

				cv::cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
				MatToTexture(frame_texture, frame);

				// {
				// 	auto copy = frame;

				// 	cv::circle(copy, p_array[0b00], 3, cv::Scalar(0, 0, 255));
				// 	cv::putText(copy, "P00", p_array[0b00], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));

				// 	cv::circle(copy, p_array[0b01], 3, cv::Scalar(0, 255, 255));
				// 	cv::putText(copy, "P01", p_array[0b01], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255));

				// 	cv::circle(copy, p_array[0b10], 3, cv::Scalar(255, 0, 255));
				// 	cv::putText(copy, "P10", p_array[0b10], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 255));

				// 	cv::circle(copy, p_array[0b11], 3, cv::Scalar(255, 255, 255));
				// 	cv::putText(copy, "P11", p_array[0b11], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255));/
				// }
			}
		}

		drawlist->AddImage((void*)frame_texture->id, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0, 0), ImVec2(1, 1));

		for (int i = 0; i < p_array.size(); i++) {
			drawlist->AddCircle({ x + w * p_array[i].x, y + h * p_array[i].y }, 4, IM_COL32(255 * ((i + 1) % 2), 255 * (i % 2), 255, 255));
		}
		// drawlist->AddCircle({ 100, 100 }, 4, IM_COL32(255, 0, 255, 255));
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

	ImGui::Begin("CV_Disp_Wrap"/* , nullptr, ImGuiWindowFlags_NoTitleBar */);
	{
		ImGui::PopStyleVar();

		auto [x, y] = ImGui::GetWindowPos();
		auto [w, h] = ImGui::GetWindowSize();

		ImGui::InvisibleButton("##hverbutwqruio", { w, h });
		if (ImGui::IsItemHovered() && ImGui::IsItemClicked() && is_wrap_select_pos) {
			norm_wrap_sel_pos.x = (ImGui::GetMousePos().x - x) / w;
			norm_wrap_sel_pos.y = (ImGui::GetMousePos().y - y) / h;
		}
		if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			is_wrap_select_pos = false;
		}

		auto* drawlist = ImGui::GetWindowDrawList();
		drawlist->AddImage((void*)wrap_texture->id, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0, 0), ImVec2(1, 1));
	}
	ImGui::End();
}

// #include "Panel.h"
// // #include <opencv2/opencv.hpp>
// #include <Lucy/imgui_lucy_impl.h>
// #include <SDL2/SDL.h>
// #include <glad/glad.h>
// #include <LucyGL/LucyGL.h>
// #include <RoboticArm6DOF/Vision.h>
// #include <RoboticArm6DOF/Controller.h>
// #include <Lucy/ECS.h>
// #include <glm/glm.hpp>

// static auto& registry = Registry::Instance();

// static lgl::Texture* texture = nullptr;
// static struct {
// 	glm::vec2 pos_2d = { 0, 0 };
// 	glm::ivec3 pos_ik[4];
// 	glm::vec2 size = { 0.5, 0.3 };
// 	glm::vec2 target = { 0.5, 0.5 };
// } position;

// void lra::panel::ComputerVisionPanel() {
// 	auto& vision = registry.store<ComputerVision>();
// 	auto& controller = registry.store<Controller>();

// 	static ImVec2 pos, size;
// 	static glm::ivec3 c_target_pos;
// 	static int pos_idx = -1;

// 	if (texture == nullptr) {
// 		texture = new lgl::Texture();
// 	}

// 	if (ImGui::Begin("Vision"/* , nullptr, ImGuiWindowFlags_NoTitleBar */)) {
// 		static float slider = 0.17;

// 		ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
// 		float offset = ImGui::GetContentRegionAvail().x * slider;

// 		ImGui::Columns(2, 0, false);
// 		ImGui::SetColumnOffset(1, offset);

// 		{
// 			ImGui::BeginChild("#0");

// 			if (ImGui::Button((vision.enable_live_feed) ? "Disable Live Feed": " Enable Live Feed")) {
// 				vision.enable_live_feed = !vision.enable_live_feed;
// 			}

// 			if (ImGui::TreeNodeEx("p", ImGuiTreeNodeFlags_DefaultOpen)) {
// 				if (ImGui::Button(("Set Image Pos##" + std::to_string(0)).c_str())) {
// 					pos_idx = 1;
// 				}

// 				ImGui::DragFloat2("Size", &position.size[0], 0.001);

// 				for (int i = 0; i < 4; i++) {
// 					ImGui::DragInt3(("##" + std::to_string(i)).c_str(), &position.pos_ik[i][0], 0);
// 					ImGui::SameLine();

// 					switch (i) {
// 						case 0:
// 							ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
// 							break;
// 						case 1:
// 							ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 255, 0, 255));
// 							break;
// 						case 2:
// 							ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 255, 0, 255));
// 							break;
// 						case 3:
// 							ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 255, 255));
// 							break;
// 					};

// 					if (ImGui::Button(("##" + std::to_string(i)).c_str(), { ImGui::GetContentRegionAvail().x, 0 })) {
// 						position.pos_ik[i] = controller.ik_target;
// 					}

// 					ImGui::PopStyleColor();
// 				}

// 				ImGui::SliderDragFloat("Target Pos 2D.x", &position.target[0], 0.001, 0, 1);
// 				ImGui::SliderDragFloat("Target Pos 2D.y", &position.target[1], 0.001, 0, 1);
// 				ImGui::DragInt3("Target Pos", &c_target_pos[0], 0);

// 				if (ImGui::Button("MoveToPos")) {
// 					controller.ik_target = c_target_pos;
// 				}

// 				ImGui::TreePop();
// 			}


// 			ImGui::EndChild();
// 		}

// 		ImGui::NextColumn();

// 		{
// 			ImGui::BeginChild("#1");
		
// 			if (!vision.frame.empty()) {
// 				cv::cvtColor(vision.frame, vision.frame, cv::COLOR_BGR2RGBA);

// 				texture->Bind();
// 				texture->SetFilteringMode(lgl::FilterMode_LINEAR, lgl::FilterMode_LINEAR);
// 				texture->SetWrapMode(lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT);
// 				glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
// 				texture->Load2D(0, lgl::RGBA, vision.frame.cols, vision.frame.rows, 0, lgl::RGBA, lgl::UNSIGNED_BYTE, vision.frame.data);
// 				texture->UnBind();

// 				pos = ImGui::GetWindowPos();
// 				size = ImGui::GetWindowSize();

// 				ImGui::InvisibleButton("##ib", size);
// 				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
// 					if (pos_idx > -1) {
// 						auto mpos = ImGui::GetMousePos();
// 						mpos.x = mpos.x - pos.x;
// 						mpos.y = mpos.y - pos.y;
// 						position.pos_2d.x = mpos.x / size.x;
// 						position.pos_2d.y = mpos.y / size.y;
// 					}
// 				} else if (pos_idx > -1 && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
// 					pos_idx = -1;
// 				}

// 				auto* drawlist = ImGui::GetWindowDrawList();
// 				drawlist->AddImage((void*)texture->id, ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(1, 1), ImVec2(0, 0));

// 				glm::vec2 p00 = position.pos_2d * *(glm::vec2*)&size + *(glm::vec2*)&pos;
// 				glm::vec2 p01 = glm::vec2(position.pos_2d.x * size.x, position.pos_2d.y * size.y + position.size.y * size.y) + *(glm::vec2*)&pos;
// 				glm::vec2 p10 = glm::vec2(position.pos_2d.x * size.x + position.size.x * size.x, position.pos_2d.y * size.y) + *(glm::vec2*)&pos;
// 				glm::vec2 p11 = glm::vec2(position.pos_2d.x * size.x + position.size.x * size.x, position.pos_2d.y * size.y + position.size.y * size.y) + *(glm::vec2*)&pos;

// 				drawlist->AddCircleFilled({ p00.x, p00.y }, 3, IM_COL32(255, 0, 0, 255));	// 0
// 				drawlist->AddCircleFilled({ p01.x, p01.y }, 3, IM_COL32(0, 255, 0, 255));	// 1
// 				drawlist->AddCircleFilled({ p10.x, p10.y }, 3, IM_COL32(255, 255, 0, 255));	// 2
// 				drawlist->AddCircleFilled({ p11.x, p11.y }, 3, IM_COL32(255, 0, 255, 255));	// 3
		
// 				drawlist->AddCircleFilled({ pos.x + position.target.x * size.x, pos.y + position.target.y * size.y }, 5, 0xffffffff);

// 				drawlist->AddLine({ p00.x, p00.y }, { p01.x, p01.y }, IM_COL32(255, 0, 0, 255));
// 				drawlist->AddLine({ p01.x, p01.y }, { p11.x, p11.y }, IM_COL32(0, 255, 0, 255));
// 				drawlist->AddLine({ p11.x, p11.y }, { p10.x, p10.y }, IM_COL32(255, 255, 0, 255));
// 				drawlist->AddLine({ p10.x, p10.y }, { p00.x, p00.y }, IM_COL32(255, 0, 255, 255));
// 			}

// 			ImGui::EndChild();
// 		}

// 		ImGui::End();
// 	}
// }
