#include "Sorting.h"
#include <Lucy/Lucy.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// Rest = { 180, 300, 0 }

static auto& registry = Registry::Instance();

#define REST_POS_X (180)
#define REST_POS_Y (200)
#define REST_POS_Z (0)
#define REST_POS (glm::ivec3(REST_POS_X, REST_POS_Y, REST_POS_Z))

enum MovementSteps {
	S0_REST_POS = 0,
	S1_PICK_UPPER_GO = 1,
	S2_PICK_LOWER_GO = 2,
	S3_PICK_LOWER_GC = 3,
	S4_PICK_UPPER_GC = 4,
	S5_PLACE_UPPER_GC = 5,
	S6_PLACE_LOWER_GC = 6,
	S7_PLACE_LOWER_GO = 7,
	S8_PLACE_UPPER_GO = 8,
	S9_REST_POS = 9,

	MovementSteps_COUNT
};

lra::Sorting::Sorting() {
	
}

void lra::Sorting::Initialize() {
	for (int i = 0; i < SelectedBall_COUNT; i++) {
		AnimationProperty animation;

		for (int n = 0; n < MovementSteps_COUNT; n++) {
			animation.step_array.push_back({});
			animation.step_array[n].target_angles.gripper_rotate = 180;
		}

		animation.step_array[S0_REST_POS].target_position = REST_POS;
		animation.step_array[S9_REST_POS].target_position = REST_POS;

		animation.step_array[S0_REST_POS].target_angles.gripper_control = 0;
		animation.step_array[S1_PICK_UPPER_GO].target_angles.gripper_control = 0;
		animation.step_array[S2_PICK_LOWER_GO].target_angles.gripper_control = 0;
		animation.step_array[S3_PICK_LOWER_GC].target_angles.gripper_control = 50;
		animation.step_array[S4_PICK_UPPER_GC].target_angles.gripper_control = 50;
		animation.step_array[S5_PLACE_UPPER_GC].target_angles.gripper_control = 50;
		animation.step_array[S6_PLACE_LOWER_GC].target_angles.gripper_control = 50;
		animation.step_array[S7_PLACE_LOWER_GO].target_angles.gripper_control = 0;
		animation.step_array[S8_PLACE_UPPER_GO].target_angles.gripper_control = 0;
		animation.step_array[S9_REST_POS].target_angles.gripper_control = 0;

		animations_ids[i] = registry.store<Animator>().NewAnimation("__##Sorting_" + std::to_string(i), animation);
	}
}

void lra::Sorting::SetParameters(SelectedBall ball, glm::ivec2 pos) {
	if (ball < 0) return;

	auto& animation = registry.store<Animator>().animation_registry[animations_ids[ball]].animation;

	animation.step_array[S1_PICK_UPPER_GO].target_position = { pos.x, REST_POS_Y, pos.y };
	animation.step_array[S2_PICK_LOWER_GO].target_position = { pos.x, 30, pos.y };
	animation.step_array[S3_PICK_LOWER_GC].target_position = { pos.x, 30, pos.y };
	animation.step_array[S4_PICK_UPPER_GC].target_position = { pos.x, REST_POS_Y, pos.y };
	animation.step_array[S5_PLACE_UPPER_GC].target_position = { 0, REST_POS_Y, 200 };
	animation.step_array[S6_PLACE_LOWER_GC].target_position = { 0, 30, 200 };
	animation.step_array[S7_PLACE_LOWER_GO].target_position = { 0, 30, 200 };
	animation.step_array[S8_PLACE_UPPER_GO].target_position = { 0, REST_POS_Y, 200 };

	for (int i = 0; i < MovementSteps_COUNT; i++) {
		float g_rot = animation.step_array[i].target_angles.gripper_rotate;
		float g_ctrl = animation.step_array[i].target_angles.gripper_control;
		bool is_valid;
		animation.step_array[i].target_angles = Kinematics::GetInverseKinematics(is_valid, animation.step_array[i].target_position);
		std::cout << glm::to_string(animation.step_array[i].target_position) << '\n';
		assert(is_valid);
		animation.step_array[i].target_angles.gripper_rotate = g_rot;
		animation.step_array[i].target_angles.gripper_control = g_ctrl;
	}
}
