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

	std::vector<glm::ivec3> positions;

	for (int i = 0; i < step_array.size(); i++) {
		auto& step = step_array[i];

		if (positions.size() == 0) {
			positions.push_back(step.target_position);
		} else {
			lucy::RayLine3DPoints(positions, positions.back(), step.target_position);
		}
	}

	for (int i = 0; i < positions.size(); i++) {
		bool is_valid;
		auto angles = Kinematics::GetInverseKinematics(is_valid, positions[i]);
		if (is_valid)
			generated.push_back(angles);
	}
}