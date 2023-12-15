#pragma once

#include "Types.h"

namespace lgl {
	bool IsInitialized();
	void Initialize(void* loadprocaddress);

	void Viewport(int x, int y, int width, int height);
	void ReadPixels(int x, int y, int width, int height, lgl::Format format, lgl::Type type, float* pixels);
	void SetReadBuffer(lgl::Attachment attachment);
	void ResetReadBuffer();

	void Clear(float r, float g, float b, float a, unsigned int flags);
	void Clear(unsigned int flags);
	void ClearColor(float r, float g, float b, float a);

	void Enable();
	void Disable();
}