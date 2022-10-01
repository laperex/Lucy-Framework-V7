#pragma once

namespace lra {
	struct JointLength {
		// float ground_to_base = 72.5;
		float arm = 0;
		float elbow = 0;
		float wrist = 0;

		float Total() {
			return arm + elbow + wrist;
		}
	};
}