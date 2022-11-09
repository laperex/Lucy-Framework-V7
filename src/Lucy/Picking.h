#pragma once

#include <Lucy/Math.h>//#include <glm/glm.hpp>
#include <LucyGL/LucyGL.h>

namespace lucy {
	struct Picking {
		glm::vec4 pixel = { 0, 0, 0, 0 };
		bool is_present = false;
		uint32_t data = 0;
		uint32_t vertexid = 0;
		uint32_t instanceid = 0;
		glm::vec2 pos = { 0, 0 };

		void Update();
		static glm::vec3 GetWorldPos(glm::vec2 mouse_pos, glm::vec2 window_size, glm::vec3 camera_position, glm::vec3 front, glm::mat4 view, glm::mat4 projection);
	};
}