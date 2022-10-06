#include "Renderer.h"
#include "ArmInfo.h"
#include <Lucy/Window.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Transform.h"
#include <LucyUtil/UUID.h>
#include <LucyRE/LucyRE.h>
#include <LucyUtil/Importer.h>
#include <Lucy/Editor.h>
#include <Lucy/ECS.h>
#include <Lucy/Light.h>
#include <Lucy/Events.h>
#include <Lucy/Material.h>
#include <Lucy/Registries/Registry.h>
#include <Lucy/Camera.h>

static auto& registry = Registry::Instance();

// 55.0000, 0.0000, 63.5085
// 54.3250, 72.5000, 63.7585
// 54.3250, 176.6761, 63.7585
// 53.6501, 271.0141, 62.7585
// 55.0750, 363.4641, 61.7585

// 55.0750, 363.4641, 61.7585
// 40.0750, 395.4242, 62.7585
// 70.0750, 395.4242, 62.7585

#define STATIC_BASE (ROBOTIC_ARM_PARTS[0])
#define DYNAMIC_BASE (ROBOTIC_ARM_PARTS[1])
#define DYNAMIC_ARM (ROBOTIC_ARM_PARTS[2])
#define DYNAMIC_ELBOW (ROBOTIC_ARM_PARTS[3])
#define DYNAMIC_WRIST (ROBOTIC_ARM_PARTS[4])
#define STATIC_GRIPPER (ROBOTIC_ARM_PARTS[5])
#define DYNAMIC_GRIPPER_RIGHT (ROBOTIC_ARM_PARTS[6])
#define DYNAMIC_GRIPPER_LEFT (ROBOTIC_ARM_PARTS[7])
#define MESH_PARTS_COUNT 8

static glm::vec4 selected_pixel;
static std::vector<glm::vec3> grid, grid_small;

static struct {
	glm::vec3 position = { 0, 0, 0 };
	std::string name;
	LUCY_UUID mesh_id = LUCY_NULL_UUID;
	Transform transform;
	glm::mat4 matrix = glm::mat4(1.0f);
} ROBOTIC_ARM_PARTS[] = {
	{{ 55.0000, 0.0000, 63.5085 }, "STATIC_BASE" },
	{{ 55.0000, 55.5000, 63.5085 }, "DYNAMIC_BASE" },
	{{ 54.3250, 72.5000, 63.7585 }, "DYNAMIC_ARM" },
	{{ 54.3250, 176.6761, 63.7585 }, "DYNAMIC_ELBOW" },
	{{ 53.6501, 271.0141, 62.7585 }, "DYNAMIC_WRIST" },
	{{ 55.0750, 363.4641, 61.7585 }, "STATIC_GRIPPER" },
	{{ 40.0750, 395.4242, 62.7585 }, "DYNAMIC_GRIPPER_RIGHT" },
	{{ 70.0750, 395.4242, 62.7585 }, "DYNAMIC_GRIPPER_LEFT" },
};

void lra::IntializeRenderer() {
	for (int i = 0; i < MESH_PARTS_COUNT; i++) {
		if (ROBOTIC_ARM_PARTS[i].mesh_id == LUCY_NULL_UUID) {
			ROBOTIC_ARM_PARTS[i].mesh_id = lre::InsertMesh("i", util::LoadMeshToGPU("D:\\C++\\Lucy Framework V7\\assets\\" + ROBOTIC_ARM_PARTS[i].name + ".stl", "", -ROBOTIC_ARM_PARTS[i].position));
			ROBOTIC_ARM_PARTS[i].transform.translation = ROBOTIC_ARM_PARTS[i].position - STATIC_BASE.position;
		}
	}

	for (int i = -500; i <= 500; i++) {
		if (i % 100 == 0) {
			grid.push_back({ -500, 0, i });
			grid.push_back({ +500, 0, i });

			grid.push_back({ i, 0, -500 });
			grid.push_back({ i, 0, +500 });
		}
		if (i % 10 == 0) {
			grid_small.push_back({ -500, 0, i });
			grid_small.push_back({ +500, 0, i });

			grid_small.push_back({ i, 0, -500 });
			grid_small.push_back({ i, 0, +500 });
		}
	}
}

