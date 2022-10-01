#include "Panel.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <Lucy/Registries/Registry.h>

static auto& registry = Registry::Instance();

void lra::panel::Lighting() {
	if (ImGui::Begin("Lighting and Material", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		for (auto& pair: registry.store<lucy::MaterialRegistry>().storage_hashmap) {
			auto& material = pair.second.store;

			if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::DragFloat3("Ambient", &material.ambient[0], 0.05, 0, 1);
				ImGui::DragFloat3("Diffuse", &material.diffuse[0], 0.05, 0, 1);
				ImGui::DragFloat3("Specular", &material.specular[0], 0.05, 0, 1);
				ImGui::DragFloat("Shininess", &material.shininess, 0.05, 0, 256);
			
				ImGui::TreePop();
			}

			// if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
			// 	ImGui::DragFloat("Ambient", &light.ambient, 0.05, 0, 1);
			// 	ImGui::DragFloat("Diffuse", &light.diffuse, 0.05, 0, 1);
			// 	ImGui::DragFloat("Specular", &light.specular, 0.1, 0, 1);

			// 	ImGui::ColorEdit4("Color", &light.color[0], ImGuiColorEditFlags_NoInputs);

			// 	ImGui::TreePop();
			// }
		}
	}
	ImGui::End();
}

