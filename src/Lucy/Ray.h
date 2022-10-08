#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace lucy {
	typedef std::function<bool(int, int, int)> RAYCAST_FUNCTION;

	void RayCastPoint(int startX, int startY, int startZ, int endX, int endY, int endZ, RAYCAST_FUNCTION function);
	void RayCastPoint(const glm::ivec3& start, const glm::ivec3& end, RAYCAST_FUNCTION function);
	void RayCastDirection(const glm::ivec3& origin, const glm::vec3& direction, const float& distance, bool normalize, RAYCAST_FUNCTION function);
}