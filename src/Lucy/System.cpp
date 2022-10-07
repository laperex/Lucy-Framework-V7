#include "System.h"
#include <array>

static std::array<std::vector<void*>, lucy::SYSTEM_TYPE_COUNT> system_function_array;

void lucy::AddSystem(SYSTEM_TYPE type, void* system) {
	assert(type < SYSTEM_TYPE_COUNT && type >= 0);

	assert(!IsSystemPresent(type, system));

	system_function_array[type].push_back(system);
}

bool lucy::IsSystemPresent(SYSTEM_TYPE type, void* system) {
	assert(type < SYSTEM_TYPE_COUNT && type >= 0);

	for (auto& system_array: system_function_array) {
		for (auto& system_function: system_array) {
			if (system_function == system && system_function_array[type] == system_array) {
				return true;
			}
		}
	}

	return false;
}

template <typename ...Args>
void RunSystems(lucy::SYSTEM_TYPE type, Args... args) {
	assert(type < lucy::SYSTEM_TYPE_COUNT && type >= 0);

	for (auto& system: system_function_array[type]) {
		switch (type) {
			case lucy::SDL_EVENT_SYSTEM:
				// auto func = [system]<typename ...Args>(SDL_Event& event, Args ...args) {
				// 	((lucy::SDL_EVENT_SYSTEM_FUNCTION)system)(event);
				// };
				// func(args...);
				break;

			default:
				((lucy::SYSTEM_FUNCTION)system)();
		}
	}
}

void lucy::RunRuntimeSystems() {
	RunSystems(RUNTIME);
}

void lucy::RunEditorSystems() {
	RunSystems(EDITOR_RUNTIME);
}

void lucy::RunInitializationSystems() {
	RunSystems(INTITIALIZATION);
}

void lucy::RunEditorInitializationSystems() {
	RunSystems(EDITOR_INITIALIZATION);
}

void lucy::RunSdlEventSystems(SDL_Event& sdl_event) {
	RunSystems(SDL_EVENT_SYSTEM, sdl_event);
}

// void lucy::RunSystems(SYSTEM_TYPE type) {
	
// }
