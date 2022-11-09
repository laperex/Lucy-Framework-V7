#include "Transform.h"
#include <iostream>
#include <Lucy/Math.h>//#include <glm/gtx/quaternion.hpp>
// #include <Lucy/Math.h>//#include <glm/gtx/string_cast.hpp>

glm::mat4 lucy::Transform::GetTranslationMatrix() {
	// return -glm::translate(glm::mat4(1.0f), glm::normalize(parent_entity->GetRotationQuat() * glm::normalize(translation - parent_entity->translation)) * glm::length(translation - parent_entity->translation));
	return glm::translate(glm::mat4(1.0f), translation);
}

glm::mat4 lucy::Transform::GetScaleMatrix() {
	return glm::scale(glm::mat4(1.0f), scale);
}

glm::quat lucy::Transform::GetRotationQuat() {
	return glm::quat(glm::radians(rotation));
}

glm::mat4 lucy::Transform::GetRotationMatrix() {
	return glm::toMat4(GetRotationQuat());
}

glm::mat4 lucy::Transform::GetModelMatrix() {
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

void lucy::Transform::Clear() {
	translation = glm::vec3(0, 0, 0);
	scale = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
}
