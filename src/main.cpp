#include "Lucy/Lucy.h"
#include "Lucy/System.h"
#include <MineClone/Game.h>

int main(int argcount, char** args) {
	lucy::AddSystem(lucy::INTITIALIZATION, lpv::InitializeGame);
	lucy::AddSystem(lucy::RUNTIME, lpv::RuntimeGame);

	lucy::Initialize();
	lucy::Mainloop();

	return 0;
}
