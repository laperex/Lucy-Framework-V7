#pragma once

#include <Lucy/Math.h>//#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitter.h>

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::bvec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::bvec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::bvec4& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec2& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec3& v);
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec4& v);

namespace YAML {
	template<>
	struct convert<glm::bvec2> {
		static Node encode(const glm::bvec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::bvec2& rhs) {
			if(!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<bool>();
			rhs.y = node[1].as<bool>();

			return true;
		}
	};

	template<>
	struct convert<glm::bvec3> {
		static Node encode(const glm::bvec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::bvec3& rhs) {
			if(!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<bool>();
			rhs.y = node[1].as<bool>();
			rhs.z = node[2].as<bool>();

			return true;
		}
	};

	template<>
	struct convert<glm::bvec4> {
		static Node encode(const glm::bvec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::bvec4& rhs) {
			if(!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<bool>();
			rhs.y = node[1].as<bool>();
			rhs.z = node[2].as<bool>();
			rhs.w = node[4].as<bool>();

			return true;
		}
	};

	template<>
	struct convert<glm::ivec2> {
		static Node encode(const glm::ivec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::ivec2& rhs) {
			if(!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();

			return true;
		}
	};

	template<>
	struct convert<glm::ivec3> {
		static Node encode(const glm::ivec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::ivec3& rhs) {
			if(!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();

			return true;
		}
	};

	template<>
	struct convert<glm::ivec4> {
		static Node encode(const glm::ivec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::ivec4& rhs) {
			if(!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			rhs.w = node[4].as<int>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if(!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if(!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if(!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};
}