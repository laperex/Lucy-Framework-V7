#pragma once

namespace lgl {
	struct Status {
		int drawcount = 0;

		int bound_vbo = 0;
		int bound_fbo = 0;
		int bound_shader = 0;
		int bound_ibo = 0;
		int bound_vao = 0;
		int bound_ubo = 0;

		int vbo_count = 0;
		int fbo_count = 0;
		int shader_count = 0;
		int ibo_count = 0;
		int vao_count = 0;
		int ubo_count = 0;
	};

	Status& GetStatus();
}