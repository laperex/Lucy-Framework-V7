#pragma once

#include <LucyGL/VertexBuffer.h>
#include <LucyGL/Status.h>

#include <glad/glad.h>
#include <assert.h>
//#include <iostream>

lgl::VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &id);

	//std::cout << "VertexBuffer Created: " << id << '\n';

	type = BufferType_DYNAMIC;
	size = 0;
}

lgl::VertexBuffer::VertexBuffer(void* data, size_t size) {
	type = BufferType_DYNAMIC;
	this->size = 0;

	glGenBuffers(1, &id);
	Bind();
	Allocate(size);
	AddDataDynamic(data, size);
	UnBind();
}

void lgl::VertexBuffer::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void lgl::VertexBuffer::UnBind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

lgl::VertexBuffer::~VertexBuffer() {
	//std::cout << "VertexBuffer Deleted: " << id << '\n';
	
	glDeleteBuffers(1, &id);
}

void lgl::VertexBuffer::Allocate(size_t size) {
	if (this->size > size && size == 0) return;

	if (this->type == BufferType_DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	if (this->type == BufferType_STATIC) glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	this->size = size;
};

void lgl::VertexBuffer::AddDataStatic(void* data, size_t size) {
	if (size == 0) return;
	if (this->type == BufferType_DYNAMIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	if (this->type == BufferType_STATIC) glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->size = size;
}

void lgl::VertexBuffer::AddDataDynamic(void* data, size_t size, size_t offset) {
	if (size == 0) return;
	assert(offset + size <= this->size);

	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	this->size = size;
}
