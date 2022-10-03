#include "Picking.h"
#include <Lucy/Events.h>

void lucy::Picking::Update() {
	
}

glm::vec3 lucy::Picking::GetWorldPos(glm::vec2 mouse_pos, glm::vec2 window_size, glm::vec3 camera_position, glm::vec3 front, glm::mat4 view, glm::mat4 projection) {
	// auto norm_cursor_pos = Events::GetCursorPosNormalized(mouse_pos.x, mouse_pos.y, window_size.x, window_size.y);
	auto norm_cursor_pos = (lucy::Events::GetCursorPosNormalized(0, 0, mouse_pos.x, mouse_pos.y) * glm::vec3(mouse_pos.x, mouse_pos.y, 0) + glm::vec3(mouse_pos.x, mouse_pos.y, 0)) / 2.0f;

	glm::vec4 ray_clip = glm::vec4(norm_cursor_pos.x, norm_cursor_pos.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	glm::vec3 ray_wor = glm::normalize(glm::inverse(view) * glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0));

	float denom = glm::dot(front, ray_wor);
	float t = -glm::dot((front * glm::length(camera_position)), front) / denom;

	return ray_wor * t + camera_position;
}

