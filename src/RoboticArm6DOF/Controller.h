#pragma once

#include <Lucy/Lucy.h>
#include "JointAngles.h"
#include "JointLength.h"

namespace lra {
	struct Controller {
		bool ik_enable = false;

		glm::ivec3 ik_target = { 150, 150, 150 };
		bool is_ik_picking = true;
		float ik_phi = 0;
		float speed = 10;

		glm::ivec3 fk_result;

		JointAngles target_joint_angles = { 90, 90, 180, 180, 0, 45 };
		JointLength lra_dimension = { 105, 100, 190 };

		bool enable_smoothing = false;
		JointAngles last_angles;
		JointAngles render_angles;
	};
}