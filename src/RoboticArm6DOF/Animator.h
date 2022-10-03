#pragma once

#include "JointAngles.h"
#include "Kinematics.h"

namespace lra {
	struct AnimationStep {
		JointAngles angles;

		float progress_len = 2000;	// Steps taken to reach this angles
		float pow_t = 1.9;	// Step Gradient

		bool enable_linear_trajectory = true;

		AnimationStep() {}
		AnimationStep(JointAngles angles): angles(angles) {}
	};

	struct Animator {

	};
}