#include "Panel.h"
#include <iostream>
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <Lucy/Registries/Registry.h>
#include <RoboticArm6DOF/Animator.h>

static auto& registry = Registry::Instance();
static auto treenode_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

void lra::panel::AnimationPanel() {
	static bool open_view_gen = false;
	auto& animator = registry.store<lra::Animator>();
	auto& controller = registry.store<lra::Controller>();

	if (ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoTitleBar)) {
		static float slider = 0.17;

		// ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnOffset(1, offset);

		{
			ImGui::BeginChild("#0");

			float x_padding = (ImGui::GetWindowContentRegionMax().x - ImGui::GetContentRegionMax().x) / 2;

			if (animator.animationstate == PLAY) {
				ImGui::PushStyleColor(ImGuiCol_Button, { 1, 1, 0, 0.5 });
				if (ImGui::Button("PAUSE", { ImGui::GetContentRegionAvail().x / 2 - x_padding, 0 })) {
					animator.animationstate = PAUSE;
				}
				ImGui::PopStyleColor();
			}
			if (animator.animationstate == PAUSE || animator.animationstate == STOP) {
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 1, 0, 0.5 });
				if (ImGui::Button("PLAY", { ImGui::GetContentRegionAvail().x / 2, 0 })) {
					animator.animationstate = PLAY;
				}
				ImGui::PopStyleColor();
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 0.5 });
			if (ImGui::Button("STOP", { ImGui::GetContentRegionAvail().x, 0 })) {
				animator.animationstate = STOP;
			}
			ImGui::PopStyleColor();

			if (ImGui::BeginTable("Animation##0203", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Name");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("Loop");
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("IK Type");

				int idx = 0;
				for (auto& pair: animator.animation_registry) {
					if (pair.second.name.starts_with("__##")) continue;

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					if (ImGui::Selectable((pair.second.name + "##" + std::to_string(idx)).c_str())) {
						animator.selected_animation = pair.first;
					}

					ImGui::TableSetColumnIndex(1);
					if (ImGui::Button((pair.second.animation.loop) ? ("True##" + std::to_string(idx)).c_str() : ("False##" + std::to_string(idx)).c_str(), { ImGui::GetColumnWidth(1), 0 })) {
						pair.second.animation.loop = !pair.second.animation.loop;
					}

					ImGui::TableSetColumnIndex(2);
					ImGui::SetNextItemWidth(ImGui::GetColumnWidth(2));
					ImGui::EnumComboLogic(("##" + std::to_string(idx)).c_str(), { "WRITING", "PICKING" }, pair.second.animation.is_ik_picking);
					idx++;
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
					animator.NewAnimation("", {});
				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}

		ImGui::NextColumn();

		{
			bool regenrate = false;

			ImGui::BeginChild("#1", { 0, 0 }, false);
		
			if (animator.selected_animation != LUCY_NULL_UUID) {
				auto& animation = animator.animation_registry[animator.selected_animation].animation;
				auto& name = animator.animation_registry[animator.selected_animation].name;

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3);
				ImGui::SanitisedInputText("Name", name);

				float w = ImGui::GetContentRegionAvail().x / 3;

				ImGui::SameLine();
				ImGui::SetNextItemWidth(w);
				if (ImGui::Button("Regenerate")) {
					regenrate = true;
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(w);
				if (ImGui::Button("View Gen")) {
					open_view_gen = !open_view_gen;
				}

				ImGui::SameLine();
				ImGui::SetNextItemWidth(w);
				if (ImGui::Button("Show Path")) {
					animator.render_path = !animator.render_path;
				}

				ImGui::SameLine();
				ImGui::SetNextItemWidth(w);
				if (ImGui::Button((animator.trace_path) ? "Trace Off": "Trace On")) {
					animator.trace_path = !animator.trace_path;
				}

				if (ImGui::BeginTable("View##0203", 1 + 6 + 3 + 2 + 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0 + 1);
					ImGui::Text("Base");
					ImGui::TableSetColumnIndex(1 + 1);
					ImGui::Text("Arm");
					ImGui::TableSetColumnIndex(2 + 1);
					ImGui::Text("Elbow");
					ImGui::TableSetColumnIndex(3 + 1);
					ImGui::Text("Wrist");
					ImGui::TableSetColumnIndex(4 + 1);
					ImGui::Text("G. Rot");
					ImGui::TableSetColumnIndex(5 + 1);
					ImGui::Text("G. Cont");

					ImGui::TableSetColumnIndex(0 + 7);
					ImGui::Text("X.x");
					ImGui::TableSetColumnIndex(1 + 7);
					ImGui::Text("X.y");
					ImGui::TableSetColumnIndex(2 + 7);
					ImGui::Text("X.z");

					ImGui::TableSetColumnIndex(10);
					ImGui::Text("Steps");

					ImGui::TableSetColumnIndex(11);
					ImGui::Text("IK");

					int idx = 0;
					static int selected_step = 0;
					for (auto& step: animation.step_array) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						if (ImGui::Button(std::to_string(idx).c_str())) {
							ImGui::OpenPopup("SH - Step");
							selected_step = idx;
						}

						bool change = false;
						for (int i = 0; i < 6; i++) {
							ImGui::TableSetColumnIndex(i + 1);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 1));
							change |= ImGui::DragFloat(("##" + std::to_string(idx)).c_str(), &step.target_angles[i], 0.1);
						}

						if (change) {
							step.target_position = Kinematics::GetForwardKinematics(step.target_angles);
							animator.temp_change = true;
						}

						for (int i = 0; i < 3; i++) {
							ImGui::TableSetColumnIndex(i + 7);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 7));

							float temp = step.target_position[i];
							if (ImGui::DragInt(("##43254" + std::to_string(idx)).c_str(), &step.target_position[i], 1)) {
								bool is_valid;
								auto angles = (animation.is_ik_picking) ? Kinematics::GetInverseKinematics(is_valid, step.target_position): Kinematics::GetInverseKinematics(is_valid, step.target_position, step.phi);
								if (is_valid) {
									step.target_angles = angles;
									regenrate = true;
								} else {
									step.target_position[i] = temp;
								}
							}

							if (step.target_position.x <= 0) {
								step.target_position.x = 0;
							}
						}

						ImGui::TableSetColumnIndex(10);
						ImGui::SetNextItemWidth(ImGui::GetColumnWidth(10));
						ImGui::DragFloat(("##tsd" + std::to_string(idx)).c_str(), &step.progress_len, 1, 0);

						ImGui::TableSetColumnIndex(11);
						ImGui::Checkbox(("##_jsia" + std::to_string(idx)).c_str(), &step.enable_ik_trajectory);

						ImGui::TableSetColumnIndex(12);
						if (ImGui::Button(("RESET##" + std::to_string(idx)).c_str(), { ImGui::GetColumnWidth(12), 0 })) {
							auto pos = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);
							bool is_valid;
							auto angles = (controller.is_ik_picking) ? Kinematics::GetInverseKinematics(is_valid, pos): Kinematics::GetInverseKinematics(is_valid, pos, controller.ik_phi);
							if (is_valid) {
								step.target_angles = angles;
								step.target_position = pos;
								step.target_angles.gripper_control = controller.target_joint_angles.gripper_control;
								step.target_angles.gripper_rotate = controller.target_joint_angles.gripper_rotate;
							}

							regenrate = true;
							animator.temp_change = true;
						}

						ImGui::TableSetColumnIndex(13);

						static JointAngles last_angles;
						static glm::ivec3 last_pos;
						static bool toggled_idx = 0;
						static bool toggled = false;
						static bool first = false;
						static AnimtationState state;

						ImGui::Button(("VIEW##" + std::to_string(idx)).c_str(), { ImGui::GetColumnWidth(13), 0 });

						bool is_hovering = ImGui::IsItemHovered();
						bool is_clicked = is_hovering && lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT);

						if ((!is_hovering || !is_clicked) && toggled && toggled_idx == idx) {
							if (controller.ik_enable) {
								controller.ik_target = last_pos;
							} else {
								controller.target_joint_angles = last_angles;
							}
							animator.animationstate = state;
							toggled = false;
						}
						if (is_clicked && !toggled) {
							toggled = true;
							toggled_idx = idx;

							if (controller.ik_enable) {
								last_pos = controller.ik_target;
								controller.ik_target = step.target_position;
							} else {
								last_angles = controller.target_joint_angles;
								controller.target_joint_angles = step.target_angles;
							}

							state = animator.animationstate;
							animator.animationstate = PAUSE;
						}

						// if (toggle && !lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT) && idx == toggled_idx) {
						// 	toggle = false;
						// 	controller.target_joint_angles = last_angles;
						// }

						// if (ImGui::IsItemHovered() && idx == toggled_idx && toggle) {
						// 	controller.target_joint_angles = step.target_angles;
						// }

						// if (!ImGui::IsItemHovered() || !lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT)) {
						// 	controller.target_joint_angles = last_angles;
						// 	toggle = false;
						// }

						idx++;
					}

					if (ImGui::BeginPopup("SH - Step")) {
						if (ImGui::Button("Delete")) {
							animation.step_array.erase(animation.step_array.begin() + selected_step);
						}

						ImGui::EndPopup();
					}

					if (regenrate) {
						animation.Generate();
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
						AnimationStep step;

						auto pos = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);
						bool is_valid;
						auto angles = (controller.is_ik_picking) ? Kinematics::GetInverseKinematics(is_valid, pos): Kinematics::GetInverseKinematics(is_valid, pos, controller.ik_phi);
						if (is_valid) {
							step.target_angles = angles;
							step.target_angles.gripper_control = controller.target_joint_angles.gripper_control;
							step.target_angles.gripper_rotate = controller.target_joint_angles.gripper_rotate;
							step.target_position = pos;
							if (animation.step_array.size() == 0)
								animation.is_ik_picking = controller.is_ik_picking;
							step.phi = controller.ik_phi;
							if (idx != 0)
								step.enable_ik_trajectory = animation.step_array[idx - 1].enable_ik_trajectory;
						}

						animation.step_array.push_back(step);
					}
					ImGui::EndTable();
				}
			}

			ImGui::EndChild();
		}

		if (animator.selected_animation != LUCY_NULL_UUID && open_view_gen) {
			if (animator.animation_registry[animator.selected_animation].animation.generated_positions.size()) {
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 10 });
				ImGui::Begin("View Generated", nullptr, ImGuiWindowFlags_NoTitleBar);
				ImGui::PopStyleVar();

				{
					int idx = 0;
					if (ImGui::BeginTable("View##0203", 5, ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadInnerX)) {
						for (auto& position: animator.animation_registry[animator.selected_animation].animation.generated_positions) {
							ImGui::TableNextRow();

							for (int i = 0; i < 3; i++) {
								ImGui::TableSetColumnIndex(i);
								ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
								ImGui::DragInt(("##" + std::to_string(i) + "#" + std::to_string(idx)).c_str(), &position.position[i], 0);
							}

							ImGui::TableSetColumnIndex(3);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
							ImGui::DragFloat(("##" + std::to_string(3) + "#" + std::to_string(idx)).c_str(), &position.gripper_rot, 0);

							ImGui::TableSetColumnIndex(4);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
							ImGui::DragFloat(("##" + std::to_string(4) + "#" + std::to_string(idx)).c_str(), &position.gripper_ctrl, 0);

							idx++;
						}
						ImGui::EndTable();
					}
				}

				ImGui::End();
			}
		}
	}
	ImGui::End();
}
