#version 450 core

uniform sampler2D u_texture;
in vec2 uv;

layout (location = 0) out vec4 FragColor_0;

void main() {
	FragColor_0 = texture(u_texture, uv);
}