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
		bool is_ik_picking = true;

		float phi = 0;
		bool enable_ik_trajectory = false;

		float progress_len = 200;	// Steps taken to reach this angles
		float pow_t = 1.9;	// Step Gradient

		AnimationStep() {}
		AnimationStep(glm::ivec3 target_position): target_position(target_position) {}
	};

	struct Position {
		glm::ivec3 position;
		float gripper_rot;
		float gripper_ctrl;

		Position() {}
		Position(glm::ivec3 position, float gripper_rot, float gripper_ctrl): position(position), gripper_rot(gripper_rot), gripper_ctrl(gripper_ctrl) {}
		Position(const AnimationStep& step): position(step.target_position), gripper_rot(step.target_angles.gripper_rotate), gripper_ctrl(step.target_angles.gripper_control) {}
	};

	struct AnimationProperty {
		bool loop = false;
		bool is_ik_picking = true;

		std::vector<AnimationStep> step_array;
		std::vector<Position> generated_positions;

		void Generate();
		const std::vector<Position>& GetGenerated();
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
		AnimtationState animationstate = STOP;
		LUCY_UUID selected_animation = LUCY_NULL_UUID;
		bool render_path = false;
		bool trace_path = false;

		int curr_idx;
		bool temp_change = false;

		void Step();
		bool IsNamePresent(std::string name);
		std::string GetName(std::string name);
		LUCY_UUID NewAnimation(std::string name, AnimationProperty animation, LUCY_UUID id = LUCY_GENERATE_UUID);
	};
}
