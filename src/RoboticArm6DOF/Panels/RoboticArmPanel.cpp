#include "Panel.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <RoboticArm6DOF/Controller.h>
#include <RoboticArm6DOF/Kinematics.h>
#include <iostream>

static auto& registry = Registry::Instance();
static auto treenode_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

void lra::panel::RoboticArmPanel() {
	auto& controller = registry.store<lra::Controller>();

	if (ImGui::Begin("RoboticArm", nullptr, ImGuiWindowFlags_NoTitleBar/* | ImGuiWindowFlags_AlwaysAutoResize */)) {
		if (ImGui::TreeNodeEx("Properties", treenode_flags)) {
			if (ImGui::Button((controller.enable_smoothing) ? "Disable Smoothing": "Enable Smooting")) {
				controller.enable_smoothing = !controller.enable_smoothing;
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Joint Angles", treenode_flags)) {
			static bool is_slider = true;

			ImGui::SliderDragFloat("Base", &controller.target_joint_angles.base, 0.1, 0, 180, is_slider);
			ImGui::SliderDragFloat("Arm", &controller.target_joint_angles.arm, 0.1, 0, 180, is_slider);
			ImGui::SliderDragFloat("Elbow", &controller.target_joint_angles.elbow, 0.1, 0, 180, is_slider);
			ImGui::SliderDragFloat("Wrist", &controller.target_joint_angles.wrist, 0.1, 0, 270, is_slider);

			ImGui::SliderDragFloat("Gripper Rotation", &controller.target_joint_angles.gripper_rotate, 0.1, 0, 180, is_slider);
			ImGui::SliderDragFloat("Gripper Control", &controller.target_joint_angles.gripper_control, 0.1, 0, 180, is_slider);

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Joint Length", treenode_flags)) {
			ImGui::DragFloat("Arm", &controller.lra_dimension.arm, 0.1, 0);
			ImGui::DragFloat("Elbow", &controller.lra_dimension.elbow, 0.1, 0);
			ImGui::DragFloat("Wrist", &controller.lra_dimension.wrist, 0.1, 0);

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Kinematics", treenode_flags)) {
			if (controller.ik_enable) {
				ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 0.7 });
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 1, 0, 0.7 });
			}

			if (ImGui::Button((controller.ik_enable) ? "Disable" : " Enable"))
				controller.ik_enable = !controller.ik_enable;

			ImGui::EnumComboLogic("Type", { "WRITING", "PICKING" }, controller.is_ik_picking);

			ImGui::PopStyleColor();

			static glm::vec3 temp;
			ImGui::DragInt3("IK", &controller.ik_target[0], 1);

			ImGui::SliderDragFloat("phi", &controller.ik_phi, 0.1, -90, 90);

			bool is_valid;
			Kinematics::GetInverseKinematics(is_valid, controller.ik_target, controller.lra_dimension);
			if (!is_valid) {
				controller.ik_target = temp;
			} else {
				temp = controller.ik_target;
			}
			ImGui::DragInt3("FK", &controller.fk_result[0], 0.0);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}
