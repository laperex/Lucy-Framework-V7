#include "Sorting.h"

// Rest = { 180, 300, 0 }

#define REST_POS_X (180)
#define REST_POS_Y (300)
#define REST_POS_Z (0)
#define REST_POS (glm::ivec3(REST_POS_X, REST_POS_Y, REST_POS_Z))

enum MovementSteps {
	S0_REST_POS = 0,
	S1_PICK_UPPER_GO = 1,
	S2_PICK_LOWER_GO = 2,
	S3_PICK_LOWER_GC = 3,
	S4_PICK_UPPER_GC = 4,
	S5_PLACE_UPPER_GC = 5,
	S6_PLACE_LOWER_GC = 6,
	S7_PLACE_LOWER_GO = 7,
	S8_PLACE_UPPER_GO = 8,
	S9_REST_POS = 9,

	MovementSteps_COUNT
};

lra::Sorting::Sorting() {
	for (auto& animation: animation_storage) {
		for (int i = 0; i < MovementSteps_COUNT; i++) {
			animation.step_array.push_back({});
		}

		animation.step_array[S0_REST_POS].target_position = REST_POS;

		

		animation.step_array[S9_REST_POS].target_position = REST_POS;
	}
}

void lra::Sorting::SetParameters(SelectedBall ball, glm::ivec2 pos) {
	if (ball < 0) return;

	auto& animation = animation_storage[ball];

	// animation.step_array[TO_UPPER_DEST_POS].target_position = { pos.x, 300,  }
}
