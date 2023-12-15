#include <iostream>
#include <Lucy/Editor.h>
#include "Lucy/Lucy.h"
#include <Lucy/Ray.h>
#include "Lucy/System.h"
#include <vulkan/vulkan.h>
#include <Lucy/imgui_lucy_impl.h>
// #include <MineClone/Game.h>
// #include <Writer 2 DOF/Write.h>
// #include <LucyVE/Game.h>
#include <RoboticArm6DOF/RoboticArm.h>

int main(int argcount, char** args) {
	// lucy::RayCastPoint({ 10, 10, 0 }, { 0, 0, 0 }, [](int x, int y, int z) -> bool {
	// 	std::cout << "P: " << x << ' ' << y << ' ' << z << '\n';
	// 	return false;
	// });

	// lucy::AddSystem(lucy::EDITOR_RUNTIME, ImGui::ShowDemoWindow);

	// lucy::AddSystem(lucy::INTITIALIZATION, lpv::InitializeGame);
	// lucy::AddSystem(lucy::RUNTIME, lpv::RuntimeGame);

	// lucy::AddSystem(lucy::INTITIALIZATION, lve::InitializeGame);
	// lucy::AddSystem(lucy::RUNTIME, lve::RuntimeGame);

	// lucy::AddSystem(lucy::EDITOR_INITIALIZATION, );

	lucy::AddSystem(lucy::EDITOR_INITIALIZATION, (void*)ImGui::Theme::RedDark);
	lucy::AddSystem(lucy::EDITOR_RUNTIME, (void*)lra::EditorUpdateArm);

	lucy::AddSystem(lucy::INTITIALIZATION, (void*)Editor::Initialize);
	lucy::AddSystem(lucy::RUNTIME, (void*)Editor::Update);

	lucy::AddSystem(lucy::INTITIALIZATION, (void*)lra::InitializeArm);
	lucy::AddSystem(lucy::RUNTIME, (void*)lra::RuntimeUpdateArm);

	// lucy::Initialize();
	lucy::Initialize("Lucy Framework V7", 0, 0, 2560, 1600, SDL_WINDOW_ALLOW_HIGHDPI);
	lucy::Mainloop();

	return 0;
}
