#pragma once

#include "Types.h"
#include <string>
#include <stddef.h>

namespace lgl {
	typedef unsigned int TextureId;
	
	struct Texture {
		TextureId id;
		TextureMode mode;

		int width, height, channels;
		int tile_x = 0, tile_y = 0, tile_w = 0, tile_h = 0;

		TextureWrapMode wrap_s = WrapMode_None;
		TextureWrapMode wrap_t = WrapMode_None;
		TextureWrapMode wrap_r = WrapMode_None;
		TextureFilteringMode mag = FilterMode_None;
		TextureFilteringMode min = FilterMode_None;

		Texture(TextureMode mode = TEXTURE_2D);
		~Texture();

		void SetMode(lgl::TextureMode mode);

		bool LoadTexture(const char* filename, bool flip = false);
		bool LoadSpriteSheet(const char* filename, int x, int y, int w, int h, bool flip = false);

		void Load1D(int level, Format internalformat, int width, int border, Format format, Type type, void* data = nullptr);
		void LoadSub1D(int level, int x, int width, Format format, Type type, void* data = nullptr);

		void Load2D(int level, Format internalformat, int width, int height, int border, Format format, Type type, void* data = nullptr);
		void LoadSub2D(int level, int x, int y, int width, int height, Format format, Type type, void* data = nullptr);

		void Load3D(int level, Format internalformat, int width, int height, int depth, int border, Format format, Type type, void* data = nullptr);
		void LoadSub3D(int level, int x, int y, int z, int width, int height, int depth, Format format, Type type, void* data = nullptr);

		void GenerateMimmap();
		void SetWrapMode(TextureWrapMode wrap_s = WrapMode_None, TextureWrapMode wrap_t = WrapMode_None, TextureWrapMode wrap_r = WrapMode_None);
		void SetFilteringMode(TextureFilteringMode mag = FilterMode_None, TextureFilteringMode min = FilterMode_None);

		void Bind();
		void BindUnit(const unsigned int unit);
		void UnBind();

		static void UnBind(const lgl::TextureMode mode);
		static void BindUnit(const unsigned int unit, const TextureId id);
	};
}