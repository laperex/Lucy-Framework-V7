#include "Chunk.h"

void lpv::AddBlock(Chunk* chunk, const glm::ivec3& position, const block_type block_id) {
	assert(chunk != nullptr);

	chunk->blocks[CHUNK_INDEX(position.x, position.y, position.z)] = block_id;
}

lpv::block_type lpv::GetBlock(Chunk* chunk, const glm::ivec3& position) {
	assert(chunk != nullptr);

	return chunk->blocks[CHUNK_INDEX(position.x, position.y, position.z)];
}

void lpv::MeshChunk(Chunk* chunk, Chunk* front, Chunk* back, Chunk* top, Chunk* bottom, Chunk* left, Chunk* right) {
	for (int z = 0; z < CHUNK_SIZE; z++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				
			}
		}
	}
}
