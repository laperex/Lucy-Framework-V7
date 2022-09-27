#include "Game.h"
#include "World.h"
#include <iostream>
#include <LucyRE/LucyRE.h>
#include <Lucy/Camera.h>
#include <Lucy/ECS.h>
#include "VoxelVertex.h"
#include "VoxelRenderer.h"

static auto& registry = Registry::Instance();

static lpv::Chunk* chunk = nullptr;

void lpv::InitializeGame() {
	auto& camera = registry.store<lucy::Camera>();

	camera.position = { 0, 0, 10 };
	camera.c_far = 100;
	camera.c_near = 0.1;

	chunk = new Chunk();

	for (int z = 0; z < CHUNK_SIZE; z++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				chunk->blocks[CHUNK_INDEX(x, y, z)] = 1;
			}
		}
	}
}

void lpv::RuntimeGame() {
	auto& camera = registry.store<lucy::Camera>();

	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	VoxelVertexTest();
}
