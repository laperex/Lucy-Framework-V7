#pragma once

#include <Lucy/Lucy.h>
#include "JointAngles.h"
#include "JointLength.h"

namespace lra {
	enum RoboticArmMode {
		WRITING,
		PICKING,

		RoboticArmMode_COUNT
	};
	struct Controller {
		bool enable_ik = false;
		glm::vec3 ik_target = { 150, 150, 150 };
		float phi = -45;
		RoboticArmMode mode;

		glm::vec3 fk_result;
		glm::vec3 fk_result_arm;
		glm::vec3 fk_result_elbow;
		glm::vec3 fk_result_wrist;

		JointAngles target_joint_angles = { 90, 90, 180, 180, 0, 45 };
		JointLength lra_dimension = { 105, 100, 190 };
	};
}