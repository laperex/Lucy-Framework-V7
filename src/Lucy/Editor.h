#pragma once

#include <vector>

class Editor {
	#define self Instance()
	static Editor* Instance() {
		static Editor instance;
		return &instance;
	}

public:
	Editor() {}
	~Editor() {}

	static void Initialize();
	static void RenderBegin();
	static void RenderEnd();
	static void Update();
	static void ShutDown();
};