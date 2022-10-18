#include "Easing.h"
#include "JointAngles.h"
#include "Controller.h"
#include "Kinematics.h"
#include <Lucy/Lucy.h>
#include <glm/glm.hpp>

#define EASE_FUNC(x, t) (pow(x, t) / (pow(x, t) + pow(1 - x, t)))

static auto& registry = Registry::Instance();

void lra::EaseMovement() {
	auto& controller = registry.store<Controller>();

	static float first = false;

	if (!controller.enable_smoothing) {
		first = false;
		controller.render_angles = controller.target_joint_angles;
		return;
	}

	if (!first) {
		controller.render_angles = controller.target_joint_angles;
		first = true;
		return;
	}

	float len = glm::length(glm::vec3(Kinematics::GetForwardKinematics(controller.target_joint_angles)) - glm::vec3(Kinematics::GetForwardKinematics(controller.render_angles)));
	if (len != 0) {
		for (int i = 0; i < 6; i++)
			controller.render_angles[i] += (controller.target_joint_angles[i] - controller.render_angles[i]) * (1 / len);
	}
}
