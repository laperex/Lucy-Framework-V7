#include "System.h"
#include <array>

static std::array<std::vector<lucy::SYSTEM_FUNCTION>, lucy::SYSTEM_TYPE_COUNT> system_function_array;

void lucy::AddSystem(SYSTEM_TYPE type, SYSTEM_FUNCTION system) {
	assert(type < SYSTEM_TYPE_COUNT && type >= 0);

	assert(!IsSystemPresent(type, system));

	system_function_array[type].push_back(system);
}

bool lucy::IsSystemPresent(SYSTEM_TYPE type, SYSTEM_FUNCTION system) {
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

void lucy::RunRuntimeSystems() {
	RunSystems(RUNTIME);
}

void lucy::RunEditorSystems() {
	RunSystems(EDITOR);
}

void lucy::RunInitializationSystems() {
	RunSystems(INTITIALIZATION);
}

void lucy::RunSystems(SYSTEM_TYPE type) {
	assert(type < SYSTEM_TYPE_COUNT && type >= 0);

	for (auto& system: system_function_array[type]) {
		system();
	}
}
