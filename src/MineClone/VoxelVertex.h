#pragma once

#include <LucyGL/LucyGL.h>

// |  4  | |  4  | |  4  | |  4  | |  4  | |  3  | |  8  |	# SIZE IN BYTES
// |pos.x| |pos.y| |pos.z| |uvw.u| |uvw.u| |normv| |texid|	# ATTRIBUTE

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
	enum VoxelFace {
		BACK,
		FRONT,
		BOTTOM,
		TOP,
		LEFT,
		RIGHT,

		VoxelFace_COUNT
	};

	#define LPV_VERTEX_POSITION_X(vertex) ((vertex.pos_norm_tex & (0xf << 12 + 4 * 4)) >> 12 + 4 * 4)
	#define LPV_VERTEX_POSITION_Y(vertex) ((vertex.pos_norm_tex & (0xf << 12 + 4 * 3)) >> 12 + 4 * 3)
	#define LPV_VERTEX_POSITION_Z(vertex) ((vertex.pos_norm_tex & (0xf << 12 + 4 * 2)) >> 12 + 4 * 2)
	#define LPV_VERTEX_UV_U(vertex) ((vertex.pos_norm_tex & (0xf << 12 + 4 * 1)) >> 12 + 4 * 1)
	#define LPV_VERTEX_UV_V(vertex) ((vertex.pos_norm_tex & (0xf << 12 + 4 * 0)) >> 12 + 4 * 0)
	#define LPV_VERTEX_NORMAL(vertex) ((vertex.pos_norm_tex & (0x3 << 8)) >> 8)
	#define LPV_VERTEX_TEX_ID(vertex) (vertex.pos_norm_tex & 0x1ff)

	struct Vertex {
		uint32_t pos_norm_tex = 0;

		Vertex() {}
		Vertex(const uint8_t posx, const uint8_t posy, const uint8_t posz, const uint8_t norm, const int uvwt) {
			pos_norm_tex = uvwt & 0x1ff;

			pos_norm_tex |= ((norm & 0x3) << 9);
			pos_norm_tex |= ((1 & 0xf) << 12 + 4 * 0);
			pos_norm_tex |= ((1 & 0xf) << 12 + 4 * 1);
			pos_norm_tex |= ((posz & 0xf) << 12 + 4 * 2);
			pos_norm_tex |= ((posy & 0xf) << 12 + 4 * 3);
			pos_norm_tex |= ((posx & 0xf) << 12 + 4 * 4);
		}

		Vertex(const uint8_t posx, const uint8_t posy, const uint8_t posz, const uint8_t u, const uint8_t v, const uint8_t norm, const int uvwt) {
			pos_norm_tex = uvwt & 0x1ff;

			pos_norm_tex |= ((norm & 0x3) << 9);
			pos_norm_tex |= ((v & 0xf) << 12 + 4 * 0);
			pos_norm_tex |= ((u & 0xf) << 12 + 4 * 1);
			pos_norm_tex |= ((posz & 0xf) << 12 + 4 * 2);
			pos_norm_tex |= ((posy & 0xf) << 12 + 4 * 3);
			pos_norm_tex |= ((posx & 0xf) << 12 + 4 * 4);
		}

		// void SetPositionX(const uint8_t posx) {
		// 	pos_norm_tex = (pos_norm_tex & ~(0x7f << 8 + 3 + 7 * 2)) | (posx << 8 + 3 + 7 * 2);
		// }
		// void SetPositionY(const uint8_t posy) {
		// 	pos_norm_tex = (pos_norm_tex & ~(0x7f << 8 + 3 + 7 * 1)) | (posy << 8 + 3 + 7 * 1);
		// }
		// void SetPositionZ(const uint8_t posz) {
		// 	pos_norm_tex = (pos_norm_tex & ~(0x7f << 8 + 3 + 7 * 0)) | (posz << 8 + 3 + 7 * 0);
		// }
		// void SetNormal(const uint8_t norm) {
		// 	pos_norm_tex = (pos_norm_tex & ~(0x3 << 8)) | (norm << 8);
		// }
		// void SetTexID(const uint8_t tex) {
		// 	pos_norm_tex = (pos_norm_tex & ~0xff) | tex;
		// }

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