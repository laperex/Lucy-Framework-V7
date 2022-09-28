#pragma once

#define LRA_MAX_JOINT (6)

namespace lra {
	struct JointAngles {
		float base = 0;
		float arm = 0;
		float elbow = 0;
		float wrist = 0;
		float gripper_rotate = 0;
		float gripper_control = 0;
		
		JointAngles() {}
		JointAngles(float angle);
		JointAngles(float base, float arm, float elbow, float wrist, float gripper_rotation, float gripper);
		float& operator[](int idx);
	};
}