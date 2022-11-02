#pragma once

#include <RoboticArm6DOF/Animator.h>
#include <unordered_map>
#include <array>
#include <LucyUtil/UUID.h>

namespace lra {
	enum SelectedBall {
		NONE = -1,
		RED,
		GREEN,
		BLUE,

		SelectedBall_COUNT
	};

	struct Sorting {
		LUCY_UUID animations_id = LUCY_NULL_UUID;

		glm::ivec3 RED_PlacePosition = { 0, 100, 200 };
		glm::ivec3 GREEN_PlacePosition = { 0, 100, -200 };

		Sorting();
		void Initialize();
		void SetParameters(SelectedBall ball, glm::ivec2 pos);
	};
}