void lra::RenderLRA(JointAngles joint_angles) {
	auto& info = registry.store<ArmInfo>();
	auto& light = registry.store<lucy::LightRegistry>().Get("Default");
	auto& camera = registry.store<lucy::Camera>();
	auto& material = registry.store<lucy::MaterialRegistry>().Get("LRA");

	joint_angles.base = 90 + joint_angles.base;
	joint_angles.arm = 180 - joint_angles.arm;
	joint_angles.elbow = 180 - joint_angles.elbow;
	joint_angles.wrist = 180 - joint_angles.wrist;

	DYNAMIC_BASE.transform.rotation = { 0, joint_angles.base - 90, 0 };
	DYNAMIC_ARM.transform.rotation = { joint_angles.arm - 90, 0, 0 };
	DYNAMIC_ELBOW.transform.rotation = { joint_angles.elbow + joint_angles.arm + 90 + 180, 0, 0 };
	DYNAMIC_WRIST.transform.rotation = { joint_angles.wrist + joint_angles.elbow + joint_angles.arm - 90, 0, 0 };

	STATIC_GRIPPER.transform.rotation = { 0, joint_angles.gripper_rotate, 0 };
	DYNAMIC_GRIPPER_LEFT.transform.rotation = { 0, 0, -joint_angles.gripper_control };
	DYNAMIC_GRIPPER_RIGHT.transform.rotation = { 0, 0, joint_angles.gripper_control };

	{
		DYNAMIC_ELBOW.transform.translation = DYNAMIC_ARM.transform.translation + ((DYNAMIC_ELBOW.position - DYNAMIC_ARM.position) * glm::quat(glm::radians(glm::vec3(-DYNAMIC_ARM.transform.rotation.x, 0, 0))));
		DYNAMIC_WRIST.transform.translation = DYNAMIC_ELBOW.transform.translation + ((DYNAMIC_WRIST.position - DYNAMIC_ELBOW.position) * glm::quat(glm::radians(glm::vec3(-DYNAMIC_ELBOW.transform.rotation.x, 0, 0))));

		STATIC_GRIPPER.transform.translation = DYNAMIC_WRIST.transform.translation + ((STATIC_GRIPPER.position - DYNAMIC_WRIST.position) * glm::quat(glm::radians(glm::vec3(-DYNAMIC_WRIST.transform.rotation.x, 0, 0))));
		DYNAMIC_GRIPPER_RIGHT.transform.translation = DYNAMIC_WRIST.transform.translation + ((DYNAMIC_GRIPPER_RIGHT.position - DYNAMIC_WRIST.position) * glm::quat(glm::radians(glm::vec3(-DYNAMIC_WRIST.transform.rotation.x, -STATIC_GRIPPER.transform.rotation.y, 0))));
		DYNAMIC_GRIPPER_LEFT.transform.translation = DYNAMIC_WRIST.transform.translation + ((DYNAMIC_GRIPPER_LEFT.position - DYNAMIC_WRIST.position) * glm::quat(glm::radians(glm::vec3(-DYNAMIC_WRIST.transform.rotation.x, -STATIC_GRIPPER.transform.rotation.y, 0))));

		STATIC_BASE.matrix = STATIC_BASE.transform.GetModelMatrix();
		DYNAMIC_BASE.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_BASE.transform.GetTranslationMatrix();
		DYNAMIC_ARM.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_ARM.transform.GetTranslationMatrix() * DYNAMIC_ARM.transform.GetRotationMatrix();
		DYNAMIC_ELBOW.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_ELBOW.transform.GetTranslationMatrix() * DYNAMIC_ELBOW.transform.GetRotationMatrix();
		DYNAMIC_WRIST.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_WRIST.transform.GetTranslationMatrix() * DYNAMIC_WRIST.transform.GetRotationMatrix();

		STATIC_GRIPPER.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * STATIC_GRIPPER.transform.GetTranslationMatrix() * DYNAMIC_WRIST.transform.GetRotationMatrix() * STATIC_GRIPPER.transform.GetRotationMatrix();
		DYNAMIC_GRIPPER_RIGHT.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_GRIPPER_RIGHT.transform.GetTranslationMatrix() * DYNAMIC_WRIST.transform.GetRotationMatrix() * STATIC_GRIPPER.transform.GetRotationMatrix() * DYNAMIC_GRIPPER_RIGHT.transform.GetRotationMatrix();
		DYNAMIC_GRIPPER_LEFT.matrix = DYNAMIC_BASE.transform.GetRotationMatrix() * DYNAMIC_GRIPPER_LEFT.transform.GetTranslationMatrix() * DYNAMIC_WRIST.transform.GetRotationMatrix() * STATIC_GRIPPER.transform.GetRotationMatrix() * DYNAMIC_GRIPPER_LEFT.transform.GetRotationMatrix();
	}

	info.J0 = DYNAMIC_BASE.transform.translation;
	info.J1 = DYNAMIC_ARM.transform.translation;
	info.J2 = DYNAMIC_ELBOW.transform.translation;
	info.J3 = DYNAMIC_WRIST.transform.translation;
	info.J4 = STATIC_GRIPPER.transform.translation;
	info.J5 = (DYNAMIC_GRIPPER_LEFT.transform.translation + DYNAMIC_GRIPPER_RIGHT.transform.translation) / 2.0f;

	auto* shader = lre::GetShader("phong");
	shader->Bind();

	light.Bind(shader);
	material.Bind(shader);

	for (int i = 0; i < MESH_PARTS_COUNT; i++) {
		if (ROBOTIC_ARM_PARTS[i].mesh_id == LUCY_NULL_UUID) continue;

		lre::SetModel(ROBOTIC_ARM_PARTS[i].matrix);
		lre::RenderMesh(ROBOTIC_ARM_PARTS[i].mesh_id, shader, i + 1);
	}

	// auto* framebuffer = Editor::GetMainFrameBuffer();
	// if (framebuffer != nullptr) {
	// 	framebuffer->Bind();
		
	// 	framebuffer->UnBind();
	// }
}

