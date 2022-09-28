#include "RoboticArm.h"
#include "Renderer.h"
#include <Lucy/Lucy.h>
#include "Light.h"
#include "Material.h"
#include <Lucy/imgui_lucy_impl.h>
#include "Panels/Panel.h"
#include "Controller.h"

static auto& registry = Registry::Instance();
static std::vector<glm::vec3> grid;

// TODO//: ROBOTIC ARM MODEL CONTROL
// TODO: Add Gizmo
// TODO//: Add Grid
// TODO: Add Animator
// TODO: Smooth Movement
// TODO: Serial Comms

void lra::InitializeArm() {
	auto& camera = registry.store<lucy::Camera>();

	IntializeRenderer();

	camera.position = { 0, 0, 750 };

	for (int i = -500; i <= 500; i += 10) {
		grid.push_back({ -500, 0, i });
		grid.push_back({ +500, 0, i });

		grid.push_back({ i, 0, -500 });
		grid.push_back({ i, 0, +500 });
	}
}

void lra::RuntimeUpdateArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();
	
	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	lre::SetModel(glm::mat4(1.0f));
	lre::RenderLine(grid, { 1, 1, 1, 0.1 });

	RenderLRA(controller.target_joint_angles);
}

void lra::EditorUpdateArm() {
	panel::Lighting();
	panel::ArmControl();
}
