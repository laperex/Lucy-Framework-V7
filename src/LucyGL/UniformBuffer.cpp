#include <LucyGL/UniformBuffer.h>

#include <glad/glad.h>
#include <assert.h>
//#include <iostream>

lgl::UniformBuffer::UniformBuffer() {
	size = 0;
	glGenBuffers(1, &id);

	//std::cout << "UniformBuffer Created: " << id << '\n';
}

lgl::UniformBuffer::~UniformBuffer() {
	//std::cout << "UniformBuffer Deleted: " << id << '\n';

	glDeleteBuffers(1, &id);
}

void lgl::UniformBuffer::Allocate(size_t size) {
	if (this->size > size) return;

	this->size = size;
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
};

void lgl::UniformBuffer::AddDataDynamic(void* data, size_t size, size_t offset) {
	assert(offset + size <= this->size);

	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void lgl::UniformBuffer::BindRange(unsigned int index, size_t size, size_t offset) {
	glBindBufferRange(GL_UNIFORM_BUFFER, index, id, offset, size);
}

void lgl::UniformBuffer::Bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void lgl::UniformBuffer::UnBind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}