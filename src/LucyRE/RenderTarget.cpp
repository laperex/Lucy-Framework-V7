#include "RenderTarget.h"

lre::RenderTarget::~RenderTarget() {
	delete framebuffer;
}

void lre::RenderTarget::Initialize() {
	
}

void lre::RenderTarget::AddColorAttachment(int index) {
	
}

void lre::RenderTarget::SetAttachmentTexture(lgl::Attachment attachment, lgl::Texture* texture) {
	texture_store[attachment] = texture;
}

lgl::Texture* lre::RenderTarget::GetAttachmentTexture(lgl::Attachment attachment) {
	if (texture_store.find(attachment) == texture_store.end())
		return nullptr;

	return texture_store[attachment];
}

bool lre::RenderTarget::HasDepthBuffer() {
	return (GetAttachmentTexture(lgl::Attachment::DEPTH_ATTACHMENT) != nullptr || GetAttachmentTexture(lgl::Attachment::DEPTH_STENCIL_ATTACHMENT) != nullptr);
}

bool lre::RenderTarget::HasStencilBuffer() {
	return (GetAttachmentTexture(lgl::Attachment::DEPTH_STENCIL_ATTACHMENT) != nullptr);
}

bool lre::RenderTarget::HasBuffer(lgl::Attachment attachment) {
	return (GetAttachmentTexture(attachment) != nullptr);
}

void lre::RenderTarget::ReadPixel(lgl::Attachment attachment, const glm::vec2& pos, float* pixel) {
	lgl::SetReadBuffer(attachment);
	lgl::ReadPixels(pos.x, pos.y, 1, 1, lgl::RGBA, lgl::FLOAT, pixel);
	lgl::ResetReadBuffer();
}

void lre::RenderTarget::BindReadOnly() {
	
}

void lre::RenderTarget::UnBind() {
	
}

bool lre::RenderTarget::HasColorBuffer(int index) {
	return (GetAttachmentTexture((lgl::Attachment)(lgl::COLOR_ATTACHMENT0 + index)) != nullptr);
}

bool lre::RenderTarget::HasColorBuffer() {
	for (uint32_t i = lgl::COLOR_ATTACHMENT0; i <= lgl::COLOR_ATTACHMENT31; i++) {
		if (GetAttachmentTexture((lgl::Attachment)i) != nullptr) {
			return true;
		}
	}

	return false;
}
