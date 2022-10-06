#pragma once

#include <Lucy/Editor.h>
#include <Lucy/Lucy.h>
#include "Panel.h"

void lra::panel::PropertiesPanel() {
	ImGui::SetNextWindowBgAlpha(WindowAlpha);

	if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::IsWindowHovered())
			PanelManager::IsHovering = true;
	}
	ImGui::End();
}