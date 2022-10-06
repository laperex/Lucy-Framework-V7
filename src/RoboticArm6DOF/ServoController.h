#pragma once

#include "JointAngles.h"
#include <map>
#include <array>

namespace lra {
	struct ServoContoller {
		uint16_t current_servo[6];
		uint16_t servo[6];

		std::array<std::map<uint8_t, uint16_t>, 6> mapped_pulse;

		ServoContoller();

		bool IsAtPosition();
		void Set(JointAngles angles);
		void Transmit();
	};
}