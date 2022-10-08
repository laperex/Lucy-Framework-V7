#include "Panel.h"
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
			// ImGui::Columns(2, 0, false);
			// ImGui::SetColumnOffset(1, ImGui::GetContentRegionAvail().x / 2);

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

			if (ImGui::BeginTable("Animation##0203", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Name");
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("Loop");

				int idx = 0;
				for (auto& pair: animator.animation_registry) {
					if (pair.second.name.starts_with("__##")) continue;

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					if (ImGui::Selectable((pair.second.name + "##" + std::to_string(idx)).c_str())) {
						animator.selected_animation = pair.first;
					}

					ImGui::TableSetColumnIndex(1);
					if (ImGui::Button((pair.second.animation.loop) ? ("True##" + std::to_string(idx++)).c_str() : ("False##" + std::to_string(idx++)).c_str(), { ImGui::GetColumnWidth(2), 0 })) {
						pair.second.animation.loop = !pair.second.animation.loop;
					}
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
					animation.Generate();
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(w);
				if (ImGui::Button("View Gen")) {
					open_view_gen = !open_view_gen;
				}

				if (ImGui::BeginTable("View##0203", 1 + 6 + 3 + 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
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

					int f = 0, idx = 0;
					for (auto& step: animation.step_array) {
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(std::to_string(idx).c_str());

						for (int i = 0; i < 6; i++) {
							ImGui::TableSetColumnIndex(i + 1);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 1));
							if (ImGui::DragFloat(("##" + std::to_string(f++)).c_str(), &step.target_angles[i], 0)) {
								// step.target_position = Kinematics::GetForwardKinematics(step.target_angles, controller.lra_dimension);
								// step.target_angles = 
							}
						}

						for (int i = 0; i < 3; i++) {
							ImGui::TableSetColumnIndex(i + 7);
							ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 7));

							float temp = step.target_position[i];
							if (ImGui::DragInt(("##" + std::to_string(f++)).c_str(), &step.target_position[i], 1)) {
								bool is_valid;
								auto angles = Kinematics::GetInverseKinematics(is_valid, step.target_position, controller.lra_dimension);
								if (is_valid) {
									step.target_angles = angles;
								} else {
									step.target_position[i] = temp;
								}
							}

							if (step.target_position.x <= 0) {
								step.target_position.x = 0;
							}
						}

						ImGui::TableSetColumnIndex(10);
						if (ImGui::Button(("RESET##" + std::to_string(idx)).c_str(), { ImGui::GetColumnWidth(10), 0 })) {
							auto pos = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);
							bool is_valid;
							auto angles = Kinematics::GetInverseKinematics(is_valid, pos);
							if (is_valid) {
								step.target_angles = angles;
								step.target_position = pos;
							}
						}
						ImGui::TableSetColumnIndex(11);
						if (ImGui::Button(("VIEW##" + std::to_string(idx)).c_str(), { ImGui::GetColumnWidth(11), 0 })) {
							controller.target_joint_angles = step.target_angles;
						}

						idx++;
					}

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
						AnimationStep step;

						auto pos = Kinematics::GetForwardKinematics(controller.target_joint_angles, controller.lra_dimension);
						bool is_valid;
						auto angles = Kinematics::GetInverseKinematics(is_valid, pos);
						if (is_valid) {
							step.target_angles = angles;
							step.target_position = pos;
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
