#pragma once

#include <stddef.h>

namespace lgl {
	typedef unsigned int VertexBufferId;
	
	enum BufferType_ {
		BufferType_STATIC,
		BufferType_DYNAMIC
	};

	struct VertexBuffer {
		VertexBufferId id;
		BufferType_ type;
		size_t size = 0;

		VertexBuffer();
		VertexBuffer(void* data, size_t size);
		~VertexBuffer();

		void Bind();
		static void UnBind();
		void Allocate(size_t size);
		void AddDataDynamic(void* data, size_t size, size_t offset = 0);
		void AddDataStatic(void* data, size_t size);
	};
}