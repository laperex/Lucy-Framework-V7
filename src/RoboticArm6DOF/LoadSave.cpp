#include "LoadSave.h"
#include <LucyUtil/LucyUtil.h>
#include <LucyUtil/LucyUtil.h>
#include "RoboticArm.h"
#include "Animator.h"
#include <Lucy/Lucy.h>

static auto& registry = Registry::Instance();

void lra::SerializeLRA(const char* filename) {
	
}

void lra::SerializeAnimator(const char* filename) {
	auto& animator = registry.store<Animator>();

	YAML::Emitter out;

	out << YAML::BeginSeq;
	out << animator.selected_animation;

	for (auto& pair: animator.animation_registry) {
		const auto& id = pair.first;
		const auto& name = pair.second.name;
		auto& animation = pair.second.animation;

		out << YAML::BeginMap;
		{
			out << YAML::Key << "id";
			out << YAML::Value << id;
			
			out << YAML::Key << "name";
			out << YAML::Value << name;
			
			out << YAML::Key << "loop";
			out << YAML::Value << animation.loop;

			out << YAML::Key << "steps";
			out << YAML::Value;
			out << YAML::BeginSeq;
			{
				for (auto& step: animation.step_array) {
					out << YAML::Flow;
					out << YAML::BeginSeq;
					{
						out << step.target_position;

						out << YAML::Flow;
						out << YAML::BeginSeq;
						for (int i = 0; i < 6; i++) {
							out << step.target_angles[i];
						}
						out << YAML::EndSeq;

						out << step.progress_len;
					}
					out << YAML::EndSeq;
				}
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	util::write_string_to_file(filename, out.c_str());
}

void lra::DeSerializeLRA(const char* filename) {
	
}

void lra::DeSerializeAnimator(const char* filename) {
	if (util::read_string_from_file(filename).second == 0) return;

	YAML::Node scene = YAML::LoadFile(filename);

	auto& animator = registry.store<Animator>();

	animator.selected_animation = scene[0].as<LUCY_UUID>();

	for (int i = 1; i < scene.size(); i++) {
		AnimationProperty animation;

		auto name = scene[i]["name"].as<std::string>();
		auto id = scene[i]["id"].as<LUCY_UUID>();

		animation.loop = scene[i]["loop"].as<bool>();

		for (int j = 0; j < scene[i]["steps"].size(); j++) {
			AnimationStep step;

			step.target_position = scene[i]["steps"][j][0].as<glm::ivec3>();
			for (int v = 0; v < 6; v++) {
				step.target_angles[v] = scene[i]["steps"][j][1][v].as<float>();
			}
			step.progress_len = scene[i]["steps"][j][2].as<float>();

			animation.step_array.push_back(step);
		}

		animator.NewAnimation(name, animation, id);
	}
}
