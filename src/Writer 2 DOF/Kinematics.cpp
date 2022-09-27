#include "Kinematics.h"

#define TO_DEGREE (180/3.14159265359)

float Clamp(float val, float min = 0, float max = 180) {
	if (val > max) val = max;
	if (val < min) val = min;
	return val;
}

arm::Position Clamp(arm::Position position) {
	for (int i = 0; i < 2; i++)
		((float*)&position)[i] = Clamp(((float*)&position)[i]);

	return position;
}

arm::Position arm::GetInverseKinematics(glm::vec2 target, float lower, float upper) {
	float a = lower;
	float b = upper;

	auto [x, y] = target;

	// float base_angle = atan2(x, y) * TO_DEGREE;
	// float l = sqrt(x * x + y * y);

	// float A = acos((b * b + l * l - a * a) / (2 * b * l)) * TO_DEGREE;
	// float B = acos((l * l + a * a - b * b) / (2 * l * a)) * TO_DEGREE;

	float c = sqrt(x * x + y * y);

	float A = acos((b * b + c * c - a * a) / (2 * b * c)) * TO_DEGREE;
	float B = acos((c * c + a * a - b * b) / (2 * c * a)) * TO_DEGREE;
	float C = acos((a * a + b * b - c * c) / (2 * a * b)) * TO_DEGREE;

	float theta = atan2(y, x) * TO_DEGREE;

	return Clamp({ B + theta, C });
}

