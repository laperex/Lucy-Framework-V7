#include "LRAGizmo.h"
#include "Controller.h"
#include "ArmInfo.h"
#include <glad/glad.h>
#include <Lucy/Lucy.h>
#include <Lucy/Window.h>
#include <Lucy/Editor.h>
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

	// float phi_0 = joint_angles.arm;
	// float phi_1 = phi_0 + joint_angles.elbow - 180;
	// float phi_2 = phi_1 + joint_angles.wrist - 180;

	if (controller.enable_ik || controller.mode == WRITING) {
		glm::mat4 delta;
		ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, &(glm::translate(glm::mat4(1.0f), controller.ik_target))[0][0], &delta[0][0]);

		if (ImGuizmo::IsUsing()) {
			glm::vec3 translation, rotation, scale;

			ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &translation[0], &rotation[0], &scale[0]);

			controller.ik_target += translation;
			if (controller.ik_target.x <= 0)
				controller.ik_target.x = 0;
		}

		static auto pixel = glm::vec4(0.0);
		if (lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT)) {
			auto norm = (lucy::Events::GetCursorPosNormalized(0, 0, window.size.x, window.size.y) * glm::vec3(window.size.x, window.size.y, 0) + glm::vec3(window.size.x, window.size.y, 0)) / 2.0f;

			lgl::SetReadBuffer(lgl::Attachment::COLOR_ATTACHMENT1);
			lgl::ReadPixels(norm.x, norm.y, 1, 1, lgl::Format::RGBA, lgl::Type::FLOAT, &pixel[0]);
			lgl::ResetReadBuffer();
		}
	}
}
