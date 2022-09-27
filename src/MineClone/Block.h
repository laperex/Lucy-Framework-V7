#pragma once

#include "VoxelVertex.h"

namespace lpv {
	typedef uint8_t block_type;

	enum BLOCK_TYPE {
		SOLID,
		TRANSPARENT,

		BLOCK_TYPE_COUNT
	};

	struct Block {
		char* name = nullptr;
		int face = 0;
		int back = 0;
		int top = 0;
		int bottom = 0;
		int left = 0;
		int right = 0;
	};

	void RegisterBlock();

	void FaceVertices(std::vector<Vertex>& vertices, VoxelFace face, const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t w, const uint8_t h, const uint8_t d, const uint32_t tex);
};