#include "Ray.h"

void lucy::RayLine3DPoints(std::vector<glm::ivec3>&positions, int startX, int startY, int startZ, int endX, int endY, int endZ) {
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

void lucy::RayLine3DPoints(std::vector<glm::ivec3>&positions, const glm::ivec3& start, const glm::ivec3& end) {
	RayLine3DPoints(positions, start.x, start.y, start.z, end.x, end.y, end.z);
}

const std::vector<glm::ivec3>& lucy::RayLine3DPoints(const glm::ivec3& start, const glm::ivec3& end) {
	std::vector<glm::ivec3> positions;
	RayLine3DPoints(positions, start, end);
	return positions;
}
