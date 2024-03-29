#include "Panel.h"
#include <Lucy/Lucy.h>
#include <RoboticArm6DOF/ServoController.h>
#include <Lucy/imgui_lucy_impl.h>

static auto& registry = Registry::Instance();

void lra::panel::SerialCommunicationPanel() {
	auto& serial_comm = registry.store<SerialCommunication>();

	if (ImGui::Begin("Servo PWM", nullptr, ImGuiWindowFlags_NoTitleBar)) {
		if (ImGui::Button("Reconnect")) {
			serial_comm.Reconnect();
		}

		ImGui::SanitisedInputText("PORT", serial_comm.port_name);

		// ImGui::TextColored(util::serial::is_port_connected() ? ImVec4(0.0f, 1.0f, 0.0f, 0.7f): ImVec4(1.0f, 0.0f, 0.0f, 0.7f), "CONNECTED");
		if (util::serial::is_port_connected()) {
			ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 0.7f}, ("CONNECTED: " + serial_comm.connect_port).c_str());
		} else {
			ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 0.7f}, "DISCONNECTED");
		}

		// if (ImGui::BeginTable("View##0203", 3, ImGuiTableFlags_ScrollY)) {
		// 	int f = 0;
		// 	for (int i = 0; i < 6; i++) {
		// 		ImGui::TableNextRow();
		// 		ImGui::TableSetColumnIndex(0);
		// 		ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
		// 		ImGui::Text(("  " + std::to_string(i)).c_str());

		// 		for (auto& pair: servo_controller.mapped_pulse[i]) {
		// 			ImGui::TableSetColumnIndex(1);
		// 			int temp = pair.first;
		// 			ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
		// 			ImGui::DragInt(("##0" + std::to_string(i) + "" + std::to_sdtring(f++)).c_str(), &temp, 0);
		// 		}

		// 		for (auto& pair: servo_controller.mapped_pulse[i]) {
		// 			ImGui::TableSetColumnIndex(2);
		// 			int temp = pair.second;
		// 			ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
		// 			ImGui::DragInt(("##0" + std::to_string(i) + "" + std::to_string(f++)).c_str(), &temp, 1);
		// 			pair.second = temp;
		// 		}
		// 	}
		// 	ImGui::EndTable();
		// }
	}
	ImGui::End();
}
