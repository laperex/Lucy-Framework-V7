#version 450 core

flat in int vertexid;
flat in int instanceid;

uniform int data;

layout (location = 0) out vec4 FragColor_0;
layout (location = 1) out vec4 FragColor_1;

void main() {
	FragColor_1 = vec4(1, float(data), float(instanceid), float(vertexid));
	FragColor_0 = vec4(0);
}