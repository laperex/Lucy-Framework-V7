#include "Position.h"
#include <assert.h>

arm::Position::Position(float lower, float upper) {
	this->lower = lower;
	this->upper = upper;
}

float& arm::Position::operator[](int idx) {
	assert(idx >= 0 && idx < 2);

	switch (idx) {
		case 0:
			return lower;
		case 1:
			return upper;
	}
}
