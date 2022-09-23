#include "Game.h"
#include "World.h"
#include <iostream>
#include <LucyRE/LucyRE.h>
#include <Lucy/Camera.h>
#include <Lucy/ECS.h>

static auto& registry = Registry::Instance();

// |pos.x| |pos.y| |pos.z| |norm |	-> x
// |uvw.0| |uvw.1| |uvw.t| |     |	-> y

// norm - 0 -> {  0,  0, -1 }
// norm - 1 -> {  0,  0,  1 }
// norm - 2 -> {  0, -1,  0 }
// norm - 3 -> {  0,  1,  0 }
// norm - 4 -> { -1,  0,  0 }
// norm - 5 -> {  1,  0,  0 }

struct Vertex {
	union {
		struct {
			uint8_t pos_x;
			uint8_t pos_y;
			uint8_t pos_z;
			uint8_t normv;
		};

		uint32_t pos_norm;
	};

	union {
		struct {
			uint8_t uvw_0;
			uint8_t uvw_1;
			uint8_t uvw_t;
		};

		uint32_t uvw;
	};

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
				{ 0, 2, lgl::UNSIGNED_INT },
			});
		}

		return instance;
	}
};

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

	lre::Test();
}