void lra::RenderCube(int val, lgl::Shader* shader) {
	static LUCY_UUID id = 0;

	static std::vector<lre::Vertex::P1N1> vertices = {
		{{ -0.5, -0.5, -0.5 }, { 0.0, 0.0, -1.0 }},
		{{ -0.5, +0.5, -0.5 }, { 0.0, 0.0, -1.0 }},
		{{ +0.5, +0.5, -0.5 }, { 0.0, 0.0, -1.0 }},
		{{ +0.5, -0.5, -0.5 }, { 0.0, 0.0, -1.0 }},

		{{ -0.5, -0.5, +0.5 }, { 0.0, 0.0, +1.0 }},
		{{ -0.5, +0.5, +0.5 }, { 0.0, 0.0, +1.0 }},
		{{ +0.5, +0.5, +0.5 }, { 0.0, 0.0, +1.0 }},
		{{ +0.5, -0.5, +0.5 }, { 0.0, 0.0, +1.0 }},

		{{ -0.5, -0.5, +0.5 }, { -1.0, 0.0, 0.0 }},
		{{ -0.5, +0.5, +0.5 }, { -1.0, 0.0, 0.0 }},
		{{ -0.5, +0.5, -0.5 }, { -1.0, 0.0, 0.0 }},
		{{ -0.5, -0.5, -0.5 }, { -1.0, 0.0, 0.0 }},

		{{ +0.5, -0.5, +0.5 }, { +1.0, 0.0, 0.0 }},
		{{ +0.5, +0.5, +0.5 }, { +1.0, 0.0, 0.0 }},
		{{ +0.5, +0.5, -0.5 }, { +1.0, 0.0, 0.0 }},
		{{ +0.5, -0.5, -0.5 }, { +1.0, 0.0, 0.0 }},
		
		{{ -0.5, -0.5, -0.5 }, { 0.0, -1.0, 0.0 }},
		{{ -0.5, -0.5, +0.5 }, { 0.0, -1.0, 0.0 }},
		{{ +0.5, -0.5, +0.5 }, { 0.0, -1.0, 0.0 }},
		{{ +0.5, -0.5, -0.5 }, { 0.0, -1.0, 0.0 }},

		{{ -0.5, +0.5, -0.5 }, { 0.0, +1.0, 0.0 }},
		{{ -0.5, +0.5, +0.5 }, { 0.0, +1.0, 0.0 }},
		{{ +0.5, +0.5, +0.5 }, { 0.0, +1.0, 0.0 }},
		{{ +0.5, +0.5, -0.5 }, { 0.0, +1.0, 0.0 }},
	};

	static std::vector<uint32_t> indices = {
		0 + 4 * 0, 1 + 4 * 0, 2 + 4 * 0, 2 + 4 * 0, 3 + 4 * 0, 0 + 4 * 0,
		0 + 4 * 1, 1 + 4 * 1, 2 + 4 * 1, 2 + 4 * 1, 3 + 4 * 1, 0 + 4 * 1,
		0 + 4 * 2, 1 + 4 * 2, 2 + 4 * 2, 2 + 4 * 2, 3 + 4 * 2, 0 + 4 * 2,
		0 + 4 * 3, 1 + 4 * 3, 2 + 4 * 3, 2 + 4 * 3, 3 + 4 * 3, 0 + 4 * 3,
		0 + 4 * 4, 1 + 4 * 4, 2 + 4 * 4, 2 + 4 * 4, 3 + 4 * 4, 0 + 4 * 4,
		0 + 4 * 5, 1 + 4 * 5, 2 + 4 * 5, 2 + 4 * 5, 3 + 4 * 5, 0 + 4 * 5,
	};

	if (id == LUCY_NULL_UUID) {
		id = lre::InsertMesh("Cube", vertices.data(), vertices.size(), indices.data(), indices.size());
	}

	// lre::SetModel(glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), scale));
	lre::RenderMesh(id, shader, val);
}

