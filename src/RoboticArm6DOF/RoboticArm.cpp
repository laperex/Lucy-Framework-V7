#include "RoboticArm.h"
#include "Renderer.h"
#include <Lucy/Lucy.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include "Kinematics.h"
#include <Lucy/imgui_lucy_impl.h>
#include "Panels/Panel.h"
#include "Controller.h"

static auto& registry = Registry::Instance();
static std::vector<glm::vec3> grid, grid_small;

// TODO//: ROBOTIC ARM MODEL CONTROL
// TODO: Add Gizmo
// TODO//: Add Grid
// TODO: Add Animator
// TODO: Smooth Movement
// TODO: Serial Comms

void lra::InitializeArm() {
	auto& camera = registry.store<lucy::Camera>();

	IntializeRenderer();

	camera.position = { 0, 0, 1000 };

	for (int i = -500; i <= 500; i += 10) {
		if (i % 100 == 0) {
			grid.push_back({ -500, 0, i });
			grid.push_back({ +500, 0, i });

			grid.push_back({ i, 0, -500 });
			grid.push_back({ i, 0, +500 });
		} else {
			grid_small.push_back({ -500, 0, i });
			grid_small.push_back({ +500, 0, i });

			grid_small.push_back({ i, 0, -500 });
			grid_small.push_back({ i, 0, +500 });
		}
	}
}

void lra::RuntimeUpdateArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();
	
	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	lre::SetModel(glm::mat4(1.0f));

	lre::RenderLine({ 0, 0, 0 }, { 1000, 0, 0 }, { 1, 0, 0, 1 });
	lre::RenderLine({ 0, 0, 0 }, { 0, 1000, 0 }, { 0, 1, 0, 1 });
	lre::RenderLine({ 0, 0, 0 }, { 0, 0, 1000 }, { 0, 0, 1, 1 });

	lre::RenderFlushLine();

	lre::RenderLine(grid, { 1, 1, 1, 0 });
	lre::RenderLine(grid_small, { 0.5, 0.5, 0.5, 0 });

	if (controller.enable_ik) {
		auto [is_valid, angles] = Kinematics::GetInverseKinematics(controller.ik_target, controller.lra_dimension);
		if (is_valid) {
			angles.gripper_control = controller.target_joint_angles.gripper_control;
			angles.gripper_rotate = controller.target_joint_angles.gripper_rotate;
			controller.target_joint_angles = angles;
		}
	}

	RenderLRA(controller.target_joint_angles);
	RenderCube(controller.ik_target + glm::vec3(0, 72.5000, 0), { 10, 10, 10 }, 2);
}

void lra::EditorUpdateArm() {
	panel::Lighting();
	panel::Controller();
	panel::Kinematics();
}
