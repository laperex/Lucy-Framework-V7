#pragma once

#include "JointAngles.h"
#include "JointLength.h"
#include <glm/glm.hpp>

namespace lra {
	class Kinematics {
	public:
		static const glm::vec3& GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions);
		static lra::JointAngles GetInverseKinematics(bool& is_valid, const glm::vec3& target, const JointLength& lra_dimensions);
		static lra::JointAngles GetInverseKinematics(bool &is_valid, const glm::vec3& target, const JointLength& lra_dimensions, float phi);
	};
}
