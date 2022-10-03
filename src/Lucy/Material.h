#pragma once

#include <LucyGL/LucyGL.h>
#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambient = { 0.06, 0.50, 1.00 };
	glm::vec3 diffuse = { 0.77, 1.00, 1.00 };
	glm::vec3 specular = { 0.508, 0.508, 0.508 };
	float shininess = 32;

	Material() {}
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess): ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
	void Bind(lgl::Shader* shader);
};