void lra::RenderAxisLine(bool x, bool y, bool z) {
	lre::SetModel(glm::mat4(1.0f));

	if (x) lre::RenderLine({ 0, 0, 0 }, { 1000, 0, 0 }, { 1, 0, 0, 1 });
	if (y) lre::RenderLine({ 0, 0, 0 }, { 0, 1000, 0 }, { 0, 1, 0, 1 });
	if (z) lre::RenderLine({ 0, 0, 0 }, { 0, 0, 1000 }, { 0, 0, 1, 1 });

	lre::RenderFlushLine();
}

void lra::RenderGrid() {
	lre::SetModel(glm::mat4(1.0f));

	lre::RenderLine(grid, { 1, 1, 1, 0.5 });
	lre::RenderLine(grid_small, { 0.5, 0.5, 0.5, 0.5 });
}

// void lra::DrawGrid(uint32_t size, const std::vector<uint32_t>& unit) {
// 	static uint32_t t_size = 1000;
// 	static std::vector<uint32_t> t_unit = { 10, 100 };

// 	if (size != t_size || t_unit != unit) {
// 		for (int i = -500; i <= 500; i++) {
// 			for (int i = 0; i < unit.size(); i++) {
// 				if (i % 100 == 0) {
// 					grid.push_back({ -500, 0, i });
// 					grid.push_back({ +500, 0, i });

// 					grid.push_back({ i, 0, -500 });
// 					grid.push_back({ i, 0, +500 });
// 				}
// 			}
// 		}
// 	}
// }

uint32_t lra::SelectID() {
	std::cout << selected_pixel.y << '\n';
	return uint32_t(selected_pixel.y);
}
