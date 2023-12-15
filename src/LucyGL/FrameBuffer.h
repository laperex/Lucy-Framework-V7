#pragma once

#include "Texture.h"

#include <vector>

namespace lgl {
	typedef unsigned int FrameBufferId;

	struct FrameBuffer {
		FrameBuffer(int width, int height, bool is_picking = false);
		FrameBuffer();
		~FrameBuffer();

		FrameBufferId id;
		int index;
		int width, height;

		Texture* picking = nullptr;
		Texture* texture = nullptr;

		void Bind();
		void BindRead();
		void BindDraw();
		static void UnBind();

		void AttachTexture2D(Attachment attachment, TextureMode target, TextureId texture, int level = 0);
		void AttachTexture2D(Attachment attachment, Texture* texture, int level = 0);

		void SetDrawAttachments(const std::vector<Attachment>& attachments);

		bool IsFrameBufferComplete();
	};
}