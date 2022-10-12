#include "ServoController.h"
#include <LucyUtil/LucyUtil.h>
#include <iostream>
#include <math.h>

#define PORT_FORMAT (std::string("\\\\.\\"))
#define MAP_ANGLE_TO_PWN(angle, p_0, p_180) (((angle / 180) * (p_180 - p_0)) + p_0)

// 0 -> 110 |  | 430
// 1 -> 100 | 300 | 490
// 2 -> 80 | 280 | (455 | 460)
// 3 -> 400 | 230 | 110
// 4 -> 140 | 460
// 5 -> 150 | 310

lra::SerialCommunication::SerialCommunication() {
	servo_angles_map[0] = {
		{ 0, 110 },
		{ 180, 430 },
	};
	servo_angles_map[1] = {
		{ 0, 100 },
		{ 90, 300 },
		{ 180, 490 },
	};
	servo_angles_map[2] = {
		{ 0, 80 },
		{ 90, 280 },
		{ 180, 455 },
	};
	servo_angles_map[3] = {
		{ 0, 400 },
		{ 90, 230 },
		{ 180, 110 },
	};
	servo_angles_map[4] = {
		{ 0, 140 },
		{ 180, 460 },
	};
	servo_angles_map[5] = {
		{ 0, 150 },
		{ 180, 310 },
	};
}


void lra::SerialCommunication::TransmitServoAngles(JointAngles angles) {
	if (!util::serial::is_port_connected()) return;

	static uint8_t data[] = { '&', 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 'M', 0 };

	data[0] = '&';
	for (int i = 0; i < 6; i++) {
		*(uint16_t*)(&data[2 * i + 1]) = uint16_t(MAP_ANGLE_TO_PWN(angles[i], servo_angles_map[i][0], servo_angles_map[i][180]));
	}

	current_angles = angles;

	util::serial::write_bytes_to_port(data, sizeof(data));
}

void lra::SerialCommunication::Reconnect() {
	if (util::serial::connect_port((PORT_FORMAT + port_name).c_str(), util::serial::BAUDRATE_38400)) {
		connect_port = port_name;
	}
}

// lra::ServoContoller::ServoContoller() {
// 	for (int i = 0; i < 6; i++) {
// 		mapped_pulse[i] = {
// 			{ 0, 200 },
// 			{ 180, 400 },
// 		};
// 	}
// }

// bool lra::ServoContoller::IsAtPosition() {
// 	for (int i = 0; i < 6; i++)
// 		if (servo[i] != current_servo[i])
// 			return false;

// 	return true;
// }

// void lra::ServoContoller::Set(JointAngles angles) {
// 	for (int i = 0; i < 6; i++) {
// 		// float m = (mapped_pulse[i][180] - mapped_pulse[i][0]) / (180 - 0);
// 		servo[i] = mapped_pulse[i][0] - (((mapped_pulse[i][180] - mapped_pulse[i][0]) / (180 - 0)) * (0 - angles[i]));
// 	}
// }
