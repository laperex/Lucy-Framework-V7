#include "Editor.h"
#include "System.h"
#include "ECS.h"
#include "Events.h"
#include "Window.h"
#include "imgui_lucy_impl.h"

static auto& registry = Registry::Instance();

void Editor::SetMainFrameBuffer(lgl::FrameBuffer* framebuffer) {
	self->framebuffer = framebuffer;
}

lgl::FrameBuffer* Editor::GetMainFrameBuffer() {
	return self->framebuffer;
}

void Editor::Initialize() {
	auto& window = registry.store<lucy::Window>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigDockingWithShift = true;

	// if (editor_state.enable_viewports) {
	// 	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// 	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	// }

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
	if (self->framebuffer != nullptr) {
		static bool p_open = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

		int x = 0, y = 0, w = 0, h = 0;
		SDL_GetWindowSize(window.sdl_window, &w, &h);

		// if (editor_state.enable_viewports)
		// 	SDL_GetWindowPosition(window.sdl_window, &x, &y);

		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowSize(ImVec2(w, h));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		ImGui::Begin("DockSpace", &p_open, window_flags);

		ImGui::PopStyleVar(3);
		// if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			
		// 	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		// 	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		// }
		ImGui::GetWindowDrawList()->AddImage((void*)self->framebuffer->texture->id, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0, (float)h / self->framebuffer->height), ImVec2((float)w / self->framebuffer->width, 0));

		lucy::RunEditorMainWindowSystems();
	
		ImGui::End();
	}
}

void Editor::RenderEnd() {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	// if (editor_state.enable_viewports) {
	// 	ImGui::UpdatePlatformWindows();
	// 	ImGui::RenderPlatformWindowsDefault();

	// 	window.SetCurrent();
	// }
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
