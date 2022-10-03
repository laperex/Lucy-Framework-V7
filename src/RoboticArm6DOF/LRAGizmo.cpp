#include "LRAGizmo.h"
#include "Controller.h"
#include <Lucy/Lucy.h>
#include <Lucy/Window.h>
#include <Lucy/Editor.h>
#include <glm/gtc/matrix_transform.hpp>

static auto& registry = Registry::Instance();

void lra::GizmoSystem() {
	auto& window = registry.store<lucy::Window>();
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(window.pos.x, window.pos.y, window.size.x, window.size.y);

	if (controller.enable_ik) {
		glm::mat4 delta;
		ImGuizmo::Manipulate(&camera.view[0][0], &camera.projection[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, &(glm::translate(glm::mat4(1.0f), controller.ik_target + glm::vec3(0, 72, 0)))[0][0], &delta[0][0]);

		if (ImGuizmo::IsUsing()) {
			glm::vec3 translation, rotation, scale;

			ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &translation[0], &rotation[0], &scale[0]);

			controller.ik_target += translation;
		}
	}
}

