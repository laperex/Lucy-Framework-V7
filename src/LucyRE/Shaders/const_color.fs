#version 450 core

uniform vec4 u_color;

flat in int vertexid;
flat in int instanceid;
uniform int data;

void main() {
	// gl_FragData[1] = vec4(1, float(data), float(gl_FragCoord.x), float(gl_FragCoord.y));

	gl_FragData[0] = u_color;
}