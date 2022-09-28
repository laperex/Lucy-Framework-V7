#pragma once

#include <Lucy/Lucy.h>
#include "JointAngles.h"

namespace lra {
	struct Controller {
		glm::vec3 target_coordinate = { 100, 100, 100 };
		JointAngles target_joint_angles = { 90, 90, 180, 180, 90, 0 };

		bool ik_enable = false;
	};
}