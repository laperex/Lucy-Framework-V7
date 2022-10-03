#include "Kinematics.h"
#include <array>
#include <iostream>

#define TO_DEGREE (180/3.14159265359)
#define TO_RADIAN (3.14159265359/180)
#define PI (3.141592653589793238462)
#define HALF_PI (3.141592653589793238462/2)

static double epsilon = 0.0001;

float Clamp(float val, float min = 0, float max = 180) {
	if (val >= max) val = max;
	if (val <= min) val = min;
	return val;
}

lra::JointAngles Clamp(lra::JointAngles position) {
	for (int i = 0; i < 6; i++)
		((float*)&position)[i] = Clamp(((float*)&position)[i]);

	return position;
}

const glm::vec3& lra::Kinematics::GetForwardKinematics(const JointAngles& joint_angles, const JointLength& lra_dimensions) {
	float l1 = lra_dimensions.arm;
	float l2 = lra_dimensions.elbow;
	float l3 = lra_dimensions.wrist;

	float phi_0 = joint_angles.arm;
	float phi_1 = phi_0 + joint_angles.elbow - 180;
	float phi_2 = phi_1 + joint_angles.wrist - 180;

	glm::vec2 P = { 0, 72 };
	glm::vec2 Q = P + glm::vec2(cos(phi_0 * TO_RADIAN), sin(phi_0 * TO_RADIAN)) * l1;
	glm::vec2 R = Q + glm::vec2(cos(phi_1 * TO_RADIAN), sin(phi_1 * TO_RADIAN)) * l2;
	glm::vec2 S = R + glm::vec2(cos(phi_2 * TO_RADIAN), sin(phi_2 * TO_RADIAN)) * l3;

	return { S.x * sin(joint_angles.base * TO_RADIAN), S.y, S.x * cos(joint_angles.base * TO_RADIAN)};
}

std::pair<bool, lra::JointAngles> lra::Kinematics::GetInverseKinematics(const glm::vec3& target, const JointLength& lra_dimensions) {
	float l1 = lra_dimensions.arm;
	float l2 = lra_dimensions.elbow;
	float l3 = lra_dimensions.wrist;

	auto [x, y, z] = target;
	y -= 72;

	float base_angle = atan2(x, z) * TO_DEGREE;
	float l = sqrt(x * x + z * z);
	float theta = atan2(y, l) * TO_DEGREE;

	float a = sqrt(l * l + y * y);
	float b = (l2 + l3) * (a / (l1 + l2 + l3));

	float A = acos((l1 * l1 + a * a - b * b) / (2 * l1 * a)) * TO_DEGREE;
	float B = acos((b * b + l1 * l1 - a * a) / (2 * b * l1)) * TO_DEGREE;
	float C = acos((l2 * l2 + b * b - l3 * l3) / (2 * l2 * b)) * TO_DEGREE;
	float D = acos((l3 * l3 + l2 * l2 - b * b) / (2 * l3 * l2)) * TO_DEGREE;

	float lm = sqrt(l1 * l1 + l2 * l2 - (cos((360 - (B + C)) * TO_RADIAN) * 2 * l1 * l2));

	float A_t = acos((l1 * l1 + lm * lm - l2 * l2) / (2 * l1 * lm)) * TO_DEGREE * 2;
	float D_t = acos((l2 * l2 + lm * lm - l1 * l1) / (2 * l2 * lm)) * TO_DEGREE * 2;

	auto angles = Clamp(JointAngles{ base_angle, A + theta + A_t, 360 - (B + C), D + D_t, 0, 0 });

	bool is_valid = true;
	for (int i = 0; i < 6; i++) {
		if ((angles[i] != angles[i])) {
			is_valid = false;
			break;
		}
	}

	return { is_valid, angles };
}
