#include "Panel.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <RoboticArm6DOF/Controller.h>
#include <iostream>

static auto& registry = Registry::Instance();
static auto treenode_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

void lra::panel::Controller() {
	auto& controller = registry.store<lra::Controller>();

	if (ImGui::Begin("Controller", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		static bool is_slider = true;

		ImGui::SliderDragFloat("Base", &controller.target_joint_angles.base, 0.1, 0, 180, is_slider);
		ImGui::SliderDragFloat("Arm", &controller.target_joint_angles.arm, 0.1, 0, 180, is_slider);
		ImGui::SliderDragFloat("Elbow", &controller.target_joint_angles.elbow, 0.1, 0, 180, is_slider);
		ImGui::SliderDragFloat("Wrist", &controller.target_joint_angles.wrist, 0.1, 0, 180, is_slider);

		ImGui::SliderDragFloat("Gripper Rotation", &controller.target_joint_angles.gripper_rotate, 0.1, 0, 180, is_slider);
		ImGui::SliderDragFloat("Gripper Control", &controller.target_joint_angles.gripper_control, 0.1, 0, 180, is_slider);
	}

	ImGui::End();
}

void lra::panel::Kinematics() {
	auto& controller = registry.store<lra::Controller>();

	if (ImGui::Begin("Kinematics", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::TreeNodeEx("Joint Length", treenode_flags)) {
			ImGui::DragFloat("Arm", &controller.lra_dimension.arm, 0.1, 0);
			ImGui::DragFloat("Elbow", &controller.lra_dimension.elbow, 0.1, 0);
			ImGui::DragFloat("Wrist", &controller.lra_dimension.wrist, 0.1, 0);

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Inverse Kinematics", treenode_flags)) {
			{
				if (controller.enable_ik)
					ImGui::PushStyleColor(ImGuiCol_Button, { 1, 0, 0, 0.7 });
				else
					ImGui::PushStyleColor(ImGuiCol_Button, { 0, 1, 0, 0.7 });

				if (ImGui::Button((controller.enable_ik) ? "Disable" : " Enable"))
					controller.enable_ik = !controller.enable_ik;

				ImGui::PopStyleColor();
			}

			glm::vec3 max = glm::normalize(controller.ik_target) * controller.lra_dimension.Total();

			ImGui::DragFloat3("End Affector", &controller.ik_target[0], 0.1);

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Forward Kinematics", treenode_flags)) {
			ImGui::DragFloat3("Position", &controller.fk_result[0], 0.0);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}
