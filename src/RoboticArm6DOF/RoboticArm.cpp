#include <chrono>
#include <iostream>
#include <Lucy/Editor.h>
#include <Lucy/Picking.h>
#include <Lucy/Events.h>
#include <Lucy/Window.h>
#include <Lucy/Lucy.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <Lucy/imgui_lucy_impl.h>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include "RoboticArm.h"
#include "Tracing.h"
#include "LoadSave.h"
#include "Renderer.h"
#include "Animator.h"
#include "Kinematics.h"
#include "Panels/Panel.h"
#include "Controller.h"
#include "Canvas.h"
#include "ServoController.h"
#include "Easing.h"
#include "Vision.h"

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
	auto& vision = registry.store<ComputerVision>();
	auto& materialregistry = registry.store<lucy::MaterialRegistry>();
	auto& lightregistry = registry.store<lucy::LightRegistry>();

	registry.store<lucy::LightRegistry>().Insert("Default");
	registry.store<lucy::MaterialRegistry>().Insert("LRA");

	DeSerializeAnimator("animator.yaml");

	IntializeRenderer();

	vision.Initialize();

	camera.position = { 0, 0, 1000 };
}

void lra::RuntimeUpdateArm() {
	auto& canvas = registry.store<Canvas>();
	auto& camera = registry.store<lucy::Camera>();
	auto& window = registry.store<lucy::Window>();
	auto& controller = registry.store<Controller>();
	auto& animator = registry.store<Animator>();
	auto& vision = registry.store<ComputerVision>();

	if (camera.framebuffer != nullptr) {
		camera.framebuffer->Bind();

		lgl::Viewport(0, 0, camera.width, camera.height);
		lgl::Clear(0, 0, 0, 1, lgl::COLOR_BUFFER_BIT | lgl::DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

		lre::SetView(camera.view);
		lre::SetProjection(camera.projection);

		if (controller.ik_enable && animator.animationstate != PLAY) {
			bool is_valid;
			auto angles = (controller.is_ik_picking) ? Kinematics::GetInverseKinematics(is_valid, controller.ik_target): Kinematics::GetInverseKinematics(is_valid, controller.ik_target, controller.ik_phi);
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
				static LUCY_UUID last_animation = LUCY_NULL_UUID;

				if (animator.selected_animation != last_animation) {
					idx = 0;
					progress = 0;
					last_animation = animator.selected_animation;
				}

				if (animator.render_path) {
					TraceAnimationPoints(idx * animator.trace_path, generated.size() - 1, &animation, { 0.2, 1, 1, 1 });
				}

				if (animator.trace_path) {
					if (idx < generated.size())
						TraceAnimationPoints(0, idx, &animation);
					else
						TraceAnimationPoints(0, generated.size() - 1, &animation);
				}

				static bool first = true;
				if (animator.animationstate == PLAY) {
					static JointAngles last_angles;

					if (idx >= animation.step_array.size()) {
						idx = 0;
					}

					if (first) {
						last_angles = controller.target_joint_angles;
						first = false;
						progress = 0;
					}

					if (progress >= 1) {
						idx++;
						progress = 0;
						first = true;
					} else {
						if (!animation.step_array[idx].enable_ik_trajectory) {
							for (int i = 0; i < 6; i++)
								controller.target_joint_angles[i] = last_angles[i] + (animation.step_array[idx].target_angles[i] - last_angles[i]) * EASE_FUNC(progress, 2.5);
							progress += 1.0f / animation.step_array[idx].progress_len;
						} else {

						}
					}

					if (idx >= animation.step_array.size()) {
						if (animation.loop) {
							idx = 0;
						} else {
							animator.animationstate = STOP;
						}
					}

					controller.ik_target = Kinematics::GetForwardKinematics(controller.target_joint_angles);
				} else {
					first = true;
				}

				if (/* animator.animationstate == PLAY &&  */false) {
					if (idx >= generated.size()) {
						idx = 0;
					}

					if (idx == 0 && generated.size()) {
						static JointAngles last_angles, next_angles;
						static float len = 0;

						if (progress == 0) {
							last_angles = controller.target_joint_angles;
							bool is_valid;
							next_angles = Kinematics::GetInverseKinematics(is_valid, generated[idx].position);
							next_angles.gripper_control = generated[idx].gripper_ctrl;
							next_angles.gripper_rotate = generated[idx].gripper_rot;
							len = glm::length(glm::vec3(Kinematics::GetForwardKinematics(next_angles)) - glm::vec3(Kinematics::GetForwardKinematics(last_angles)));
						}

						if (progress >= 1 || len == 0) {
							progress = 0;
							idx++;
						} else {
							for (int i = 0; i < 6; i++)
								controller.target_joint_angles[i] = last_angles[i] + (next_angles[i] - last_angles[i]) * EASE_FUNC(progress, 2.5);

							controller.ik_target = animation.generated_positions[idx].position;
							progress += 1.0f / len;
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
						controller.ik_target = animation.generated_positions[idx].position;

						if (current_time - last_time >= controller.speed) {
							last_time = current_time;
							idx++;
						}

						if (idx >= generated.size()) {
							if (animation.loop)
								idx = 0;
							else
								animator.animationstate = PAUSE;
						}
					}
				}
			}
			if (animator.animationstate == STOP) {
				idx = 0;
			}
		}

		EaseMovement();

		RenderAxisLine(true, false, true);
		RenderGrid();
		RenderLRA(controller.render_angles);
		canvas.Render();
		vision.Update();

		if (lucy::Events::IsKeyChord({ SDL_SCANCODE_LCTRL, SDL_SCANCODE_S })) {
			SerializeAnimator("animator.yaml");
		}

		camera.framebuffer->UnBind();

		{
			auto& serial_comm = registry.store<SerialCommunication>();

			serial_comm.TransmitServoAngles(controller.render_angles);
		}
	}
}

void lra::EditorUpdateArm() {
	panel::PanelUpdate();
}
