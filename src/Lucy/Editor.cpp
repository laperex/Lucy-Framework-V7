#include "Editor.h"
#include "System.h"
#include "ECS.h"
#include "Events.h"
#include "Window.h"
#include "imgui_lucy_impl.h"
#include "Camera.h"
#include <iostream>

#define ENABLE_DOCKSPACE
// #define ENABLE_VIEWPORT

static auto& registry = Registry::Instance();

void Editor::Initialize() {
	auto& window = registry.store<lucy::Window>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigDockingWithShift = true;

	#ifdef ENABLE_VIEWPORT
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	#endif ENABLE_VIEWPORT

	ImGui_ImplSDL2_InitForOpenGL(window.sdl_window, (void*)window.sdl_glcontext);
	ImGui_ImplOpenGL3_Init("#version 400");

	VS_Theme();

	ImGui::StyleColorsDark();

	lucy::Events::AddFunction([](SDL_Event& event) {
		ImGui_ImplSDL2_ProcessEvent(&event);
	});
}

void Editor::RenderBegin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	auto& window = registry.store<lucy::Window>();

	// Screen
	#ifdef ENABLE_DOCKSPACE
		static bool p_open = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_AutoHideTabBar;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

		int x = 0, y = 0, w = 0, h = 0;
		SDL_GetWindowSize(window.sdl_window, &w, &h);

		#ifdef ENABLE_VIEWPORT
			SDL_GetWindowPosition(window.sdl_window, &x, &y);
		#endif ENABLE_VIEWPORT

		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowSize(ImVec2(w, h));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		ImGui::Begin("DockSpace", &p_open, window_flags);

		ImGui::PopStyleVar(3);
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// lucy::RunEditorMainWindowSystems();
	
		ImGui::End();
	#endif ENABLE_DOCKSPACE
}

void Editor::RenderEnd() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	#ifdef ENABLE_VIEWPORT
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		registry.store<lucy::Window>().SetCurrent();
	#endif ENABLE_VIEWPORT
}

void Editor::Update() {
	RenderBegin();

	lucy::RunEditorSystems();

	RenderEnd();

	if (lucy::Events::IsQuittable()) {
		Editor::ShutDown();
	}
}

void Editor::ShutDown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}
