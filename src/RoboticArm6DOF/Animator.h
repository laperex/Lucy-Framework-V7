#pragma once

#include "JointAngles.h"
#include "Kinematics.h"
#include "Controller.h"
#include <unordered_map>

namespace lra {
	enum AnimtationState {
		PLAY,
		PAUSE,
		STOP,

		AnimtationState_COUNT
	};

	struct AnimationStep {
		JointAngles target_angles;
		glm::vec3 target_position;

		JointLength lra_dimension;
		RoboticArmMode mode;
		float phi;
		bool enable_ik_trajectory = true;

		float progress_len = 2000;	// Steps taken to reach this angles
		float pow_t = 1.9;	// Step Gradient

		AnimationStep() {}
	};

	struct AnimationProperty {
		std::vector<AnimationStep> step_array;
		bool loop = false;

		AnimationProperty() {}
	};

	struct Animator {
	private:
		struct AnimationPropertyContainer {
			std::string name;
			AnimationProperty animation;
		};

	public:
		std::unordered_map<UTIL_UUID, AnimationPropertyContainer> animation_registry;
		AnimtationState animtationstate = STOP;

		bool IsNamePresent(std::string name);
		std::string GetName(std::string name);
		UTIL_UUID NewAnimation(std::string name, AnimationProperty animation, UTIL_UUID id = UTIL_GENERATE_UUID);
	};
}