#include "Game.h"
#include <Lucy/Lucy.h>
#include <LucyRE/LucyRE.h>
#include <iostream>

static auto& registry = Registry::Instance();

void line3D(std::vector<glm::ivec3>&positions, int endX, int endY, int endZ, int startX, int startY, int startZ) {
	int x1 = endX, y1 = endY, z1 = endZ, x0 = startX, y0 = startY, z0 = startZ;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int dz = abs(z1 - z0);

	int stepX = x0 < x1 ? 1 : -1;
	int stepY = y0 < y1 ? 1 : -1;
	int stepZ = z0 < z1 ? 1 : -1;

	double hypotenuse = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

	double tMaxX = hypotenuse * 0.5 / dx;
	double tMaxY = hypotenuse * 0.5 / dy;
	double tMaxZ = hypotenuse * 0.5 / dz;

	double tDeltaX = hypotenuse / dx;
	double tDeltaY = hypotenuse / dy;
	double tDeltaZ = hypotenuse / dz;

	while (x0 != x1 || y0 != y1 || z0 != z1) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			} else if (tMaxX > tMaxZ) {
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			} else{
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			}
		} else if (tMaxX > tMaxY){
			if (tMaxY < tMaxZ) {
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
			} else if (tMaxY > tMaxZ){
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			} else{
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;

			}
		} else {
			if (tMaxY < tMaxZ) {
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
			} else if (tMaxY > tMaxZ) {
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;
			} else {
				x0 = x0 + stepX;
				tMaxX = tMaxX + tDeltaX;
				y0 = y0 + stepY;
				tMaxY = tMaxY + tDeltaY;
				z0 = z0 + stepZ;
				tMaxZ = tMaxZ + tDeltaZ;

			}
		}

		positions.push_back({ x0, y0, z0 });
	}
}

void lve::InitializeGame() {
	auto& camera = registry.store<lucy::Camera>();

	camera.position = { 0, 0, 10 };
	camera.c_far = 100;
	camera.c_near = 0.1;

	std::vector<glm::ivec3> positions;
	line3D(positions, 5, 3, 4, 0, 0, 0);

	for (int i = 0; i < positions.size(); i++) {
		std::cout << positions[i].x << ' ' << positions[i].y << ' ' << positions[i].z << '\n';
	}
}

void lve::RuntimeGame() {
	auto& camera = registry.store<lucy::Camera>();

	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	lre::Test();
}
