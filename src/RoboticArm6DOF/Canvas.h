#pragma once

#include <LucyRE/LucyRE.h>
#include <glm/glm.hpp>

namespace lra {
	struct Canvas {
		glm::ivec3 position = { 150, 0, 150 };
		glm::ivec3 scale = { 100, 100, 100 };

		bool visible = false;

		void Render(const glm::vec4& color = { 1, 0, 1, 1});
		const glm::ivec2& GetOriginPos();
		const glm::ivec2& GetCanvasSize();
		const glm::ivec3& GetArmRestPos();
	};
}

