#include "Lucy.h"
#include "System.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <LucyRE/LucyRE.h>
#include "Camera.h"
#include "ECS.h"
#include "Events.h"
#include <glad/glad.h>

static auto& registry = Registry::Instance();

void lucy::Initialize(const char* title, int posx, int posy, int width, int height) {
	auto& window = registry.store<Window>();

	window.pos.x = posx;
	window.pos.y = posy;
	window.size.x = width;
	window.size.y = height;
	window.title = title;
}

void lucy::Initialize() {
	auto& window = registry.store<Window>();

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	window.InitSDLWindow();

	lgl::Initialize(SDL_GL_GetProcAddress);
	lre::Initialize();
}

void lucy::Mainloop() {
	auto& window = registry.store<Window>();
	auto& camera = registry.store<Camera>();

	RunInitializationSystems();

	double dt = 0;
	while (!Events::IsQuittable()) {
		const auto& start_time = std::chrono::high_resolution_clock::now();

		Events::Update();
		camera.Update(dt);

		glEnable(GL_DEPTH_TEST);
		lgl::Viewport(0, 0, window.size.x, window.size.y);
		lgl::Clear(0, 0, 0, 1, lgl::COLOR_BUFFER_BIT | lgl::DEPTH_BUFFER_BIT);

		if (window.enable_framebuffer) {
			if (window.framebuffer != nullptr) {
				if (window.size.x != window.framebuffer->width || window.size.y != window.framebuffer->height) {
					delete window.framebuffer;
					window.framebuffer = nullptr;
				}
			}

			if (window.framebuffer == nullptr) {
				window.framebuffer = new lgl::FrameBuffer(window.size.x, window.size.y, false);
			}
		}

		RunRuntimeSystems();

		window.SwapWindow();

		const auto& end_time = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
	}

	lre::Destroy();
}

