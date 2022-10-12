#include "JointAngles.h"
#include <assert.h>

lra::JointAngles::JointAngles(float angle) {
	for (int i = 0; i < LRA_MAX_JOINT; i++)
		operator[](i) = angle;
}

lra::JointAngles::JointAngles(float base, float arm, float elbow, float wrist, float gripper_rotate, float gripper_control) {
	this->base = base;
	this->arm = arm;
	this->elbow = elbow;
	this->wrist = wrist;
	this->gripper_rotate = gripper_rotate;
	this->gripper_control = gripper_control;
}

float& lra::JointAngles::operator[](int idx) {
	assert(idx >= 0 && idx < LRA_MAX_JOINT);

	switch (idx) {
		case 0:
			return base;
			break;
		case 1:
			return arm;
			break;
		case 2:
			return elbow;
			break;
		case 3:
			return wrist;
			break;
		case 4:
			return gripper_rotate;
			break;
		case 5:
			return gripper_control;
			break;
	}
}

bool lra::JointAngles::operator==(const JointAngles& angles) {
	return (base == angles.base && arm == angles.arm && elbow == angles.elbow && wrist == angles.wrist && gripper_rotate == angles.gripper_rotate && gripper_control == angles.gripper_control);
}

bool lra::JointAngles::operator!=(const JointAngles& angles) {
	return (base != angles.base && arm != angles.arm && elbow != angles.elbow && wrist != angles.wrist && gripper_rotate != angles.gripper_rotate && gripper_control != angles.gripper_control);
}
