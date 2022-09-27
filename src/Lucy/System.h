#pragma once

#include "ECS.h"
#include <vector>
#include <SDL2/SDL_events.h>

namespace lucy {
	enum SYSTEM_TYPE {
		INTITIALIZATION = 0,
		RUNTIME = 1,
		EDITOR = 2,
		SDL_EVENT_SYSTEM = 3,

		SYSTEM_TYPE_COUNT
	};

	typedef void(*SYSTEM_FUNCTION)();
	typedef void(*SDL_EVENT_SYSTEM_FUNCTION)(SDL_Event&);

	void AddSystem(SYSTEM_TYPE type, void* system);
	bool IsSystemPresent(SYSTEM_TYPE type, void* system);

	void RunRuntimeSystems();
	void RunEditorSystems();
	void RunInitializationSystems();
	void RunSdlEventSystems(SDL_Event& sdl_event);
}