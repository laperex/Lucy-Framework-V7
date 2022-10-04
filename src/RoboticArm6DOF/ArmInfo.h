#pragma once

#include <glm/glm.hpp>

namespace lra {
	struct ArmInfo {
		glm::vec3 J0;
		glm::vec3 J1;
		glm::vec3 J2;
		glm::vec3 J3;
		glm::vec3 J4;
		glm::vec3 J5;
	};
};