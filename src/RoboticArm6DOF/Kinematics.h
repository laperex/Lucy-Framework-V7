#pragma once

#include "JointAngles.h"
#include "JointLength.h"
#include <Lucy/Math.h>

namespace lra {
	namespace Kinematics {
		const glm::ivec3 GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions);
		const glm::ivec3 GetForwardKinematics(const JointAngles& joint_angles);

		lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target, const JointLength& lra_dimensions);
		lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target);
		lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target, float phi);
		lra::JointAngles GetInverseKinematics(bool &is_valid, const glm::ivec3& target, const JointLength& lra_dimensions, float phi);
	};
}
