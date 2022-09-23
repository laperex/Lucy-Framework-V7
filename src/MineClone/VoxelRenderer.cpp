#include "VoxelRenderer.h"
#include "VoxelVertex.h"
#include <LucyRE/LucyRE.h>

void lpv::DrawVoxel(uint8_t posx, uint8_t posy, uint8_t posz, VoxelFace face, int tex) {
	
}

void lpv::VoxelVertexTest() {
	lre::SetModel(glm::mat4(1.0f));

	static Vertex test_vertices[] = {
		{ 0, 0, 0, 2, 0 },
		{ 0, 1, 0, 2, 0 },
		{ 1, 1, 0, 2, 0 },
		{ 1, 1, 0, 2, 0 },
		{ 1, 0, 0, 2, 0 },
		{ 0, 0, 0, 2, 0 },

		{ 0, 0, 1, 2, 0 },
		{ 0, 1, 1, 2, 0 },
		{ 1, 1, 1, 2, 0 },
		{ 1, 1, 1, 2, 0 },
		{ 1, 0, 1, 2, 0 },
		{ 0, 0, 1, 2, 0 },
	};

	static lgl::VertexArray* vertexarray = Vertex::VertexArray();
	static lgl::VertexBuffer* vertexbuffer = nullptr;
	static lgl::Texture* texture = nullptr;
	static auto* shader = lre::GetShader("voxel-uv");

	if (vertexbuffer == nullptr) {
		vertexbuffer = new lgl::VertexBuffer();

		vertexbuffer->Bind();
		vertexbuffer->Allocate(sizeof(test_vertices));
		vertexbuffer->AddDataDynamic(test_vertices, sizeof(test_vertices));
	}

	if (texture == nullptr) {
		texture = new lgl::Texture(lgl::TEXTURE_2D);

		texture->Bind();
		texture->LoadTexture(nullptr);
		texture->SetFilteringMode(lgl::FilterMode_NEAREST, lgl::FilterMode_NEAREST);
		texture->SetWrapMode(lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT);
		texture->UnBind();
	}

	if (shader == nullptr) {
		shader = new lgl::Shader();
		shader->VertexShader("D:\\C++\\Lucy Framework V7\\src\\LucyRE\\Shaders\\voxel.vert");
		shader->FragmentShader("D:\\C++\\Lucy Framework V7\\src\\LucyRE\\Shaders\\uv.fs");
		shader->Link();
		lre::InsertShader("voxel-uv", shader);
	}

	texture->BindUnit(0);

	shader->Bind();
	shader->SetUniformi("u_texture", 0);

	vertexarray->Bind();
	vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);

	lgl::Draw(lgl::TRIANGLE, 0, 6 * 2);
}