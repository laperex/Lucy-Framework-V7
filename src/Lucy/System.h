#pragma once

#include "ECS.h"
#include <vector>
#include <SDL2/SDL_events.h>

namespace lucy {
	enum SYSTEM_TYPE {
		INTITIALIZATION = 0,
		RUNTIME = 1,
		SDL_EVENT_SYSTEM = 2,

		EDITOR_RUNTIME = 3,
		EDITOR_INITIALIZATION = 4,

		SYSTEM_TYPE_COUNT
	};

	typedef void(*SYSTEM_FUNCTION)();
	typedef void(*SDL_EVENT_SYSTEM_FUNCTION)(SDL_Event&);

	void AddSystem(SYSTEM_TYPE type, void* system);
	bool IsSystemPresent(SYSTEM_TYPE type, void* system);

	void RunRuntimeSystems();
	void RunEditorSystems();
	void RunInitializationSystems();
	void RunEditorInitializationSystems();
	void RunSdlEventSystems(SDL_Event& sdl_event);
}