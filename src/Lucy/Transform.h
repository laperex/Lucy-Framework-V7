#pragma once

#include <Lucy/Math.h>//#include <glm/glm.hpp>
#include <Lucy/Math.h>//#include <glm/gtc/matrix_transform.hpp>
#include <Lucy/Math.h>//#include <glm/gtc/quaternion.hpp>

namespace lucy {
	struct Transform {
		glm::vec3 translation = { 0, 0, 0 };
		glm::vec3 rotation = { 0, 0, 0 };
		glm::vec3 scale = { 1, 1, 1 };

		Transform() {}
		Transform(const glm::vec3& translation): translation(translation) {}

		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetScaleMatrix();
		glm::quat GetRotationQuat();
		glm::mat4 GetRotationMatrix();
		glm::mat4 GetModelMatrix();

		void Clear();
	};
}