#pragma once

#include "JointAngles.h"
#include "JointLength.h"
#include <Lucy/Math.h>

namespace lra {
	class Kinematics {
	public:
		static const glm::ivec3& GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions);
		static const glm::ivec3& GetForwardKinematics(const JointAngles& joint_angles);

		static lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target, const JointLength& lra_dimensions);
		static lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target);
		static lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::ivec3& target, float phi);
		static lra::JointAngles GetInverseKinematics(bool &is_valid, const glm::ivec3& target, const JointLength& lra_dimensions, float phi);
	};
}
