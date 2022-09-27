#include "Lucy/Lucy.h"
#include <Lucy/Editor.h>
#include "Lucy/System.h"
// #include <MineClone/Game.h>
#include <Writer 2 DOF/Writer.h>

int main(int argcount, char** args) {
	// lucy::AddSystem(lucy::INTITIALIZATION, lpv::InitializeGame);
	// lucy::AddSystem(lucy::RUNTIME, lpv::RuntimeGame);

	lucy::AddSystem(lucy::INTITIALIZATION, Editor::Initialize);
	lucy::AddSystem(lucy::RUNTIME, Editor::Update);
	lucy::AddSystem(lucy::EDITOR, arm::EditorUpdate);

	lucy::Initialize();
	lucy::Mainloop();

	Editor::ShutDown();

	return 0;
}
