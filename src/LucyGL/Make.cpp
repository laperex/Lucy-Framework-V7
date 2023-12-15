#include "LucyGL/Make.h"

#include <unordered_map>
#include <memory>

namespace lgl {
	std::vector<std::unique_ptr<Texture>> texture_array;
	std::vector<std::unique_ptr<Shader>> shader_array;
	std::vector<std::unique_ptr<FrameBuffer>> framebuffer_array;
	std::vector<std::unique_ptr<UniformBuffer>> uniformbuffer_array;
	std::vector<std::unique_ptr<VertexArray>> vertexarray_array;
	std::vector<std::unique_ptr<VertexBuffer>> vertexbuffer_array;
	std::vector<std::unique_ptr<IndexBuffer>> indexbuffer_array;

	void DeleteTexture(Texture* texture) {
		for (int i = 0; i < texture_array.size(); i++) {
			if (texture == texture_array[i].get()) {
				// texture_array.erase(i);
			}
		}
	}
}