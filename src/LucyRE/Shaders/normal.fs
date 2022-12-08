#version 450 core

in vec3 normal;
layout (location = 0) out vec4 FragColor_0;

void main() {
	vec3 result = normal;

	if (normal.x < 0)
		result.x = -normal.x;
	if (normal.y < 0)
		result.y = -normal.y;
	if (normal.z < 0)
		result.z = -normal.z;

	FragColor_0 = vec4(1);
}