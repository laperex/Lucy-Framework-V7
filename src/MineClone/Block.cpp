#include "Block.h"

void lpv::FaceVertices(std::vector<Vertex>& vertices, VoxelFace face, const uint8_t x, const uint8_t y, const uint8_t z, const uint8_t w, const uint8_t h, const uint8_t d, const uint32_t tex) {
	vertices.reserve(6 + vertices.size());

	switch (face) {
		case BACK:
			vertices.emplace_back(Vertex{                x,                y, (uint8_t)(z + d), BACK, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h), (uint8_t)(z + d), BACK, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h), (uint8_t)(z + d), BACK, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h), (uint8_t)(z + d), BACK, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y, (uint8_t)(z + d), BACK, tex });
			vertices.emplace_back(Vertex{                x,                y, (uint8_t)(z + d), BACK, tex });
			break;

		case FRONT:
			vertices.emplace_back(Vertex{                x,                y,                z, FRONT, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h),                z, FRONT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h),                z, FRONT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h),                z, FRONT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y,                z, FRONT, tex });
			vertices.emplace_back(Vertex{                x,                y,                z, FRONT, tex });
			break;

		case TOP:
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h),                z, TOP, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h), (uint8_t)(z + d), TOP, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h), (uint8_t)(z + d), TOP, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h), (uint8_t)(z + d), TOP, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h),                z, TOP, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h),                z, TOP, tex });
			break;

		case BOTTOM:
			vertices.emplace_back(Vertex{                x,                y,                z, BOTTOM, tex });
			vertices.emplace_back(Vertex{                x,                y, (uint8_t)(z + d), BOTTOM, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y, (uint8_t)(z + d), BOTTOM, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y, (uint8_t)(z + d), BOTTOM, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y,                z, BOTTOM, tex });
			vertices.emplace_back(Vertex{                x,                y,                z, BOTTOM, tex });
			break;

		case LEFT:
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y, (uint8_t)(z + d), LEFT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h), (uint8_t)(z + d), LEFT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h),                z, LEFT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w), (uint8_t)(y + h),                z, LEFT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y,                z, LEFT, tex });
			vertices.emplace_back(Vertex{ (uint8_t)(x + w),                y, (uint8_t)(z + d), LEFT, tex });
			break;

		case RIGHT:
			vertices.emplace_back(Vertex{                x,                y, (uint8_t)(z + d), RIGHT, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h), (uint8_t)(z + d), RIGHT, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h),                z, RIGHT, tex });
			vertices.emplace_back(Vertex{                x, (uint8_t)(y + h),                z, RIGHT, tex });
			vertices.emplace_back(Vertex{                x,                y,                z, RIGHT, tex });
			vertices.emplace_back(Vertex{                x,                y, (uint8_t)(z + d), RIGHT, tex });
			break;
	}
}
