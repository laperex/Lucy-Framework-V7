#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "LRAGizmo.h"
#include "Controller.h"
#include "ArmInfo.h"
#include <glad/glad.h>
#include <iostream>
#include <Lucy/Lucy.h>
#include <Lucy/Window.h>
#include <Lucy/Editor.h>
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

static auto& registry = Registry::Instance();

void lra::GizmoSystem() {
	auto& window = registry.store<lucy::Window>();
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();
	auto& info = registry.store<ArmInfo>();

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(window.pos.x, window.pos.y, window.size.x, window.size.y);

	if (controller.enable_ik || controller.mode == WRITING) {
		glm::mat4 delta;
		ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, &(glm::translate(glm::mat4(1.0f), glm::vec3(controller.ik_target)))[0][0], &delta[0][0]);

		if (ImGuizmo::IsUsing()) {
			glm::vec3 translation, rotation, scale;

			ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &translation[0], &rotation[0], &scale[0]);

			controller.ik_target += translation;
			if (controller.ik_target.x <= 0)
				controller.ik_target.x = 0;
		}
	} else {
		static uint32_t selected_id = 0;

		if (SelectID() && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
			selected_id = SelectID();

		if (selected_id) {
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
	}
}
