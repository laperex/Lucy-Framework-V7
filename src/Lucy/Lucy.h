#pragma once

#include "ECS.h"
#include "Camera.h"
#include "Events.h"
#include "System.h"


namespace lucy {
	void Initialize(const char* title, int posx, int posy, int width, int height);
	void Initialize();
	void Mainloop();
}
