#pragma once

#include "ECS.h"
#include <vector>

namespace lucy {
	enum SYSTEM_TYPE {
		INTITIALIZATION = 0,
		RUNTIME = 1,
		EDITOR = 2,

		SYSTEM_TYPE_COUNT
	};

	typedef void(*SYSTEM_FUNCTION)();

	void AddSystem(SYSTEM_TYPE type, SYSTEM_FUNCTION system);
	bool IsSystemPresent(SYSTEM_TYPE type, SYSTEM_FUNCTION system);

	void RunRuntimeSystems();
	void RunEditorSystems();
	void RunInitializationSystems();
	void RunSystems(SYSTEM_TYPE type);
}