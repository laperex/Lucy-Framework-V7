#pragma once

#include "JointAngles.h"
#include <glm/glm.hpp>
#include <LucyGL/LucyGL.h>

namespace lra {
	void IntializeRenderer();

	// void RenderBase(float base_angle);
	// void RenderArm(float arm_angle);
	// void RenderElbow(float elbow_angle);
	// void RenderWrist(float wrist_angle);
	// void RenderGripper(float gripper_angle);

	void RenderLRA(JointAngles jont_angles);
	void RenderCube(int val = 0, lgl::Shader* shader = nullptr);

	void SetGridColor(glm::vec4* bigger, glm::vec4* smaller);
	void RenderAxisLine(bool x, bool y, bool z);
	void RenderGrid();
	// void DrawGrid(uint32_t size = 1000, const std::vector<uint32_t>& unit = { 10, 100 });
	glm::vec4 PickingData(const glm::vec2& coord);
	uint32_t SelectID();
}
