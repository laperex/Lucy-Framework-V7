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

		SelectedBall_COUNT
	};

	struct Sorting {
		std::array<LUCY_UUID, SelectedBall_COUNT> animations_ids;

		glm::ivec3 RED_PlacePosition = { 0, 100, 200 };
		glm::ivec3 GREEN_PlacePosition = { 0, 100, -200 };

		Sorting();
		void Initialize();
		void SetParameters(SelectedBall ball, glm::ivec2 pos);
	};
}
