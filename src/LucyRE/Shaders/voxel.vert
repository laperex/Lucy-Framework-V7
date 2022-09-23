#version 450 core

layout (location = 0) in uvec2 v_data;

layout (std140, binding = 0) uniform ProjectionMatrix {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec3 v_view_pos;
};

uniform vec3 offset;

out vec3 normal;
out vec3 uvw;

out vec3 frag_pos;
out vec3 view_pos;

flat out int vertexid;
flat out int instanceid;

void main() {
	vec3 v_pos = vec3(
		(v_data.x & 0xff000000) >> 24,
		(v_data.x & 0x00ff0000) >> 16,
		(v_data.x & 0x0000ff00) >> 8
	);

	uvw = vec3(
		(v_data.y & 0xff000000) >> 24,
		(v_data.y & 0x00ff0000) >> 16,
		(v_data.y & 0x0000ff00) >> 8
	);

	switch (v_data.x & 0x000000ff) {
		case 0:
			normal = vec3(0,  0, -1);
			break;
		case 1:
			normal = vec3(0,  0,  1);
			break;
		case 2:
			normal = vec3(0, -1,  0);
			break;
		case 3:
			normal = vec3(0,  1,  0);
			break;
		case 4:
			normal = vec3(1,  0,  0);
			break;
		case 5:
			normal = vec3(1,  0,  0);
			break;
	}

	frag_pos = vec3(model * vec4(v_pos + offset, 1.0));
	gl_Position = projection * view * vec4(frag_pos, 1.0);

	vertexid = gl_VertexID;
	instanceid = gl_InstanceID;

	view_pos = v_view_pos;
}
