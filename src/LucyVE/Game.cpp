#include "Game.h"
#include <Lucy/Lucy.h>
#include <LucyRE/LucyRE.h>

static auto& registry = Registry::Instance();

void lve::InitializeGame() {
	auto& camera = registry.store<lucy::Camera>();

	camera.position = { 0, 0, 10 };
	camera.c_far = 100;
	camera.c_near = 0.1;
}

void lve::RuntimeGame() {
	auto& camera = registry.store<lucy::Camera>();

	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	// lre::Test();
}
