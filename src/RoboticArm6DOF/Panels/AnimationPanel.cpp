#include "Panel.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <Lucy/Registries/Registry.h>
#include <RoboticArm6DOF/Animator.h>

static auto& registry = Registry::Instance();
static auto treenode_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

void lra::panel::Animator() {
	auto& animator = registry.store<lra::Animator>();
	auto& controller = registry.store<lra::Controller>();
	static UTIL_UUID selected_animation = UTIL_NULL_UUID;

	if (ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoTitleBar)) {
		static float slider = 0.25;
		// ImGui::SliderFloat("Column Size", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnOffset(1, offset);

		ImGui::BeginChild("#0");

		if (animator.animtationstate == PLAY) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 1, 0.5 });
			if (ImGui::Button("PAUSE")) {
				animator.animtationstate = PAUSE;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 0.5 });
			if (ImGui::Button("STOP")) {
				animator.animtationstate = STOP;
			}
			ImGui::PopStyleColor();
		}
		if (animator.animtationstate == PAUSE || animator.animtationstate == STOP) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 1, 0, 0.5 });
			if (ImGui::Button("PLAY")) {
				animator.animtationstate = PLAY;
			}
			ImGui::PopStyleColor();
		}

		// if (ImGui::Button("New Animation")) {
			
		// }

		if (ImGui::BeginTable("Animation##0203", 1, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
			for (auto& pair: animator.animation_registry) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				ImGui::SanitisedInputText(("Name##" + std::to_string(pair.first)).c_str(), pair.second.name);
				ImGui::Checkbox(("Loop##" + std::to_string(pair.first)).c_str(), &pair.second.animation.loop);
				if (ImGui::Button(("View##" + std::to_string(pair.first)).c_str())) {
					selected_animation = pair.first;
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

		ImGui::NextColumn();

		ImGui::BeginChild("#1");
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		if (selected_animation != UTIL_NULL_UUID) {
			if (ImGui::BeginTable("View##0203", 1 + 6 + 3 + 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders)) {
				auto& animation = animator.animation_registry[selected_animation].animation;
				auto& name = animator.animation_registry[selected_animation].name;

				int f = 0, idx = 0;
				for (auto& step: animation.step_array) {
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

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(std::to_string(idx).c_str());

					for (int i = 0; i < 6; i++) {
						ImGui::TableSetColumnIndex(i + 1);
						ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 1));
						if (ImGui::DragFloat(("##" + std::to_string(f++)).c_str(), &step.target_angles[i], 0.1, 0, 180)) {
							step.target_position = Kinematics::GetForwardKinematics(step.target_angles, step.lra_dimension);
						}
					}

					for (int i = 0; i < 3; i++) {
						ImGui::TableSetColumnIndex(i + 7);
						ImGui::SetNextItemWidth(ImGui::GetColumnWidth(i + 7));

						float temp = step.target_position[i];
						if (ImGui::DragFloat(("##" + std::to_string(f++)).c_str(), &step.target_position[i], 0.1)) {
							bool is_valid;
							auto angles = Kinematics::GetInverseKinematics(is_valid, step.target_position, step.lra_dimension);
							if (is_valid) {
								step.target_angles = angles;
							} else {
								step.target_position[i] = temp;
							}
						}
					}

					ImGui::TableSetColumnIndex(10);
					ImGui::SetNextItemWidth(ImGui::GetColumnWidth(10));
					if (ImGui::Button(("RESET##" + std::to_string(idx)).c_str())) {
						step.target_angles = controller.target_joint_angles;
						step.target_position = Kinematics::GetForwardKinematics(step.target_angles, step.lra_dimension);
					}
					if (ImGui::Button(("VIEW##" + std::to_string(idx)).c_str())) {
						// step.target_angles = controller.target_joint_angles;
						// step.target_position = Kinematics::GetForwardKinematics(step.target_angles, step.lra_dimension);
						controller.target_joint_angles = step.target_angles;
					}

					// ImGui::TableSetColumnIndex(11);
					// ImGui::SetNextItemWidth(ImGui::GetColumnWidth(11));

					// auto temp = step.mode;
					// ImGui::EnumComboLogic("Select Mode", { "WRITING", "PICKING" }, step.mode);
					// if (step.mode != temp) {
					// 	step.target_position = controller.ik_target;
					// 	bool is_valid;
					// 	step.target_angles = Kinematics::GetForwardKinematics(is_valid, step.target_position, step.lra_dimension);
					// }
					idx++;
				}

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				if (ImGui::Button("+", { ImGui::GetColumnWidth(), 0 })) {
					AnimationStep step;
					step.target_position = controller.ik_target;
					step.target_angles = controller.target_joint_angles;
					step.lra_dimension = controller.lra_dimension;
					step.mode = controller.mode;
					step.phi = controller.phi;
					animation.step_array.push_back(step);
				}
				ImGui::EndTable();
			}
		}

		ImGui::EndChild();
	}
	ImGui::End();
}
