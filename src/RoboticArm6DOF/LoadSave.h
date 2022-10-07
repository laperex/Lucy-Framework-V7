#pragma once

#include <string>

namespace lra {
	void SerializeLRA(const char* filename);
	void SerializeAnimator(const char* filename);
	void DeSerializeLRA(const char* filename);
	void DeSerializeAnimator(const char* filename);
}