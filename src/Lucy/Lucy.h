#pragma once

#include "ECS.h"
#include "Camera.h"
#include "Events.h"
#include "System.h"
#include <Lucy/Registries/Registry.h>

namespace lucy {
	void Initialize(const char* title, int posx, int posy, int width, int height, uint32_t flags = 0);
	void Initialize();
	void Mainloop();
}
