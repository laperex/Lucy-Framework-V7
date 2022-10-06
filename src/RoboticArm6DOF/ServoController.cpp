#include "ServoController.h"

lra::ServoContoller::ServoContoller() {
	for (int i = 0; i < 6; i++) {
		mapped_pulse[i] = {
			{ 0, 200 },
			{ 180, 400 },
		};
	}
}

bool lra::ServoContoller::IsAtPosition() {
	for (int i = 0; i < 6; i++)
		if (servo[i] != current_servo[i])
			return false;

	return true;
}

void lra::ServoContoller::Set(JointAngles angles) {
	for (int i = 0; i < 6; i++) {
		// float m = (mapped_pulse[i][180] - mapped_pulse[i][0]) / (180 - 0);
		servo[i] = uint8_t(mapped_pulse[i][0] - (((mapped_pulse[i][180] - mapped_pulse[i][0]) / (180 - 0)) * (0 - angles[i])));
	}
}
