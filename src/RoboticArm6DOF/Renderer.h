#pragma once

#include "JointAngles.h"
#include <glm/glm.hpp>

namespace lra {
	void IntializeRenderer();

	// void RenderBase(float base_angle);
	// void RenderArm(float arm_angle);
	// void RenderElbow(float elbow_angle);
	// void RenderWrist(float wrist_angle);
	// void RenderGripper(float gripper_angle);

	void RenderLRA(JointAngles jont_angles);
	void RenderCube(glm::vec3 pos, glm::vec3 scale, int val);

	glm::vec4 PickingData(const glm::vec2& coord);
}
