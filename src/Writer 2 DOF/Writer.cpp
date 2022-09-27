#include "Writer.h"
#include "RoboticArm.h"
#include "Kinematics.h"
#include <LucyUtil/SerialIO.h>
#include <Lucy/imgui_lucy_impl.h>

char serial_data[255];
char port[] = "\\\\.\\COM6";
uint8_t data_package[27] = { '&', 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, '$', '\n' };

void arm::Initialize() {
	
}

void arm::Update() {
	
}

static const float lower = 10.5;
static const float upper = 9.8;

#define TYPE_CHANGE(t0, t1) (*(t0*)(&(t1)))
#define TO_IMVec2(type) TYPE_CHANGE(ImVec2, type)
#define TO_IMVec3(type) TYPE_CHANGE(ImVec3, type)
#define TO_IMVec4(type) TYPE_CHANGE(ImVec4, type)

void DrawArm(glm::vec2 target, float lower, float lower_angle, float upper, float upper_angle) {
	if (ImGui::Begin("Test")) {
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();

		if (canvas_sz.x < 50.0f)
			canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f)
			canvas_sz.y = 50.0f;

		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
		draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

		// ImVec2 p0 = { 0, 0 };
		float height = lower + upper;
		float width = height * 2;

		glm::vec2 min_p0 = { 0, 0 };
		glm::vec2 max_p1 = { height, height };

		// draw_list->AddLine(TO_IMVec2(min_p0 * glm::vec2(0)), { 0, 0 }, ImGui::GetColorU32({ 0, 1, 1, 1 }));
	}
	ImGui::End();
}

void arm::EditorUpdate() {
	static glm::vec2 target = { 10, 10 };
	static Position position;
	position = arm::GetInverseKinematics(target, lower, upper);

	if (ImGui::Begin("Info")) {
		ImGui::DragFloat("Lower Joint", &position[0], 0.1, 0, 180);
		ImGui::DragFloat("Upper Joint", &position[1], 0.1, 0, 180);

		ImGui::NewLine();

		glm::vec2 temp = target;
		ImGui::DragFloat2("Target", &temp[0], 0.1, 100);
		if (glm::length(temp) < lower + upper) {
			target = temp;
		}
	}
	ImGui::End();

	if (!util::serial::is_port_connected()) {
		util::serial::connect_port(port, util::serial::BAUDRATE_38400);
	} else {
		*(float*)(&data_package[5]) = position[0];
		*(float*)(&data_package[9]) = position[1];

		util::serial::write_bytes_to_port(data_package, 27);
	}

	DrawArm(target, lower, position[0], upper, position[1]);
}

