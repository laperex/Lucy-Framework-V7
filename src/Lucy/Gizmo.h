#pragma once

#include <LucyRE/LucyRE.h>

namespace lucy {
	struct Gizmo {
		UTIL_UUID x_gimbal_transform = UTIL_NULL_UUID;
		UTIL_UUID x_gimbal_scale = UTIL_NULL_UUID;
		UTIL_UUID x_gimbal_rotation = UTIL_NULL_UUID;

		UTIL_UUID y_gimbal_transform = UTIL_NULL_UUID;
		UTIL_UUID y_gimbal_scale = UTIL_NULL_UUID;
		UTIL_UUID y_gimbal_rotation = UTIL_NULL_UUID;

		UTIL_UUID z_gimbal_transform = UTIL_NULL_UUID;
		UTIL_UUID z_gimbal_scale = UTIL_NULL_UUID;
		UTIL_UUID z_gimbal_rotation = UTIL_NULL_UUID;

		UTIL_UUID xy_gimbal_transform = UTIL_NULL_UUID;
		UTIL_UUID yz_gimbal_transform = UTIL_NULL_UUID;
		UTIL_UUID xz_gimbal_transform = UTIL_NULL_UUID;

		glm::vec3 Transform(glm::vec3 pos, bool is_mouse_focused);
		void Scale();
		void Rotation();
	};
}