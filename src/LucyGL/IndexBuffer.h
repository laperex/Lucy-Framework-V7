#pragma once

#include <stddef.h>

namespace lgl {
	typedef unsigned int IndexBufferId;

	struct IndexBuffer {
		IndexBufferId id;
		size_t size;
		
		IndexBuffer();
		IndexBuffer(void* data, size_t size);
		~IndexBuffer();

		void Bind();
		static void UnBind();
		void Allocate(size_t size);
		void AddData(void* data, size_t size, size_t offset = 0);
	};
}