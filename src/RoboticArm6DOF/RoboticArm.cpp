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
#include <Lucy/Math.h>
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
// #include "Vision.h"

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
	// auto& vision = registry.store<ComputerVision>();
	auto& materialregistry = registry.store<lucy::MaterialRegistry>();
	auto& lightregistry = registry.store<lucy::LightRegistry>();

	registry.store<lucy::LightRegistry>().Insert("Default");
	registry.store<lucy::MaterialRegistry>().Insert("LRA");

	DeSerializeAnimator("animator.yaml");

	IntializeRenderer();

	// vision.Initialize();

	camera.position = { 0, 0, 1000 };
}

void lra::RuntimeUpdateArm() {
	auto& canvas = registry.store<Canvas>();
	auto& camera = registry.store<lucy::Camera>();
	auto& window = registry.store<lucy::Window>();
	auto& controller = registry.store<Controller>();
	auto& animator = registry.store<Animator>();
	// auto& vision = registry.store<ComputerVision>();

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
			static float progress = 0;
			static uint32_t progress_step = 0;
			static bool first = true;

			if (animator.selected_animation != LUCY_NULL_UUID) {
				auto& animation = animator.animation_registry[animator.selected_animation].animation;
				auto& generated = animation.GetGenerated();
				static LUCY_UUID last_animation = LUCY_NULL_UUID;

				if (animator.selected_animation != last_animation) {
					animator.curr_idx = 0;
					progress = 0;
					progress_step = 0;
					last_animation = animator.selected_animation;
				}

				if (animator.render_path) {
					TraceAnimationPoints(0, generated.size() - 1, &animation, { 0, 1, 0, 1 });
					// Trace(&animation, animator.curr_idx, progress, false);
				}

				if (animator.trace_path) {
					std::cout << animation.step_array[animator.curr_idx].progress_len * progress << ' ' << animator.curr_idx << ' ' << first << '\n';
					if (animator.curr_idx < generated.size())
						TraceAnimationPoints(progress_step, generated.size() - animator.curr_idx, &animation, { 1, 1, 0, 1 });
					// else
					// 	TraceAnimationPoints(0, generated.size() - 1, &animation, { 0, 1, 0, 1 });
				}

				if (animator.animationstate == PLAY) {
					static JointAngles last_angles;

					if (animator.curr_idx >= animation.step_array.size()) {
						animator.curr_idx = 0;
					}

					if (first) {
						last_angles = controller.target_joint_angles;
						first = false;
						progress = 0;
					}

					if (progress >= 1) {
						animator.curr_idx++;
						progress = 0;
						first = true;
					} else {
						if (!animation.step_array[animator.curr_idx].enable_ik_trajectory) {
							for (int i = 0; i < 6; i++)
								controller.target_joint_angles[i] = last_angles[i] + (animation.step_array[animator.curr_idx].target_angles[i] - last_angles[i]) * EASE_FUNC(progress, 2.5);
						} else {
							auto curr_pos = Kinematics::GetForwardKinematics(last_angles);

							glm::vec3 difference = animation.step_array[animator.curr_idx].target_position - curr_pos;
							glm::vec3 dir = glm::normalize(difference);
							float dist = glm::length(difference) * progress;
							glm::vec3 target = glm::vec3(curr_pos) + dir * dist;

							bool is_vaild = false;
							JointAngles angles = Kinematics::GetInverseKinematics(is_vaild, target);
							if (is_vaild) controller.target_joint_angles = angles;

							for (int i = 4; i < 6; i++)
								controller.target_joint_angles[i] = last_angles[i] + (animation.step_array[animator.curr_idx].target_angles[i] - last_angles[i]) * EASE_FUNC(progress, 2.5);
						}

						progress += 1.0f / animation.step_array[animator.curr_idx].progress_len;
						// progress_step += (1 & (animator.curr_idx >= 1));
					}

					if (animator.curr_idx >= animation.step_array.size()) {
						if (animation.loop) {
							animator.curr_idx = 0;
						} else {
							animator.animationstate = STOP;
						}
					}

					controller.ik_target = Kinematics::GetForwardKinematics(controller.target_joint_angles);
				} else {
					first = true;
				}
			}
			if (animator.animationstate == STOP) {
				animator.curr_idx = 0;
				progress_step = 0;
			}
		}

		EaseMovement();

		RenderAxisLine(true, false, true);
		RenderGrid();
		RenderLRA(controller.render_angles);
		canvas.Render();
		// vision.Update();

		if (lucy::Events::IsKeyChord({ SDL_SCANCODE_LCTRL, SDL_SCANCODE_S })) {
			SerializeAnimator("animator.yaml");
		}

		camera.framebuffer->UnBind();

		{
			auto& serial_comm = registry.store<SerialCommunication>();

			// serial_comm.TransmitServoAngles(controller.render_angles);
		}
	}

	if (lucy::Events::IsQuittable()) {
		SerializeAnimator("animator.yaml");
	}
}

void lra::EditorUpdateArm() {
	panel::PanelUpdate();
}
