#include "Canvas.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

void lra::Canvas::Render(const glm::vec4& color) {
	static lgl::VertexBuffer* vertexbuffer = nullptr;
	static glm::vec3 positions[] = {
		{ -0.5, 0, -0.5 },
		{ -0.5, 0, +0.5 },
		{ +0.5, 0, +0.5 },
		{ +0.5, 0, -0.5 },
		{ -0.5, 0, -0.5 },
	};

	if (vertexbuffer == nullptr) {
		vertexbuffer = new lgl::VertexBuffer();

		vertexbuffer->Bind();
		vertexbuffer->Allocate(sizeof(positions));
		vertexbuffer->AddDataDynamic(positions, sizeof(positions));
	}

	lre::SetModel(glm::translate(glm::mat4(1.0f), glm::vec3(position)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale)));

	auto* shader = lre::GetShader("const_color");

	shader->Bind();
	shader->SetUniformi("data", 100);
	shader->SetUniformVec4("u_color", &color[0]);

	glLineWidth(3);

	lre::Render(lgl::LINE_STRIP, nullptr, lre::Vertex::P1::VertexArray(), vertexbuffer, 5);

	glLineWidth(1);
}

const glm::ivec2& lra::Canvas::GetOriginPos() {
	auto pos = position - (scale / 2);
	return { pos.x, pos.z };
}

const glm::ivec2& lra::Canvas::GetCanvasSize() {
	return { scale.x, scale.z };
}

const glm::ivec3& lra::Canvas::GetArmRestPos() {
	auto pos = GetOriginPos();
	return { pos.x, 50, pos.y };
}
