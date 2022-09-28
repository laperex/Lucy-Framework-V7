#include "ArmControl.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <RoboticArm6DOF/Light.h>
#include <RoboticArm6DOF/Material.h>
#include <RoboticArm6DOF/Controller.h>

static auto& registry = Registry::Instance();

void lra::panel::ArmControl() {
	auto& controller = registry.store<Controller>();

	if (ImGui::Begin("Controller")) {
		ImGui::SliderFloat("Base", &controller.target_joint_angles.base, 0, 180);
		ImGui::SliderFloat("Arm", &controller.target_joint_angles.arm, 0, 180);
		ImGui::SliderFloat("Elbow", &controller.target_joint_angles.elbow, 0, 180);
		ImGui::SliderFloat("Wrist", &controller.target_joint_angles.wrist, 0, 180);
		ImGui::SliderFloat("Gripper Rotation", &controller.target_joint_angles.gripper_rotate, 0, 180);
		ImGui::SliderFloat("Gripper Control", &controller.target_joint_angles.gripper_control, 0, 180);

		ImGui::Checkbox("Enable Invers Kinematics", &controller.ik_enable);
	}
	ImGui::End();
}

