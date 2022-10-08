#include "Tracing.h"
#include <iostream>
#include <LucyRE/LucyRE.h>
#include <glm/glm.hpp>

void lra::TraceAnimationPoints(int start_idx, int idx, AnimationProperty* animation, const glm::vec4& color) {
	assert(animation != nullptr);
	if (!animation->GetGenerated().size()) return;

	static void* last_element = nullptr;
	static lgl::VertexBuffer* vertexbuffer = nullptr;

	if (last_element != (void*)(&animation->GetGenerated().back())) {
		std::vector<glm::vec3> positions;

		positions.reserve(animation->generated_positions.size());
		for (int i = 0; i < animation->generated_positions.size(); i++) {
			positions.emplace_back(glm::vec3(animation->generated_positions[i].position));
		}

		if (vertexbuffer == nullptr)
			vertexbuffer = new lgl::VertexBuffer();

		vertexbuffer->Bind();
		vertexbuffer->Allocate(positions.size() * sizeof(glm::vec3));
		vertexbuffer->AddDataDynamic(positions.data(), positions.size() * sizeof(glm::vec3));

		last_element = (void*)(&animation->GetGenerated().back());
	}

	lre::SetModel(glm::mat4(1.0f));

	auto* shader = lre::GetShader("const_color");

	shader->Bind();
	shader->SetUniformVec4("u_color", &color[0]);

	lre::Render(lgl::LINE_STRIP, nullptr, lre::Vertex::P1::VertexArray(), vertexbuffer, start_idx, idx - start_idx);
}
