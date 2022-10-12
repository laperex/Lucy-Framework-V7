#include "Easing.h"
#include "JointAngles.h"
#include "Controller.h"
#include "Kinematics.h"
#include <Lucy/Lucy.h>
#include <glm/glm.hpp>

static auto& registry = Registry::Instance();

void lra::EaseMovement() {
	// auto& controller = registry.store<Controller>();

	// static JointAngles target_angles;
	// static JointAngles last_angles;
	// static float progress_len = 0;
	// static bool first = false;
	// static bool set_target = false;

	// if (!first) {
	// 	last_angles = controller.target_joint_angles;
	// 	first = true;
	// } else if (!set_target) {
	// 	target_angles = controller.target_joint_angles;
	// 	set_target = true;
	// } else {
	// 	controller.render_angles;
	// }
}
