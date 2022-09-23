#include "Game.h"
#include "World.h"
#include <iostream>
#include <LucyRE/LucyRE.h>
#include <Lucy/Camera.h>
#include <Lucy/ECS.h>
#include "VoxelVertex.h"
#include "VoxelRenderer.h"

static auto& registry = Registry::Instance();

void lpv::InitializeGame() {
	auto& camera = registry.store<lucy::Camera>();

	camera.position = { 0, 0, 10 };
	camera.c_far = 100;
	camera.c_near = 0.01;
}

void lpv::RuntimeGame() {
	auto& camera = registry.store<lucy::Camera>();

	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	VoxelVertexTest();
}
