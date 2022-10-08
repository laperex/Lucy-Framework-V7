#include "RoboticArm.h"
#include "Tracing.h"
#include <chrono>
#include <iostream>
#include "LoadSave.h"
#include "Renderer.h"
#include "Animator.h"
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

#define EASE_FUNC(x, t) (pow(x, t) / (pow(x, t) + pow(1 - x, t)))

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

	DeSerializeAnimator("animator.yaml");

	IntializeRenderer();

	camera.position = { 0, 0, 1000 };
}

void lra::RuntimeUpdateArm() {
	auto& camera = registry.store<lucy::Camera>();
	auto& window = registry.store<lucy::Window>();
	auto& controller = registry.store<Controller>();
	auto& animator = registry.store<Animator>();

	if (camera.framebuffer != nullptr) {
		camera.framebuffer->Bind();

		lgl::Viewport(0, 0, camera.width, camera.height);
		lgl::Clear(0, 0, 0, 1, lgl::COLOR_BUFFER_BIT | lgl::DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

		lre::SetView(camera.view);
		lre::SetProjection(camera.projection);

		if (controller.enable_ik) {
			bool is_valid;
			auto angles = Kinematics::GetInverseKinematics(is_valid, controller.ik_target, controller.lra_dimension);
			if (is_valid) {
				angles.gripper_control = controller.target_joint_angles.gripper_control;
				angles.gripper_rotate = controller.target_joint_angles.gripper_rotate;

				controller.target_joint_angles = angles;
			}
		}

		controller.fk_result = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);

		{
			static int idx = 0;
			static float progress = 0;

			if (animator.selected_animation != LUCY_NULL_UUID) {
				auto& animation = animator.animation_registry[animator.selected_animation].animation;
				auto& generated = animation.GetGenerated();

				TraceAnimationPoints(idx, &animation);

				if (animator.animationstate == PLAY) {
					if (idx >= generated.size()) {
						idx = 0;
					}

					if (idx == 0) {
						static JointAngles last_angles, next_angles;

						if (progress == 0) {
							last_angles = controller.target_joint_angles;
							bool is_valid;
							next_angles = Kinematics::GetInverseKinematics(is_valid, generated[idx].position);
							next_angles.gripper_control = generated[idx].gripper_ctrl;
							next_angles.gripper_rotate = generated[idx].gripper_rot;
						}

						if (progress >= 1) {
							progress = 0;
							idx++;
						} else {
							for (int i = 0; i < 6; i++)
								controller.target_joint_angles[i] = last_angles[i] + (next_angles[i] - last_angles[i]) * EASE_FUNC(progress, 2.5);
							progress += 1.0f / 500.0f;
						}
					} else if (idx < generated.size()) {
						static auto last_time = std::clock();
						auto current_time = std::clock();
						static JointAngles last_angles, next_angles;
						static int last_idx = 0;

						if (last_idx != idx) {
							bool is_valid;
							last_angles = Kinematics::GetInverseKinematics(is_valid, generated[idx - 1].position);
							last_angles.gripper_control = generated[idx - 1].gripper_ctrl;
							last_angles.gripper_rotate = generated[idx - 1].gripper_rot;

							next_angles = Kinematics::GetInverseKinematics(is_valid, generated[idx].position);
							next_angles.gripper_control = generated[idx].gripper_ctrl;
							next_angles.gripper_rotate = generated[idx].gripper_rot;

							last_idx = idx;
						}

						float p = (current_time - last_time) / controller.speed;
						if (p >= 1) p = 1;
						for (int i = 0; i < 6; i++)
							controller.target_joint_angles[i] = last_angles[i] + (next_angles[i] - last_angles[i]) * EASE_FUNC(p, 2.5);

						if (current_time - last_time >= controller.speed) {
							last_time = current_time;
							idx++;
						}

						if (idx >= generated.size()) {
							// idx = 0;
							if (!animation.loop)
								animator.animationstate = PAUSE;
						}
					}
				}
			}
			if (animator.animationstate == STOP) {
				idx = 0;
			}
		}

		RenderAxisLine(true, false, true);
		RenderGrid();
		RenderLRA(controller.target_joint_angles);

		if (lucy::Events::IsKeyChord({ SDL_SCANCODE_LCTRL, SDL_SCANCODE_S })) {
			SerializeAnimator("animator.yaml");
		}

		camera.framebuffer->UnBind();
	}
}

void lra::EditorUpdateArm() {
	panel::PanelUpdate();
}
