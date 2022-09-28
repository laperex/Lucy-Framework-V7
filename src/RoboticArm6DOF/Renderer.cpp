#include "Renderer.h"
#include <glm/glm.hpp>
#include <iostream>
#include "Transform.h"
#include <LucyUtil/UUID.h>
#include <LucyRE/LucyRE.h>
#include <LucyUtil/Importer.h>
#include <Lucy/ECS.h>
#include "Light.h"
#include "Material.h"

static auto& registry = Registry::Instance();

auto& light = registry.store<Light>();
auto& material = registry.store<Material>();

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

static struct {
	glm::vec3 position = { 0, 0, 0 };
	std::string name;
	UTIL_UUID mesh_id = UTIL_NULL_UUID;
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
		if (ROBOTIC_ARM_PARTS[i].mesh_id == UTIL_NULL_UUID) {
			ROBOTIC_ARM_PARTS[i].mesh_id = lre::InsertMesh("i", util::LoadMeshToGPU("D:\\C++\\Lucy Framework V7\\assets\\" + ROBOTIC_ARM_PARTS[i].name + ".stl", "", -ROBOTIC_ARM_PARTS[i].position));
			ROBOTIC_ARM_PARTS[i].transform.translation = ROBOTIC_ARM_PARTS[i].position - STATIC_BASE.position;
		}
	}
}

void lra::RenderLRA(JointAngles joint_angles) {
	DYNAMIC_BASE.transform.rotation = { 0, joint_angles.base - 90, 0 };
	DYNAMIC_ARM.transform.rotation = { joint_angles.arm - 90, 0, 0 };
	DYNAMIC_ELBOW.transform.rotation = { joint_angles.elbow + joint_angles.arm + 90, 0, 0 };
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

	auto* shader = lre::GetShader("phong");

	shader->Bind();

	light.Bind(shader);
	material.Bind(shader);

	for (int i = 0; i < MESH_PARTS_COUNT; i++) {
		if (ROBOTIC_ARM_PARTS[i].mesh_id == UTIL_NULL_UUID) continue;

		lre::SetModel(ROBOTIC_ARM_PARTS[i].matrix);

		lre::RenderMesh(ROBOTIC_ARM_PARTS[i].mesh_id, shader, i);
	}
}
