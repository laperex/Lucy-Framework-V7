#pragma once

#include <stddef.h>

namespace lgl {
	typedef unsigned int UniformBufferId;

	struct UniformBuffer {
		UniformBufferId id;
		size_t size = 0;
		
		UniformBuffer();
		~UniformBuffer();

		void Allocate(size_t size);
		void AddDataDynamic(void* data, size_t size, size_t offset = 0);
		void BindRange(unsigned int index, size_t size, size_t offset = 0);
		void Bind();
		static void UnBind();
	};
}