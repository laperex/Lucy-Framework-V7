#pragma once

#include <LucyGL/LucyGL.h>
#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambient = { 1, 1, 1 };
	glm::vec3 diffuse = { 1, 1, 1 };
	glm::vec3 specular = { 0.508273, 0.508273, 0.508273 };
	float shininess = 32;

	Material() {}
	void Bind(lgl::Shader* shader);
};