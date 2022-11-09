#pragma once

#include <LucyGL/LucyGL.h>
#include <array>
#include <Lucy/Math.h>//#include <glm/glm.hpp>
#include "Block.h"

#define CHUNK_SIZE (16)
#define CHUNK_INDEX(x, y, z) (((z) * CHUNK_SIZE * CHUNK_SIZE) + ((y) * CHUNK_SIZE) + (x))
#define IS_VALID_CHUNK_INDEX(x, y, z) (x >= 0 && y >= 0 && z >= 0 && CHUNK_INDEX(x, y, z) < (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE))
#define IS_VALID_CHUNK_INDEX(chunk_idx) (chunk_idx >= 0 && chunk_idx < (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE))

namespace lpv {
	struct Chunk {
		glm::ivec3 chunk_id = { 0, 0, 0};
		block_type blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE] = { 0 };

		lgl::VertexBuffer* vertexbuffer[BLOCK_TYPE_COUNT] = { nullptr };
	};

	void AddBlock(Chunk* chunk, const glm::ivec3& position, const block_type block_id);
	block_type GetBlock(Chunk* chunk, const glm::ivec3& position);

	void MeshChunk(Chunk* chunk, Chunk* front, Chunk* back, Chunk* top, Chunk* bottom, Chunk* left, Chunk* right);
}
