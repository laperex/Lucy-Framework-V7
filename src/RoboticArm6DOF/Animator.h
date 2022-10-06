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
		glm::ivec3 target_position;

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

		std::vector<JointAngles> generated;
		std::size_t size;

		void Generate();
		AnimationProperty() {}
	};

	struct Animator {
	private:
		struct AnimationPropertyContainer {
			std::string name;
			AnimationProperty animation;
		};

	public:
		std::unordered_map<LUCY_UUID, AnimationPropertyContainer> animation_registry;
		AnimtationState animtationstate = STOP;
		LUCY_UUID selected_animation = LUCY_NULL_UUID;

		bool IsNamePresent(std::string name);
		std::string GetName(std::string name);
		LUCY_UUID NewAnimation(std::string name, AnimationProperty animation, LUCY_UUID id = LUCY_GENERATE_UUID);
	};
}