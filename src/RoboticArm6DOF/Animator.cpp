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
	generated.clear();

	std::vector<std::tuple<glm::ivec3, glm::vec2>> positions;

	for (int i = 0; i < step_array.size(); i++) {
		auto& step = step_array[i];

		if (positions.size() == 0) {
			positions.push_back({ step.target_position, { step.target_angles.gripper_rotate, step.target_angles.gripper_control }});
		} else {
			auto [pos, gripper] = positions.back();
			std::vector<glm::ivec3> targets;
			lucy::RayLine3DPoints(targets, pos, step.target_position);

			for (const auto& target_pos: targets)
				positions.push_back(std::tuple<glm::ivec3, glm::vec2>(target_pos, gripper));
		}
	}

	for (int i = 0; i < positions.size(); i++) {
		bool is_valid;
		auto [pos, gripper] = positions[i];
		auto angles = Kinematics::GetInverseKinematics(is_valid, pos);
		if (is_valid)
			generated.push_back(angles);
	}
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