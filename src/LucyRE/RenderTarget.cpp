#include "RenderTarget.h"

lre::RenderTarget::~RenderTarget() {
	delete postprocess_fbo;
	postprocess_fbo = nullptr;
}

void lre::RenderTarget::Initialize(int width, int height, RenderTargetFlags_ flags) {
	postprocess_fbo = new lgl::FrameBuffer;

	postprocess_fbo->width = width;
	postprocess_fbo->height = height;

	postprocess_fbo->Bind();

	std::vector<lgl::Attachment> draw_attachments;

	{
		auto* draw_texture = new lgl::Texture();
		draw_texture->Bind();

		draw_texture->Load2D(0, lgl::RGBA, width, height, 0, lgl::RGBA, lgl::UNSIGNED_BYTE, nullptr);

		draw_texture->SetWrapMode(lgl::WrapMode_MIRRORED_REPEAT, lgl::WrapMode_MIRRORED_REPEAT);
		draw_texture->SetFilteringMode(lgl::FilterMode_LINEAR, lgl::FilterMode_LINEAR);

		draw_texture->UnBind();

		postprocess_fbo->AttachTexture2D(lgl::COLOR_ATTACHMENT0, draw_texture->mode, draw_texture->id, 0);

		draw_attachments.push_back(lgl::COLOR_ATTACHMENT0);
	}
	{
		auto* picking_texture = new lgl::Texture();
		picking_texture->Bind();

		picking_texture->Load2D(0, lgl::RGBA32F, width, height, 0, lgl::RGBA, lgl::UNSIGNED_BYTE, nullptr);
		
		picking_texture->SetWrapMode(lgl::WrapMode_MIRRORED_REPEAT, lgl::WrapMode_MIRRORED_REPEAT);
		picking_texture->SetFilteringMode(lgl::FilterMode_LINEAR, lgl::FilterMode_LINEAR);

		picking_texture->UnBind();
		postprocess_fbo->AttachTexture2D(lgl::COLOR_ATTACHMENT1, picking_texture->mode, picking_texture->id, 0);

		draw_attachments.push_back(lgl::COLOR_ATTACHMENT1);
	}
	{
		auto* depth_stencil_texture = new lgl::Texture();
		depth_stencil_texture->Bind();

		depth_stencil_texture->Load2D(0, lgl::DEPTH24_STENCIL8, width, height, 0, lgl::DEPTH_STENCIL, lgl::UNSIGNED_INT_24_8, nullptr);

		depth_stencil_texture->UnBind();
		postprocess_fbo->AttachTexture2D(lgl::DEPTH_STENCIL_ATTACHMENT, depth_stencil_texture->mode, depth_stencil_texture->id, 0);
	}

	postprocess_fbo->SetDrawAttachments(draw_attachments);

	assert(postprocess_fbo->IsFrameBufferComplete());

	UnBind();

}

void lre::RenderTarget::BindReadOnly() {
	assert(postprocess_fbo != nullptr);

	postprocess_fbo->BindRead();
}

void lre::RenderTarget::BindDrawOnly() {
	assert(postprocess_fbo != nullptr);

	postprocess_fbo->BindDraw();
}

void lre::RenderTarget::UnBind() {
	lgl::FrameBuffer::UnBind();
}
