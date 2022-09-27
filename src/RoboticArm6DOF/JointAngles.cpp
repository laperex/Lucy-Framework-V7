#include "JointAngles.h"
#include <assert.h>

arm::JointAngles::JointAngles(float base, float elbow, float arm, float wrist, float gripper_control, float gripper_rotate) {
	this->base = base;
	this->elbow = elbow;
	this->arm = arm;
	this->wrist = wrist;
	this->gripper_control = gripper_control;
	this->gripper_rotate = gripper_rotate;
}

float& arm::JointAngles::operator[](int idx) {
	assert(idx >= 0 && idx <= 5);

	switch (idx) {
		case 0:
			return base;
			break;
		case 1:
			return elbow;
			break;
		case 2:
			return arm;
			break;
		case 3:
			return wrist;
			break;
		case 4:
			return gripper_control;
			break;
		case 5:
			return gripper_rotate;
			break;
	}
}
