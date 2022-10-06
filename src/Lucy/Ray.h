#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace lucy {
	void RayLine3DPoints(std::vector<glm::ivec3>&positions, int startX, int startY, int startZ, int endX, int endY, int endZ);
	void RayLine3DPoints(std::vector<glm::ivec3>&positions, const glm::ivec3& start, const glm::ivec3& end);
}