#include "RoboticArm.h"
#include <iostream>
#include "Renderer.h"
#include <Lucy/Editor.h>
#include <Lucy/Picking.h>
#include <Lucy/Events.h>
#include <Lucy/Window.h>
#include <Lucy/Lucy.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include "Kinematics.h"
#include <Lucy/imgui_lucy_impl.h>
#include "Panels/Panel.h"
#include <glm/gtx/string_cast.hpp>
#include "Controller.h"
#include <glad/glad.h>
#include "LRAGizmo.h"

static auto& registry = Registry::Instance();

// TODO//: ROBOTIC ARM MODEL CONTROL
// TODO//: Add Gizmo
// TODO//: Add Grid
// TODO//: Add Animator
// TODO: Smooth Movement
// TODO: Serial Comms

void lra::InitializeArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& controller = registry.store<Controller>();
	auto& materialregistry = registry.store<lucy::MaterialRegistry>();
	auto& lightregistry = registry.store<lucy::LightRegistry>();

	registry.store<lucy::LightRegistry>().Insert("Default");
	registry.store<lucy::MaterialRegistry>().Insert("LRA");

	IntializeRenderer();

	camera.position = { 0, 0, 1000 };
	controller.mode = PICKING;

	lucy::AddSystem(lucy::EDITOR_MAIN_WINDOW_SYSTEM, lra::GizmoSystem);
}

void lra::RuntimeUpdateArm() {
	static lgl::FrameBuffer* framebuffer = nullptr;
	auto& camera = registry.store<lucy::Camera>();
	auto& window = registry.store<lucy::Window>();
	auto& controller = registry.store<Controller>();

	if (controller.mode == WRITING) {
		controller.lra_dimension.wrist = 240;
	} else {
		controller.lra_dimension.wrist = 190;
	}

	if (framebuffer == nullptr) {
		framebuffer = new lgl::FrameBuffer(camera.width, camera.height, true);
	}

	framebuffer->Bind();

	lgl::Viewport(0, 0, window.size.x, window.size.y);
	lgl::Clear(0, 0, 0, 1, lgl::COLOR_BUFFER_BIT | lgl::DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lre::SetView(camera.view);
	lre::SetProjection(camera.projection);

	if ((controller.mode == PICKING && controller.enable_ik) || controller.mode == WRITING) {
		bool is_valid;
		auto angles = Kinematics::GetInverseKinematics(is_valid, controller.ik_target, controller.lra_dimension);
		if (is_valid) {
			angles.gripper_control = controller.target_joint_angles.gripper_control;
			angles.gripper_rotate = controller.target_joint_angles.gripper_rotate;

			controller.target_joint_angles = angles;
		}
	}

	controller.fk_result = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);

	RenderAxisLine(true, false, true);
	RenderGrid();
	RenderLRA(controller.target_joint_angles);

	framebuffer->UnBind();

	// lre::RenderFrameBufferToScreen(framebuffer, { camera.width, camera.height });
	Editor::SetMainFrameBuffer(framebuffer);
}

void lra::EditorUpdateArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& window = registry.store<lucy::Window>();

	panel::PanelUpdate();

	camera.enable = Editor::IsMainWindowHovered();
}
