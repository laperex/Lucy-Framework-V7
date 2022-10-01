#pragma once

#include "JointAngles.h"
#include "JointLength.h"
#include <glm/glm.hpp>

namespace lra {
	class Kinematics {
	public:
		static const glm::vec3& GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions);
		static std::pair<bool, lra::JointAngles> GetInverseKinematics(const glm::vec3& target, const JointLength& lra_dimensions);
	};
}
