#pragma once

#include <LucyGL/LucyGL.h>

// |  7  | |  7  | |  7  | |  3  | |  8  |	# SIZE IN BYTES
// |pos.x| |pos.y| |pos.z| |normv| |texid|	# ATTRIBUTE

// NORMALS_ID - 0 -> {  0,  0, -1 } => BACK
// NORMALS_ID - 1 -> {  0,  0,  1 } => FRONT
// NORMALS_ID - 2 -> {  0, -1,  0 } => BOTTOM
// NORMALS_ID - 3 -> {  0,  1,  0 } => TOP
// NORMALS_ID - 4 -> { -1,  0,  0 } => LEFT
// NORMALS_ID - 5 -> {  1,  0,  0 } => RIGHT

// UV_ID - 0 -> { 1, 1 }
// UV_ID - 1 -> { 1, 0 }
// UV_ID - 2 -> { 0, 0 }
// UV_ID - 3 -> { 0, 0 }
// UV_ID - 4 -> { 0, 1 }
// UV_ID - 5 -> { 1, 1 }

namespace lpv {
	struct Vertex {
		uint32_t pos_norm = 0;

		Vertex() {}
		Vertex(uint8_t posx, uint8_t posy, uint8_t posz, uint8_t norm, uint8_t uvwt) {
			pos_norm = uvwt;
			pos_norm |= norm << 8;
			pos_norm |= posz << 8 + 3 + 7 * 0;
			pos_norm |= posy << 8 + 3 + 7 * 1;
			pos_norm |= posx << 8 + 3 + 7 * 2;
		}

		static lgl::VertexArray* VertexArray() {
			return GetVertexArray(false);
		}

		static lgl::VertexArray* FreeVertexArray() {
			return GetVertexArray(true);
		}

		static lgl::VertexArray* GetVertexArray(bool free) {
			static lgl::VertexArray* instance = nullptr;

			if (free) {
				if (instance != nullptr) {
					delete instance;
					instance = nullptr;
				}
			} else if (instance == nullptr) {
				instance = new lgl::VertexArray({
					{ 0, 1, lgl::UNSIGNED_INT },
				});
			}

			return instance;
		}
	};
}