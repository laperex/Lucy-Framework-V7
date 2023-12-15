#pragma once

#include <stdint.h>

#include <vector>
#include "Types.h"

namespace lgl {
	struct IndexBuffer;
	struct VertexBuffer;

	typedef unsigned int VertexArrayId;

	struct VertexArrayAttribDescriptor {
		unsigned int idx;
		unsigned int size;
		Type type;

		VertexArrayAttribDescriptor(unsigned int idx, unsigned int size, Type type) {
			this->idx = idx;
			this->size = size;
			this->type = type;
		}
	};

	struct VertexArray {
		VertexArrayId id;
		size_t stride = 0;
		uint32_t elem_stride = 0;
		
		VertexArray(std::vector<VertexArrayAttribDescriptor> layouts);
		~VertexArray();

		void Bind();
		void BindVertexBuffer(VertexBuffer* vertexbuffer, size_t stride = 0, size_t offset = 0);
		void BindIndexBuffer(IndexBuffer* indexbuffer);
		static void UnBind();
	};
}