#pragma once

#include <LucyGL/LucyGL.h>
#include <unordered_map>
#include <Lucy/Math.h>//#include <glm/glm.hpp>

namespace lre {
	enum RenderTargetFlags_ {
		RenderTargetFlags_None,
		RenderTargetFlags_Picking,
		RenderTargetFlags_Depth,
		RenderTargetFlags_DepthStencil,
		RenderTargetFlags_Stencil,
		RenderTargetFlags_MultiSampling,

		RenderTargetFlags_COUNT
	};

	enum RenderTargetAttchments_ {
		RenderTargetAttchments_None,
		RenderTargetAttchments_Picking,
		RenderTargetAttchments_Sampling,
		RenderTargetAttchments_Texture,
		RenderTargetAttchments_Depth,
		RenderTargetAttchments_Stencil,
		RenderTargetAttchments_DepthStencil,

		RenderTargetAttchments_COUNT
	};

	struct RenderTarget {
	private:
		lgl::FrameBuffer* postprocess_fbo = nullptr;
		lgl::FrameBuffer* msaa_fbo = nullptr;
		std::unordered_map<lgl::Attachment, lgl::Texture*> texture_store;

		RenderTargetFlags_ flags = RenderTargetFlags_None;

	public:
		RenderTarget() {}
		~RenderTarget();

		void Initialize(int width, int height, RenderTargetFlags_ flags);
		// void AddColorAttachment(int index);
		// void SetAttachmentTexture(lgl::Attachment attachment, lgl::Texture* texture);
		// lgl::Texture* GetAttachmentTexture(lgl::Attachment attachment);

		// bool HasDepthBuffer();
		// bool HasStencilBuffer();
		// bool HasColorBuffer(int index);
		// bool HasColorBuffer();
		// bool HasBuffer(lgl::Attachment attachment);

		// void ReadPixel(lgl::Attachment attachment, const glm::vec2& pos, float* pixel);

		void BindReadOnly();
		void BindDrawOnly();
		void Bind();
		static void UnBind();

		// glm::vec4 Picking();
	};
}
