#pragma once

#include <LucyRE/LucyRE.h>

namespace lucy {
	struct Gizmo {
		LUCY_UUID x_gimbal_transform = LUCY_NULL_UUID;
		LUCY_UUID x_gimbal_scale = LUCY_NULL_UUID;
		LUCY_UUID x_gimbal_rotation = LUCY_NULL_UUID;

		LUCY_UUID y_gimbal_transform = LUCY_NULL_UUID;
		LUCY_UUID y_gimbal_scale = LUCY_NULL_UUID;
		LUCY_UUID y_gimbal_rotation = LUCY_NULL_UUID;

		LUCY_UUID z_gimbal_transform = LUCY_NULL_UUID;
		LUCY_UUID z_gimbal_scale = LUCY_NULL_UUID;
		LUCY_UUID z_gimbal_rotation = LUCY_NULL_UUID;

		LUCY_UUID xy_gimbal_transform = LUCY_NULL_UUID;
		LUCY_UUID yz_gimbal_transform = LUCY_NULL_UUID;
		LUCY_UUID xz_gimbal_transform = LUCY_NULL_UUID;

		glm::vec3 Transform(glm::vec3 pos, bool is_mouse_focused);
		void Scale();
		void Rotation();
	};
}