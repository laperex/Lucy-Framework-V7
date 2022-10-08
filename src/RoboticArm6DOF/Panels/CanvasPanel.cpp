#include "Panel.h"
#include <RoboticArm6DOF/Canvas.h>
#include <RoboticArm6DOF/Kinematics.h>
#include <RoboticArm6DOF/Animator.h>
#include <Lucy/imgui_lucy_impl.h>
#include <glm/glm.hpp>
#include <Lucy/Lucy.h>

static auto& registry = Registry::Instance();

enum DrawMode {
	NONE,
	TRIANGLE,
	QUAD,
	CIRCLE,
	LINE,

	DrawMode_COUNT
};

void lra::panel::CanvasPanel() {
	if (ImGui::Begin("Canvas")) {
		auto& canvas = registry.store<Canvas>();
		auto& animator = registry.store<Animator>();

		static std::vector<std::vector<glm::vec2>> canvas_shapes;
		static DrawMode shape;
		static bool draw_begin = false;

		static float slider = 0.17;
		ImGui::SliderFloat("##3842746", &slider, 0.1f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		float offset = ImGui::GetContentRegionAvail().x * slider;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnOffset(1, offset);

		if (ImGui::BeginChild("#6783")) {
			if (ImGui::TreeNodeEx("Properties", ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen)) {
				static glm::ivec3 position = { 150, 0, 150 }, scale = { 100, 100, 100 };
				static LUCY_UUID test_id = LUCY_NULL_UUID, last_selected_animation = LUCY_NULL_UUID;

				ImGui::DragInt3("Position", &canvas.position[0], 1);
				ImGui::DragInt3("Scale", &canvas.scale[0], 1);

				if (ImGui::Button("Test")) {
					if (test_id == LUCY_NULL_UUID) {
						AnimationProperty animation;
						animation.loop = false;
						test_id = animator.NewAnimation("__##Test", animation);
					}

					last_selected_animation = animator.selected_animation;
					animator.selected_animation = test_id;
					animator.animationstate = PLAY;
				}

				if (animator.selected_animation == test_id && animator.animationstate == PAUSE) {
					animator.animationstate = STOP;
					animator.selected_animation = last_selected_animation;
				}

				auto rest_pos = canvas.GetArmRestPos();

				bool is_valid = false;
				if (rest_pos.x >= 0) {
					auto size = canvas.GetCanvasSize();

					glm::ivec3 p0 = { rest_pos.x, canvas.position.y, rest_pos.z };
					glm::ivec3 p1 = { rest_pos.x, canvas.position.y, rest_pos.z + size.y };
					glm::ivec3 p2 = { rest_pos.x + size.x, canvas.position.y, rest_pos.z };
					glm::ivec3 p3 = { rest_pos.x + size.x, canvas.position.y, rest_pos.z + size.y };

					Kinematics::GetInverseKinematics(is_valid, p0);
					if (is_valid) Kinematics::GetInverseKinematics(is_valid, p1);
					if (is_valid) Kinematics::GetInverseKinematics(is_valid, p2);
					if (is_valid) Kinematics::GetInverseKinematics(is_valid, p3);
					if (is_valid) Kinematics::GetInverseKinematics(is_valid, rest_pos);

					if (is_valid) {
						auto& animation = animator.animation_registry[test_id].animation;
						animation.step_array.clear();
						animation.step_array.reserve(5);
						// animation.step_array.emplace_back(AnimationStep{{ rest_pos }});
						animation.step_array.emplace_back(AnimationStep{{ p0 }});
						animation.step_array.emplace_back(AnimationStep{{ p1 }});
						animation.step_array.emplace_back(AnimationStep{{ p2 }});
						animation.step_array.emplace_back(AnimationStep{{ p3 }});
						animation.step_array.emplace_back(AnimationStep{{ rest_pos }});

						animation.Generate();
					}
				}

				if (!is_valid) {
					canvas.position = position;
					canvas.scale = scale;
				} else {
					position = canvas.position;
					scale = canvas.scale;
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Functions", ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen)) {
				if (ImGui::Button("LINE")) {
					shape = LINE;
				}

				if (ImGui::Button("TRIANGLE")) {
					shape = TRIANGLE;
				}

				if (ImGui::Button("QUAD")) {
					shape = QUAD;
				}

				if (ImGui::Button("CIRCLE")) {
					shape = CIRCLE;
				}

				if (ImGui::Button("Clear")) {
					canvas_shapes.clear();
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		if (ImGui::BeginChild("#0283")) {
			if (ImGui::IsWindowHovered() && lucy::Events::IsButtonPressed(SDL_BUTTON_LEFT))
				draw_begin = true;
			else
				draw_begin = false;

			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
		}
		ImGui::EndChild();
	}
	ImGui::End();
}