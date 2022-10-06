#pragma once

namespace lra::panel {
	#define WindowAlpha (0.5)
	#define IS_WINDOW_HOVERED if (ImGui::IsWindowHovered() && ImGui::IsItemHovered()) lra::panel::PanelManager::IsHovering = true

	struct PanelManager {
		static bool IsHovering;
	};

	void RendererPanel();
	void RoboticArmPanel();
	void PropertiesPanel();
	void AnimationPanel();
	void SerialCommunicationPanel();

	void PanelUpdate();
}