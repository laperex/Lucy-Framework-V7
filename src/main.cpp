#include <iostream>
#include <Lucy/Editor.h>
#include "Lucy/Lucy.h"
#include <Lucy/Ray.h>
#include "Lucy/System.h"
#include <Lucy/imgui_lucy_impl.h>
// #include <MineClone/Game.h>
// #include <Writer 2 DOF/Write.h>
#include <LucyVE/Game.h>
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

	lucy::AddSystem(lucy::EDITOR_INITIALIZATION, ImGui::Theme::RedDark);
	lucy::AddSystem(lucy::EDITOR_RUNTIME, lra::EditorUpdateArm);

	lucy::AddSystem(lucy::INTITIALIZATION, Editor::Initialize);
	lucy::AddSystem(lucy::RUNTIME, Editor::Update);

	lucy::AddSystem(lucy::INTITIALIZATION, lra::InitializeArm);
	lucy::AddSystem(lucy::RUNTIME, lra::RuntimeUpdateArm);

	// lucy::Initialize();
	lucy::Initialize("Lucy Framework V7", 0, 0, 2560, 1440, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS);
	lucy::Mainloop();

	return 0;
}
