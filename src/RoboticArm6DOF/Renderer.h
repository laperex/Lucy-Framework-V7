#pragma once

#include "JointAngles.h"

namespace lra {
	void IntializeRenderer();

	// void RenderBase(float base_angle);
	// void RenderArm(float arm_angle);
	// void RenderElbow(float elbow_angle);
	// void RenderWrist(float wrist_angle);
	// void RenderGripper(float gripper_angle);

	void RenderLRA(JointAngles jont_angles);
}