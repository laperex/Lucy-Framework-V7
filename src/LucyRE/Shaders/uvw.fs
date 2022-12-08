#version 450 core

in vec3 uvw;

uniform sampler2DArray u_texture;
layout (location = 0) out vec4 FragColor_0;

void main() {
	FragColor_0 = texture(u_texture, uvw);
}