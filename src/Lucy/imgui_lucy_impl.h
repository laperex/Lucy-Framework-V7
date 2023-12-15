#pragma once

#include <vector>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <map>
#include <functional>

namespace ImGui {
	void SanitisedInputText(const char* label, std::string& text);
	void TransformFloat3(const char* label, float *v, float resetValue = 0.0f, float columnWidth = 100.0f);
	bool in_expections(std::string value, const std::vector<std::string>& exceptions);

	template <typename T>
	void EnumComboLogic(const char* name, const std::vector<std::string>& total_modes, T& idx, const std::vector<std::string>& exceptions = {}, ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton) {
		if (ImGui::BeginCombo(name, total_modes[idx].c_str(), flags)) {
			for (int i = 0; i < total_modes.size(); i++) {
				if (!in_expections(total_modes[i], exceptions))
					if (total_modes[i] != total_modes[idx])
						if (ImGui::Selectable(total_modes[i].c_str(), true))
							idx = (T)i;
			}
			ImGui::EndCombo();
		}
	}

	bool SliderDragFloat(const char* label, float* v, float speed, float min, float max);
	bool SliderDragFloat2(const char* label, float* v, float speed, float min, float max);
	bool SliderDragFloat3(const char* label, float* v, float speed, float min, float max);
	bool SliderDragFloat(const char* label, float* v, float speed, float min, float max, bool& is_slider);
	bool SliderDragFloat2(const char* label, float* v, float speed, float min, float max, bool& is_slider);
	bool SliderDragFloat3(const char* label, float* v, float speed, float min, float max, bool& is_slider);
	bool SliderDragInt(const char* label, int* v, int speed, int min, int max);
	bool SliderDragInt2(const char* label, int* v, int speed, int min, int max);
	bool SliderDragInt3(const char* label, int* v, int speed, int min, int max);
	bool SliderDragInt(const char* label, int* v, int speed, int min, int max, bool& is_slider);
	bool SliderDragInt2(const char* label, int* v, int speed, int min, int max, bool& is_slider);
	bool SliderDragInt3(const char* label, int* v, int speed, int min, int max, bool& is_slider);

	void ColorEdit3(const char* label, float* v, ImVec2 size);
}

namespace ImGui::Theme {
	void EmbraceTheDarkness();
	void VisualStudio();
	void RedDark();
}