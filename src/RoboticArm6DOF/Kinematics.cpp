#include "Kinematics.h"
#include <array>

#define TO_DEGREE (180/3.14159265359)
#define TO_RADIAN (3.14159265359/180)
#define PI (3.141592653589793238462)
#define HALF_PI (3.141592653589793238462/2)

static double epsilon = 0.0001;

float Clamp(float val, float min = 0, float max = 180) {
	if (val > max) val = max;
	if (val < min) val = min;
	return val;
}

lra::JointAngles Clamp(lra::JointAngles position) {
	for (int i = 0; i < 6; i++)
		((float*)&position)[i] = Clamp(((float*)&position)[i]);

	return position;
}

const glm::vec3& lra::Kinematics::GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions) {
	return { 0, 0, 0 };
}

std::pair<bool, lra::JointAngles> lra::Kinematics::GetInverseKinematics(const glm::vec3& target, const JointLength& lra_dimensions) {
	float l1 = lra_dimensions.arm;
	float l2 = lra_dimensions.elbow;
	float l3 = lra_dimensions.wrist;

	auto [x, y, z] = target;

	float base_angle = atan2(x, z) * TO_DEGREE;
	float l = sqrt(x * x + z * z);

	float c = sqrt(l * l + y * y);
	float f = c - l1;

	float A = acos((l1 * l1 + f * f - l3 * l3) / (2 * l1 * f)) * TO_DEGREE;
	float B = acos((f * f + l3 * l3 - l1 * l1) / (2 * f * l3)) * TO_DEGREE;
	float C = acos((l3 * l3 + l1 * l1 - f * f) / (2 * l3 * l1)) * TO_DEGREE;

	float theta = atan2(y, l) * TO_DEGREE;

	auto angles = Clamp({ base_angle, A + theta, C + B, 180 - B, 0, 0 });

	bool is_valid = true;
	for (int i = 0; i < 6; i++) {
		if ((angles[i] != angles[i])) {
			is_valid = false;
			break;
		}
	}

	return { is_valid, angles };
}
