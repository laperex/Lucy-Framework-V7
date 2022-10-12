#pragma once

#include "JointAngles.h"
#include <map>
#include <array>
#include <string>

namespace lra {
	struct SerialCommunication {
		std::map<int, int> servo_angles_map[6];
		std::string port_name = "COM3";
		std::string connect_port = "";

		JointAngles current_angles;

		SerialCommunication();
		void TransmitServoAngles(JointAngles angles);
		void Reconnect();
	};
}