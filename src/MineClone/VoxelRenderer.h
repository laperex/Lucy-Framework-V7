#pragma once

#include "VoxelVertex.h"

namespace lpv {
	void DrawVoxel(uint8_t posx, uint8_t posy, uint8_t posz, VoxelFace face, int tex);

	void VoxelVertexTest();
}