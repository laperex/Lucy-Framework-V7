#pragma once

#include <vector>
#include <LucyGL/LucyGL.h>
#include <Lucy/imgui_lucy_impl.h>

class Editor {
	lgl::FrameBuffer* framebuffer = nullptr;
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