#include "RoboticArm.h"
#include "Renderer.h"
#include <Lucy/Lucy.h>
#include "Light.h"
#include "Material.h"
#include <Lucy/imgui_lucy_impl.h>
#include "Panels/Lighting.h"
#include "Panels/ArmControl.h"
#include "Controller.h"

static auto& registry = Registry::Instance();
static std::vector<glm::vec3> grid;

void lra::InitializeArm() {
	auto& camera = registry.store<lucy::Camera>();

	IntializeRenderer();

	camera.position = { 0, 0, 750 };

	for (int z = 0; z < 1000; z += 10) {
		for (int x = 0; x < 1000; x += 10) {
			grid.push_back({ 1000/2 - x, 0, 1000/2 - z });
		}
	}
}

void lra::RuntimeUpdateArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();
	
	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	lre::SetModel(glm::mat4(1.0f));
	lre::RenderLine(grid, { 1, 1, 1, 1 });

	RenderLRA(controller.target_joint_angles);
}

void lra::EditorUpdateArm() {
	panel::Lighting();
	panel::ArmControl();
}
