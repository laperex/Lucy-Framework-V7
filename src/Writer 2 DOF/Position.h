#pragma once

namespace arm {
	struct Position {
		float lower = 90;
		float upper = 90;

		Position() {}
		Position(float base, float lower_elbow);
		float& operator[](int idx);
	};
}