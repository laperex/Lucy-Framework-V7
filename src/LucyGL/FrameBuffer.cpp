#pragma once

#include <LucyGL/FrameBuffer.h>

#include <glad/glad.h>
#include <assert.h>

lgl::FrameBuffer::FrameBuffer(int width, int height, bool is_picking) {
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &id);
	//std::cout << "FBO Created: " << id << '\n';

	Bind();

	texture = new Texture();
	texture->Bind();

	texture->Load2D(0, RGBA, width, height, 0, RGBA, UNSIGNED_BYTE, nullptr);
		
	texture->SetWrapMode(WrapMode_MIRRORED_REPEAT, WrapMode_MIRRORED_REPEAT);
	texture->SetFilteringMode(FilterMode_LINEAR, FilterMode_LINEAR);

	texture->UnBind();
	AttachTexture2D(COLOR_ATTACHMENT0, TEXTURE_2D, texture->id, 0);

	if (is_picking) {
		picking = new Texture();
		picking->Bind();

		picking->Load2D(0, RGBA32F, width, height, 0, RGBA, UNSIGNED_BYTE, nullptr);
		
		picking->SetWrapMode(WrapMode_MIRRORED_REPEAT, WrapMode_MIRRORED_REPEAT);
		picking->SetFilteringMode(FilterMode_LINEAR, FilterMode_LINEAR);

		picking->UnBind();
		AttachTexture2D(COLOR_ATTACHMENT1, TEXTURE_2D, picking->id, 0);
	}

	Texture depth_stencil;
	depth_stencil.Bind();
	depth_stencil.Load2D(0, DEPTH24_STENCIL8, width, height, 0, DEPTH_STENCIL, UNSIGNED_INT_24_8, nullptr);
	depth_stencil.UnBind();
	AttachTexture2D(DEPTH_STENCIL_ATTACHMENT, TEXTURE_2D, depth_stencil.id, 0);

	if (picking) {
		SetDrawAttachments({ COLOR_ATTACHMENT0, COLOR_ATTACHMENT1 });
	} else {
		SetDrawAttachments({ COLOR_ATTACHMENT0 });
	}

	assert(IsFrameBufferComplete());

	UnBind();
}

lgl::FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &id);	
}

void lgl::FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void lgl::FrameBuffer::BindRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);
}

void lgl::FrameBuffer::BindDraw() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->id);
}

void lgl::FrameBuffer::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

lgl::FrameBuffer::~FrameBuffer() {
	if (texture != nullptr) {
		delete texture;
		texture = nullptr;
	}

	if (picking != nullptr) {
		delete picking;
		picking = nullptr;
	}

	//std::cout << "FBO Deleted: " << id << '\n';
	glDeleteFramebuffers(1, &id);
}

void lgl::FrameBuffer::AttachTexture2D(Attachment attachment, TextureMode target, TextureId texture, int level) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, level);
}

void lgl::FrameBuffer::AttachTexture2D(Attachment attachment, Texture* texture, int level) {
	assert(texture != nullptr);

	AttachTexture2D(attachment, texture->mode, texture->id, level);
}

void lgl::FrameBuffer::SetDrawAttachments(const std::vector<Attachment>& attachments) {
	auto* draw_attachments = new uint32_t[attachments.size() * sizeof(uint32_t)];

	for (int i = 0; i < attachments.size(); i++)
		draw_attachments[i] = attachments[i];

	glDrawBuffers(attachments.size(), draw_attachments);

	delete[] draw_attachments;
}

bool lgl::FrameBuffer::IsFrameBufferComplete() {
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
