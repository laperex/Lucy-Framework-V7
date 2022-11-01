#pragma once

#include <RoboticArm6DOF/Animator.h>
#include <unordered_map>
#include <array>

namespace lra {
	enum SelectedBall {
		NONE = -1,
		RED,
		GREEN,

		SelectedBall_COUNT
	};

	struct Sorting {
		std::array<AnimationProperty, SelectedBall_COUNT> animation_storage;

		glm::ivec3 RED_PlacePosition = { 0, 100, 200 };
		glm::ivec3 GREEN_PlacePosition = { 0, 100, -200 };

		Sorting();
		void SetParameters(SelectedBall ball, glm::ivec2 pos);
	};
}
