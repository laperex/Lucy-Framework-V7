#include "Panel.h"
#include <Lucy/Lucy.h>
#include <Lucy/imgui_lucy_impl.h>
#include <Lucy/Light.h>
#include <Lucy/Material.h>
#include <Lucy/Registries/Registry.h>

static auto& registry = Registry::Instance();
static auto treenode_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

void lra::panel::RendererPanel() {
	ImGui::SetNextWindowBgAlpha(WindowAplha);

	if (ImGui::Begin("RendererProperties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::TreeNodeEx("Properties", treenode_flags)) {
			// ImGui::Checkbox("Enable Grid");

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Material Registry", treenode_flags)) {
			int i = 0;
			for (auto& pair: registry.store<lucy::MaterialRegistry>().storage_hashmap) {
				auto& material = pair.second.store_data;
				auto& name = pair.second.name;

				if (ImGui::TreeNodeEx(name.c_str(), treenode_flags)) {
					ImGui::DragFloat3(("Ambient##Mat" + std::to_string(i++)).c_str(), &material.ambient[0], 0.01, 0, 1);
					ImGui::DragFloat3(("Diffuse##Mat" + std::to_string(i++)).c_str(), &material.diffuse[0], 0.01, 0, 1);
					ImGui::DragFloat3(("Specular##Mat" + std::to_string(i++)).c_str(), &material.specular[0], 0.01, 0, 1);
					ImGui::DragFloat(("Shininess##Mat" + std::to_string(i++)).c_str(), &material.shininess, 0.01, 0, 256);

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Light Registry", treenode_flags)) {
			int i = 0;
			for (auto& pair: registry.store<lucy::LightRegistry>().storage_hashmap) {
				auto& light = pair.second.store_data;
				auto& name = pair.second.name;

				if (ImGui::TreeNodeEx(name.c_str(), treenode_flags)) {
					ImGui::DragFloat(("Ambient##Ligh" + std::to_string(i++)).c_str(), &light.ambient, 0.01, 0, 1);
					ImGui::DragFloat(("Diffuse##Ligh" + std::to_string(i++)).c_str(), &light.diffuse, 0.01, 0, 1);
					ImGui::DragFloat(("Specular##Ligh" + std::to_string(i++)).c_str(), &light.specular, 0.1, 0, 1);

					ImGui::ColorEdit4(("Color##Ligh" + std::to_string(i++)).c_str(), &light.color[0], ImGuiColorEditFlags_NoInputs);

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();
}
