#pragma once

#include <LucyGL/LucyGL.h>
#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambient = { 1, 1, 1 };
	glm::vec3 diffuse = { 1, 1, 1 };
	glm::vec3 specular = { 0.508273, 0.508273, 0.508273 };
	float shininess = 32;

	Material() {}
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess): ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
	void Bind(lgl::Shader* shader);
};