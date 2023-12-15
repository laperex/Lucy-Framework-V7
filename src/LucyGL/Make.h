#pragma once

#include "LucyGL.h"
#include <vector>
#include <memory>

namespace lgl {
	template <typename ...Args>
	Texture* MakeTexture(Args ...params) {
		extern std::vector<std::unique_ptr<Texture>> texture_array;

		texture_array.push_back(std::make_unique<Texture>(params...));

		return texture_array.back().get();
	}

	template <typename ...Args>
	Shader* MakeShader(Args ...params) {
		extern std::vector<std::unique_ptr<Shader>> shader_array;

		shader_array.push_back(std::make_unique<Shader>(params...));

		return shader_array.back().get();
	}

	template <typename ...Args>
	FrameBuffer* MakeFrameBuffer(Args ...params) {
		extern std::vector<std::unique_ptr<FrameBuffer>> framebuffer_array;

		framebuffer_array.push_back(std::make_unique<FrameBuffer>(params...));

		return framebuffer_array.back().get();
	}

	template <typename ...Args>
	UniformBuffer* MakeUniformBuffer(Args ...params) {
		extern std::vector<std::unique_ptr<UniformBuffer>> uniformbuffer_array;

		uniformbuffer_array.push_back(std::make_unique<UniformBuffer>(params...));

		return uniformbuffer_array.back().get();
	}

	template <typename ...Args>
	VertexArray* MakeVertexArray(Args ...params) {
		extern std::vector<std::unique_ptr<VertexArray>> vertexarray_array;

		vertexarray_array.push_back(std::make_unique<VertexArray>(params...));

		return vertexarray_array.back().get();
	}

	template <typename ...Args>
	VertexBuffer* MakeVertexBuffer(Args ...params) {
		extern std::vector<std::unique_ptr<VertexBuffer>> vertexbuffer_array;

		vertexbuffer_array.push_back(std::make_unique<VertexBuffer>(params...));

		return vertexbuffer_array.back().get();
	}

	template <typename ...Args>
	IndexBuffer* MakeIndexBuffer(Args ...params) {
		extern std::vector<std::unique_ptr<IndexBuffer>> indexbuffer_array;

		indexbuffer_array.push_back(std::make_unique<IndexBuffer>(params...));

		return indexbuffer_array.back().get();
	}

	void DeleteTexture(Texture* texture);

	void DeleteShader(Shader* shader);

	void DeleteFrameBuffer(FrameBuffer* framebuffer);

	void DeleteUniformBuffer(UniformBuffer* uniformbuffer);

	void DeleteVertexArray(VertexArray* vertexarray);

	void DeleteVertexBuffer(VertexBuffer* vertexbuffer);

	void DeleteIndexBuffer(IndexBuffer* indexbuffer);
}