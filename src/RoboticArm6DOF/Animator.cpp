#include "Animator.h"
#include "Kinematics.h"
#include <Lucy/Ray.h>
#include <assert.h>

bool lra::Animator::IsNamePresent(std::string name) {
	for (auto& pair: animation_registry) {
		if (pair.second.name == name) {
			return true;
		}
	}

	return false;
}

std::string lra::Animator::GetName(std::string name) {
	std::string result = name;

	for (int i = 1; IsNamePresent(result); i++) {
		result = name + " (" + std::to_string(i) + ")";
	}

	return result;
}

LUCY_UUID lra::Animator::NewAnimation(std::string name, AnimationProperty animation, LUCY_UUID id) {
	assert(animation_registry.find(id) == animation_registry.end());

	if (name == "")
		name = "New Animation";

	name = GetName(name);

	animation_registry[id] = { name, animation };

	return id;
}

void lra::AnimationProperty::Generate() {
	generated_positions.clear();
	if (step_array.size() == 0) return;

	generated_positions.push_back(step_array[0]);

	for (int i = 1; i < step_array.size(); i++) {
		auto& step = step_array[i];

		lucy::RayCastPoint(generated_positions.back().position, step.target_position, [&](int x, int y, int z) -> bool {
			bool is_valid;
			Kinematics::GetInverseKinematics(is_valid, { x, y, z });
			if (is_valid) {
				generated_positions.push_back({{ x, y, z }, step.target_angles.gripper_rotate, step.target_angles.gripper_control });
			}
			return false;
		});
	}
}

const std::vector<lra::Position>& lra::AnimationProperty::GetGenerated() {
	if (generated_positions.size() == 0)
		Generate();
	return generated_positions;
}

void lra::Animator::Step() {
	// static Position prev_position;
	// static float progress = 0;
	// static int idx = 0;

	// if (animation == nullptr) {
	// 	prev_position = position;

	// 	return;
	// }

	// if (animation_status == PAUSE) {
	// 	progress = 0;
	// 	prev_position = position;

	// 	return;
	// }

	// if (this->animation_status == PLAY && this->animation->animation_step_array.size() > 0) {
	// 	this->ik_enable = false;

	// 	if (progress >= 1) {
	// 		for (int i = 0; i < 6; i++)
	// 			prev_position[i] = this->position[i];

	// 		idx++;
	// 		progress = 0;
	// 	} else {
	// 		for (int i = 0; i < 6; i++)
	// 			this->position[i] = prev_position[i] + (this->animation->animation_step_array[idx].position[i] - prev_position[i]) * EaseFunc(progress, animation->animation_step_array[idx].pow_t);

	// 		progress += (1 / animation->animation_step_array[idx].progress_len);
	// 	}
	// } else if (this->animation_status != PAUSE) {
	// 	for (int i = 0; i < 6; i++)
	// 		prev_position[i] = this->position[i];
	// }

	// if (idx >= this->animation->animation_step_array.size()) {
	// 	if (!animation->loop) {
	// 		this->animation_status = STOP;
	// 	} else {
	// 		idx = 0;
	// 	}
	// }

	// // if (this->animation_status == STOP) {
	// // 	idx = 0;
	// // 	progress = 0;
	// // 	prev_position = position;
	// // }
}