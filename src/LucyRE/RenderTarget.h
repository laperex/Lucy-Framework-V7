#pragma once

#include <LucyGL/LucyGL.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace lre {
	enum RenderTargetFlags_ {
		RenderTargetFlags_,
		RenderTargetFlags_Picking,
		RenderTargetFlags_MultiSampling,
		RenderTargetFlags_COUNT
	};

	struct RenderTarget {
	private:
		lgl::FrameBuffer* framebuffer = nullptr;
		std::unordered_map<lgl::Attachment, lgl::Texture*> texture_store;

		lgl::Attachment picking_attachment = lgl::Attachment::Attachment_NONE;

	public:
		RenderTarget() {}
		~RenderTarget();

		void Initialize();
		void AddColorAttachment(int index);
		void SetAttachmentTexture(lgl::Attachment attachment, lgl::Texture* texture);
		lgl::Texture* GetAttachmentTexture(lgl::Attachment attachment);

		bool HasDepthBuffer();
		bool HasStencilBuffer();
		bool HasColorBuffer(int index);
		bool HasColorBuffer();
		bool HasBuffer(lgl::Attachment attachment);

		void ReadPixel(lgl::Attachment attachment, const glm::vec2& pos, float* pixel);

		void BindReadOnly();
		void BindWriteOnly();
		void Bind();
		static void UnBind();

		// glm::vec4 Picking();
	};
}
