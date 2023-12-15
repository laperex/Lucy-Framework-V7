#pragma once

#include <LucyGL/VertexArray.h>
#include <LucyGL/VertexBuffer.h>
#include <LucyGL/IndexBuffer.h>

#include <glad/glad.h>
#include <assert.h>
//#include <iostream>

lgl::VertexArray::VertexArray(std::vector<VertexArrayAttribDescriptor> layouts) {
	glGenVertexArrays(1, &id);
	
	//std::cout << "VertexArray Created: " << id << '\n';

	Bind();

	uint32_t relativeoffset = 0;
	uint32_t elem_relativeoffset = 0;

	for (auto& attrib: layouts) {
		glVertexArrayAttribBinding(id, attrib.idx, 0);
		glEnableVertexArrayAttrib(id, attrib.idx);

		switch (attrib.type) {
			case FLOAT:
				glVertexArrayAttribFormat(id, attrib.idx, attrib.size, attrib.type, false, relativeoffset);
				break;
			case BYTE:
			case UNSIGNED_BYTE:
			case UNSIGNED_INT:
			case INT:
				glVertexArrayAttribIFormat(id, attrib.idx, attrib.size, attrib.type, relativeoffset);
				break;
			default:
				assert(false);
		}

		switch(attrib.type) {
			case FLOAT:
				relativeoffset += sizeof(GLfloat)*attrib.size;
				break;
			
			case FIXED:
				relativeoffset += sizeof(GLfixed)*attrib.size;
				break;
			
			case BYTE:
				relativeoffset += sizeof(GLbyte)*attrib.size;
				break;
			
			case UNSIGNED_BYTE:
				relativeoffset += sizeof(GLubyte)*attrib.size;
				break;

			case UNSIGNED_INT:
				relativeoffset += sizeof(GLuint)*attrib.size;
				break;

			case INT:
				relativeoffset += sizeof(GLint)*attrib.size;
				break;
			
			default:
				assert(false);
		}
		
		elem_relativeoffset += attrib.size;
	}

	this->stride = relativeoffset;
	this->elem_stride = elem_relativeoffset;
}

lgl::VertexArray::~VertexArray() {
	//std::cout << "VertexArray Deleted: " << id << '\n';

	glDeleteVertexArrays(1, &id);
}

void lgl::VertexArray::Bind() {
	glBindVertexArray(id);
}

void lgl::VertexArray::UnBind() {
	glBindVertexArray(0);
}

void lgl::VertexArray::BindVertexBuffer(VertexBuffer *vertexbuffer, size_t stride, size_t offset) {
	glVertexArrayVertexBuffer(id, 0, vertexbuffer->id, offset, (stride == 0) ? this->stride : stride);
}

void lgl::VertexArray::BindIndexBuffer(IndexBuffer *indexbuffer) {
	glVertexArrayElementBuffer(id, indexbuffer->id);
}