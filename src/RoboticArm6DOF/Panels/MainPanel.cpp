#include "Panel.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <RoboticArm6DOF/Controller.h>
#include <RoboticArm6DOF/Canvas.h>
#include <RoboticArm6DOF/ArmInfo.h>
#include <glad/glad.h>
#include <iostream>
#include <Lucy/Lucy.h>
#include <Lucy/Window.h>
#include <Lucy/Editor.h>
#include <glm/gtc/matrix_transform.hpp>
#include <RoboticArm6DOF/Renderer.h>
#include <RoboticArm6DOF/RoboticArm.h>

static auto& registry = Registry::Instance();

void lra::panel::MainPanel() {
	auto& window = registry.store<lucy::Window>();
	auto& camera = registry.store<lucy::Camera>();
	auto& canvas = registry.store<Canvas>();
	auto& controller = registry.store<Controller>();
	auto& info = registry.store<ArmInfo>();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

	ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoTitleBar);

	ImGui::PopStyleVar();
	{
		auto [x, y] = ImGui::GetWindowPos();
		auto [w, h] = ImGui::GetWindowSize();

		if (camera.framebuffer != nullptr) {
			if (camera.framebuffer->width != w || camera.framebuffer->height != h) {
				delete camera.framebuffer;
				camera.framebuffer = nullptr;
			}
		}

		camera.enable = ImGui::IsWindowHovered();
		camera.posx = int(x);
		camera.posy = int(y);
		camera.width = int(w);
		camera.height = int(h);

		if (camera.framebuffer == nullptr) {
			camera.framebuffer = new lgl::FrameBuffer(w, h, true);
		}

		ImGui::GetWindowDrawList()->AddImage((void*)camera.framebuffer->texture->id, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0, (float)h / camera.framebuffer->height), ImVec2((float)w / camera.framebuffer->width, 0));

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(x, y, w, h);

		if (controller.ik_enable) {
			glm::mat4 delta;
			ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, &(glm::translate(glm::mat4(1.0f), glm::vec3(controller.ik_target)))[0][0], &delta[0][0]);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;

				ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &translation[0], &rotation[0], &scale[0]);

				controller.ik_target += translation;
				if (controller.ik_target.x <= 0)
					controller.ik_target.x = 0;
			}
		}

		static uint32_t selected_id = 0;
		static glm::vec4 pixel;
		if (selected_id == 100 && !controller.ik_enable) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(canvas.position)) * glm::scale(glm::mat4(1.0f), glm::vec3(canvas.scale));
			ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::SCALE_X | ImGuizmo::OPERATION::SCALE_Z, ImGuizmo::LOCAL, &model[0][0]);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;

				ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &translation[0], &rotation[0], &scale[0]);

				canvas.position = translation;
				canvas.scale = scale;
			}
		} else if (selected_id && (!controller.ik_enable || (controller.ik_enable && (selected_id == 7 || selected_id == 8 || selected_id == 6)))) {
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, controller.target_joint_angles.base, 0))));
			glm::mat4 model = rotation;

			switch (selected_id) {
				case 1: case 2:
					model *= glm::translate(glm::mat4(1.0f), info.J0);
					break;
				
				case 3:
					model *= glm::translate(glm::mat4(1.0f), info.J1);
					break;
				
				case 4:
					model *= glm::translate(glm::mat4(1.0f), info.J2);
					break;
					
				case 5:
					model *= glm::translate(glm::mat4(1.0f), info.J3);
					break;

				case 6:
					model *= glm::translate(glm::mat4(1.0f), info.J4);
					break;
				
				case 7: case 8:
					model *= glm::translate(glm::mat4(1.0f), info.J5);
					break;
					
				default:
					break;
			}

			glm::mat4 delta;
			ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::ROTATE_SCREEN, ImGuizmo::WORLD, &model[0][0], &delta[0][0]);

			model *= glm::inverse(rotation);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;

				ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &translation[0], &rotation[0], &scale[0]);

				switch (selected_id) {
					case 1: case 2:
						controller.target_joint_angles.base += rotation.y;
						break;

					case 3:
						controller.target_joint_angles.arm += rotation.x;
						break;

					case 4:
						controller.target_joint_angles.elbow += rotation.x;
						break;

					case 5:
						controller.target_joint_angles.wrist += rotation.x;
						break;

					case 6:
						controller.target_joint_angles.gripper_rotate += rotation.x;
						break;

					case 7: case 8:
						controller.target_joint_angles.gripper_control += rotation.y;
						break;

					default:
						break;
				}

				for (int i = 0; i < 6; i++) {
					if (controller.target_joint_angles[i] > 180)
						controller.target_joint_angles[i] = 180;
					if (controller.target_joint_angles[i] < 0)
						controller.target_joint_angles[i] = 0;
				}
			}
		}

		if (lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT) && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing() && ImGui::IsWindowHovered()) {
			camera.framebuffer->Bind();
			glm::ivec2 norm = (lucy::Events::GetCursorPosNormalized(camera.posx, camera.posy, camera.width, camera.height) * glm::vec3(camera.width, camera.height, 0) + glm::vec3(camera.width, camera.height, 0)) / 2.0f;
			lgl::SetReadBuffer(lgl::Attachment::COLOR_ATTACHMENT1);
			lgl::ReadPixels(norm.x, norm.y, 1, 1, lgl::Format::RGBA, lgl::Type::FLOAT, &pixel[0]);
			lgl::ResetReadBuffer();
			selected_id = uint32_t(pixel.y);
			camera.framebuffer->UnBind();
			// std::cout << norm.x << ' ' << norm.y << " | " << selected_pixel.x << ' ' << selected_pixel.y << ' ' << selected_pixel.z << ' ' << selected_pixel.w << '\n';
		} else if (!ImGuizmo::IsOver() && !ImGuizmo::IsUsing() && lucy::Events::IsButtonPressedAny() && selected_id == 0 && ImGui::IsWindowHovered()) {
			selected_id = 0;
		}
	}
	ImGui::End();
}