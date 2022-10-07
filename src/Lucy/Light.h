#pragma once

#include <LucyGL/LucyGL.h>
#include <glm/glm.hpp>

struct Light {
	float ambient = 0.2f;
	float diffuse = 0.5f;
	float specular = 1.0f;

	glm::vec4 color = glm::vec4(1.0);
	glm::vec4 pos = glm::vec4(1, 1, 0, 0);

	Light() {}
	void Bind(lgl::Shader* shader);
};