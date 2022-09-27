#include "VoxelRenderer.h"
#include "VoxelVertex.h"
#include "Block.h"
#include <LucyRE/LucyRE.h>
#include <iostream>

void lpv::DrawVoxel(uint8_t posx, uint8_t posy, uint8_t posz, VoxelFace face, int tex) {
	
}

void lpv::VoxelVertexTest() {
	lre::SetModel(glm::mat4(1.0f));

	static std::vector<Vertex> vertices;
	static lgl::VertexArray* vertexarray = Vertex::VertexArray();
	static lgl::VertexBuffer* vertexbuffer = nullptr;
	static lgl::Texture* texture = nullptr;
	static lgl::Shader* shader = lre::GetShader("voxel-uv");

	if (vertexbuffer == nullptr) {
		vertexbuffer = new lgl::VertexBuffer();

		FaceVertices(vertices, FRONT, 0, 0, 0, 1, 1, 1, 3);
		FaceVertices(vertices, BACK, 0, 0, 0, 1, 1, 1, 0);
		FaceVertices(vertices, LEFT, 0, 0, 0, 1, 1, 1, 0);
		FaceVertices(vertices, RIGHT, 0, 0, 0, 1, 1, 1, 0);
		FaceVertices(vertices, BOTTOM, 0, 0, 0, 1, 1, 1, 1);
		FaceVertices(vertices, TOP, 0, 0, 0, 1, 1, 1, 6);

		vertexbuffer->Bind();
		vertexbuffer->Allocate(sizeof(Vertex) * vertices.size());
		vertexbuffer->AddDataDynamic(vertices.data(), sizeof(Vertex) * vertices.size());
	}

	if (texture == nullptr) {
		texture = new lgl::Texture(lgl::TEXTURE_2D);

		texture->Bind();
		texture->LoadSpriteSheet("D:\\C++\\Lucy Framework V7\\assets\\Spritesheet.png", 3, 3, 80, 80);
		texture->SetFilteringMode(lgl::FilterMode_NEAREST, lgl::FilterMode_NEAREST);
		texture->SetWrapMode(lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT, lgl::WrapMode_REPEAT);
		texture->UnBind();
	}

	if (shader == nullptr) {
		shader = new lgl::Shader();
		shader->VertexShader("D:\\C++\\Lucy Framework V7\\src\\LucyRE\\Shaders\\voxel.vert");
		shader->FragmentShader("D:\\C++\\Lucy Framework V7\\src\\LucyRE\\Shaders\\uvw.fs");
		shader->Link();
		lre::InsertShader("voxel-uv", shader);
	}

	texture->BindUnit(0);

	shader->Bind();
	shader->SetUniformi("u_texture", 0);

	vertexarray->Bind();
	vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);

	lgl::Draw(lgl::TRIANGLE, 0, 6 * 6);
}